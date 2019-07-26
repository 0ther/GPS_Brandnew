#include "pch.h"
#include "ClientController.h"

void CONTROLLER::Connect() {
	SOCKET Connection;
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			GiveMessageToUser(ERR);
			exit(1);
		}

		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(1111);
		addr.sin_family = AF_INET;

		Connection = socket(AF_INET, SOCK_STREAM, NULL);
		if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
			GiveMessageToUser(ERR);
			exit(1);
		}

		SetSocket(Connection);
		GiveMessageToUser(SUCCON);
    GenerateServJSON(BEGIN);
    SendSingle(SERVICE);

    if (ReceiveSingle() != GOODCONNECT) {
      std::cout << "Пришел некорректный ответ" << std::endl;
			GiveMessageToUser(ERR);
      exit(2);
    }
};


void CONTROLLER::Disconnect() {
	//Остановить потоки
	bool flag = false;
	SwitchNMEAThread(flag);
	SwitchJSONThread(flag);
  Sleep(2000);
  GenerateServJSON(STOPSEND); //Вот здесь 3, а не 4, надо поправить на сервере
  SendSingle(SERVICE);
  if (ReceiveSingle() == ENDCONNECT) {
    GenerateServJSON(END); //Вот здесь 4, а не 5, надо поправить на сервере
    SendSingle(SERVICE);
    //std::cout << "Сообщение о закрытии отправлено" << std::endl;
    //Закрываем сокет
  }
  Sleep(1000);
	WSACleanup();
	closesocket(MyServer);
	GiveMessageToUser(BADCON);
};


void CONTROLLER::SendSingle(int input) {
  std::lock_guard<std::mutex> lock(JSONMutex);
	int N = 0;
	std::ifstream filename;
	switch (input) {
	case SERVICE: //Сообщение служебного характера
		N = 4;
		filename.open("service.json");
		break;
	case LOG: //Сообщение с логином и паролем
		N = 5;
		filename.open("login.json");
		break;
	case NMEA: //Сообщение с координатами
		N = 9;
		filename.open("temp.json");
		break;
	}
	std::string msg1;
	char buff[60];
	for (int i = 0; i < N; i++) {
		filename.getline(buff, 60);
		msg1 = buff;
		std::cout << msg1 << std::endl;
		int msg_size = msg1.size();
		send(MyServer, msg1.c_str(), msg_size, NULL);
		Sleep(30);
	}
	std::cout << "Sent!" << std::endl;
	filename.close();
};


int CONTROLLER::ReceiveSingle() {
	for (int i = 0; i < 3; i++) {
		char msg[60] = {};
		recv(MyServer, msg, sizeof(msg), NULL);
    std::cout << msg << std::endl;
		Sleep(30);
		if (i == 1) {
			int answer = msg[14] - '0'; //Вот тут пересчитать индекс массива, сделано
      //std::cout << answer << std::endl;
      recv(MyServer, msg, sizeof(msg), NULL); //Получаем закрывающую скобku
			std::cout << "Сообщение принято!" << std::endl;
      return answer;
		}
	}
  return 0;
};


int CONTROLLER::Authorisation() {
	Connect();
	std::string login, password;
	std::cout << "Введите логин: ";
  std::cin >> login;
	std::cout << std::endl << "Введите пароль: ";
  std::cin >> password;
	std::cout << std::endl;
	GenerateLogJSON(login, password);
    SendSingle(LOG);
    
    if (ReceiveSingle() == GOODAUTH) {
      std::cout << "Получилось!" << std::endl;
      bool flag = true;
      SwitchNMEAThread(flag);
      SwitchJSONThread(flag);
      return 1; //Потоки включать здесь
    }
	else return 0;
};


void CONTROLLER::GiveMessageToUser(int input) {
	switch (input) {
	case ERR: //Сообщение об ошибке
		std::cout << "Ошибка!" << std::endl;
		break;
	case SUCREG: //Успешное завершение регистрации
		std::cout << "Авторизован!" << std::endl;
		break;
	case SUCCON: //Успешная настройка соединения с сервером
		std::cout << "Соединение установлено!" << std::endl;
		break;
	case BADCON:
		std::cout << "Соединение разорвано!" << std::endl;
		break;
	case LOGOUT: //Успешно разлогинился
		std::cout << "Разлогинился!" << std::endl;
		break;
	case END: //Успешно закончил работать с программой
		std::cout << "Выход!" << std::endl;
		break;
	}
};


void CONTROLLER::GenerateLogJSON(std::string login, std::string password) {
	std::lock_guard<std::mutex> lock(JSONMutex);
	std::ofstream fout("login.json");
	if (fout.is_open()) {
		fout << "{\n";
		fout << " \"Type\" : \"" << LOG << "\", \n";
		fout << " \"Login\" : \"" << login << "\", \n";
		fout << " \"Password\" : \"" << password << "\" \n";
		fout << "}";
		fout.close();
	}
};


void CONTROLLER::GenerateServJSON(int type) {
  std::lock_guard<std::mutex> lock(JSONMutex);
	std::ofstream fout("service.json");
	if (fout.is_open()) {
		fout << "{\n";
		fout << " \"Type\" : \"" << SERVICE << "\", \n";
		fout << " \"Message\" : \"" << type << "\" \n";
		fout << "}";
		fout.close();
	}
};


void CONTROLLER::NMEAInAThread(bool& flag) {
	while (flag) {
		Generate_NMEA();
		Sleep(965);
	}
};

static int counter = 0;

void CONTROLLER::JSONInAThread(bool& flag) {
	while (flag) {
		Sleep(500);
		GenerateGPSJSON();
		SendSingle(NMEA);
		//ReceiveSingle();
		Sleep(260);
		counter++;
		std::cout << counter << "messages sent" << std::endl;
		if (counter > 15) {
			char buf[60];
			if (recv(MyServer, buf, sizeof(buf), NULL)) {
				std::cout << buf << std::endl;
				if (counter > 30)	
					if (buf[15] == '4') std::cout << "Квитанцияяяяяяяя" << std::endl;
					else {
						GenerateFATAL(FATALERR, NOAPPROVE);
						SendSingle(SERVICE);
					}
			}
		}
	}
};

//Методы, отвечающие за создание потоков
void CONTROLLER::SwitchNMEAThread(bool& flag) { //1 - включить, 0 - выключить
	if (flag) {
    GenFlag = 1;
		std::thread NMEA(&CONTROLLER::NMEAInAThread, this, std::ref(GenFlag));
		NMEA.detach();
	}
	else {
		GenFlag = 0;
	}
}; //поток генерации строк NMEA


void CONTROLLER::SwitchJSONThread(bool& flag) {
	if (flag) {
    SenFlag = 1;
		std::thread JSON(&CONTROLLER::JSONInAThread, this, std::ref(SenFlag));
		JSON.detach();//нам не нужно, чтобы поток прерывался до обнуления флага
	}
	else {
		SenFlag = 0;
	}
}; //поток, записывающий структуру в JSON

void CONTROLLER::GenerateFATAL(int input, int reason) {
	std::lock_guard<std::mutex> lock(JSONMutex);
	std::ofstream fout("service.json");
	if (fout.is_open()) {
		fout << "{\n";
		fout << " \"Type\" : \"" << SERVICE << "\", \n";
		fout << " \"Message\" : \"" << FATALERR << "\", \n";
		fout << " \"Reason\" : \"" << reason << "\" \n";
		fout << "}";
		fout.close();
	}
}