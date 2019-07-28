#include "pch.h"
#include "ClientController.h"

void CONTROLLER::Connect() {
	SOCKET Connection;
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			GiveMessageToUser(ERR);
			ThrowFatal(WSAStartupError);
		}

		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(1111);
		addr.sin_family = AF_INET;

		Connection = socket(AF_INET, SOCK_STREAM, NULL);
		if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
			GiveMessageToUser(ERR);
			ThrowFatal(FailConnect);
		}

		SetSocket(Connection);
		GiveMessageToUser(SUCCON);
    GenerateServJSON(BEGIN);
    SendSingle(SERVICE);

    if (ReceiveSingle() != GOODCONNECT) {
      //std::cout << "������ ������������ �����" << std::endl;
			GiveMessageToUser(ERR);
			ThrowFatal(FailMessageExchange);
    }
};


void CONTROLLER::Disconnect() {
	//���������� ������
	bool flag = false;
	SwitchNMEAThread(flag);
	SwitchJSONThread(flag);
  Sleep(2000);
  GenerateServJSON(STOPSEND); //��� ����� 3, � �� 4, ���� ��������� �� �������
  SendSingle(SERVICE);
  if (ReceiveSingle() == ENDCONNECT) {
    GenerateServJSON(END); //��� ����� 4, � �� 5, ���� ��������� �� �������
    SendSingle(SERVICE);
    //std::cout << "��������� � �������� ����������" << std::endl;
    //��������� �����
  }
  Sleep(1000);
	WSACleanup();
	closesocket(MyServer);
	GiveMessageToUser(BADCON);
};

enum MSGSizes {SERVSIZE = 4, LOGSIZE = 5, RMCSIZE = 9};

void CONTROLLER::SendSingle(int input) {
  std::lock_guard<std::mutex> lock(JSONMutex);
	int N = 0;
	std::ifstream filename;
	switch (input) {
	case SERVICE: //��������� ���������� ���������
		N = SERVSIZE;
		filename.open("service.json");
		break;
	case LOG: //��������� � ������� � �������
		N = LOGSIZE;
		filename.open("login.json");
		break;
	case RMCMSG: //��������� � ������������
		N = RMCSIZE;
		filename.open("temp.json");
		break;
	}
	std::string msg1;
	char buff[60];
	for (int i = 0; i < N; i++) {
		filename.getline(buff, 60);
		msg1 = buff;
		//std::cout << msg1 << std::endl;
		int msg_size = msg1.size();
		send(MyServer, msg1.c_str(), msg_size, NULL);
		Sleep(30);
	}
	//std::cout << "Sent!" << std::endl;
	filename.close();
};


int CONTROLLER::ReceiveSingle() {
	for (int i = 0; i < 3; i++) {
		char msg[60] = {};
		recv(MyServer, msg, sizeof(msg), NULL);
    //std::cout << msg << std::endl;
		Sleep(30);
		if (i == 1) {
			int answer = msg[14] - '0'; //��� ��� ����������� ������ �������, �������
      //std::cout << answer << std::endl;
      recv(MyServer, msg, sizeof(msg), NULL); //�������� ����������� ����ku
			//std::cout << "��������� �������!" << std::endl;
      return answer;
		}
	}
  return 0;
};


int CONTROLLER::Authorisation() {
	std::string login, password;
	std::cout << "������� �����: ";
	std::cin >> login;
	std::cout << std::endl << "������� ������: ";
	std::cin >> password;
	std::cout << std::endl;
	if (CheckLogData(login, password)) {
	GenerateLogJSON(login, password);
	SendSingle(LOG);

	if (ReceiveSingle() == GOODAUTH) {
		std::cout << "����������!" << std::endl;
		bool flag = true;
		SwitchNMEAThread(flag);
		SwitchJSONThread(flag);
		return 1; //������ �������� �����
	}
	else {
		AuthTries--;
		if (!AuthTries) return 0;
		std::cout << "Error! " << AuthTries << " tries last" << std::endl;
		Sleep(1000);
		Authorisation();
	};
  }
	else {
		AuthTries--;
		if (!AuthTries) return 0;
		std::cout << "Error! " << AuthTries << " tries last" << std::endl;
		Sleep(1000);
		Authorisation();
	}
};


void CONTROLLER::ThrowFatal(int input) {
	switch (input) {
	case WSAStartupError: {
		std::cout << "������ ������ ���������� �������" << std::endl;
		break;
	}
	case FailConnect: {
		std::cout << "������ ��������� ����������" << std::endl;
		break;
	}
	case FailMessageExchange: {
		std::cout << "������ ���������������� ������ �����������" << std::endl;
		break;
	}
	default: break;
	}
	exit(0);
}


void CONTROLLER::GiveMessageToUser(int input) {
	switch (input) {
	case ERRTYPE: //��������� �� ������
		std::cout << "������!" << std::endl;
		break;
	case SUCREG: //�������� ���������� �����������
		std::cout << "�����������!" << std::endl;
		break;
	case SUCCON: //�������� ��������� ���������� � ��������
		std::cout << "���������� �����������!" << std::endl;
		break;
	case BADCON:
		std::cout << "���������� ���������!" << std::endl;
		break;
	case LOGOUT: //������� ������������
		std::cout << "������������!" << std::endl;
		break;
	case END: //������� �������� �������� � ����������
		std::cout << "�����!" << std::endl;
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
		SendSingle(RMCMSG);
		//ReceiveSingle();
		Sleep(260);
		counter++;
		//std::cout << counter << "messages sent" << std::endl;
		if (counter > 15) {
			char buf[60];
			if (recv(MyServer, buf, sizeof(buf), NULL)) {
				//std::cout << buf << std::endl;
				if (counter > 30)	
					if (buf[15] != '4') /*std::cout << "����������������" << std::endl;*/
					{
						GenerateFATAL(FATALERR, NOAPPROVE);
						SendSingle(SERVICE);
					}
			}
		}
	}
};

//������, ���������� �� �������� �������
void CONTROLLER::SwitchNMEAThread(bool& flag) { //1 - ��������, 0 - ���������
	if (flag) {
    GenFlag = 1;
		std::thread NMEA(&CONTROLLER::NMEAInAThread, this, std::ref(GenFlag));
		NMEA.detach();
	}
	else {
		GenFlag = 0;
	}
}; //����� ��������� ����� NMEA


void CONTROLLER::SwitchJSONThread(bool& flag) {
	if (flag) {
    SenFlag = 1;
		std::thread JSON(&CONTROLLER::JSONInAThread, this, std::ref(SenFlag));
		JSON.detach();//��� �� �����, ����� ����� ���������� �� ��������� �����
	}
	else {
		SenFlag = 0;
	}
}; //�����, ������������ ��������� � JSON

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


bool CheckLogData(std::string login, std::string pass) {
	if (login.size() != 6 || pass.size() != 6) return false;
	for (int i = 0; i < 6; i++) {
		char temp = login[i];
		if ((int)temp < 97 || (int)temp > 122) return false; //������ 6 ��������� ��������
	}
	for (int i = 0; i < 6; i++) {
		char temp = pass[i];
		if ((int)temp < 48 || (int)temp > 57) return false; //������ 6 ����
	}
	return true;
};