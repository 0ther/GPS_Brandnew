#include "pch.h"
#include "servcontroller.h"


void CONTROLLER::Connect() {
  WSAData wsaData;
  WORD DLLVersion = MAKEWORD(2, 1);

  if (WSAStartup(DLLVersion, &wsaData) != 0) {
    //std::cout << "Error" << std::endl;
    exit(1);
  }

  SOCKADDR_IN addr;
  int sizeofaddr = sizeof(addr);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr.sin_port = htons(1111);
  addr.sin_family = AF_INET;
  SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
  bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
  listen(sListen, SOMAXCONN);

  SOCKET newConnection;
  newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

  if (newConnection == 0) {
    //��� ������ ������
  }
  else {
    Sock = newConnection;
    //ReceiveSingle();
    //Sleep(500);
    //SendSingle(4);
    bool start = true;
    SwitchReceivingSocket(start);
  }
}


void CONTROLLER::ReceiveSingle() {
  for (int i = 0; i < 2; i++) {
    char msg[60] = {};
    recv(Sock, msg, sizeof(msg), NULL); //�� ������ �������� ����� ��� ���������, ������ ��������� ��-�������
    //std::cout << msg << std::endl;
    if (i == 1) {
      if (msg[11] == '0') { //��������, ��� ��������� ���������� ���������
        //std::cout << "�������� ��������� ���������" << std::endl;
        ReceiveRestOfJSON(SERVICE);
      }
      else if (msg[11] == '1') { //��������, ��� ��������� � ������� � �������
        //std::cout << "�������� ��������� � ������� � �������" << std::endl;
        ReceiveRestOfJSON(LOG);
      }
      else if (msg[11] == '2') { //��������, ��� ��������� � ������������
        //std::cout << "�������� ��������� � ������������" << std::endl;
        ReceiveRestOfJSON(NMEA);
      }
      //else std::cout << "�� ���������������!" << std::endl;
    }
    Sleep(30);
  };
  //std::cout << "��������� �������!" << std::endl;
};


void CONTROLLER::ReceiveRestOfJSON(int type) {
  std::ofstream Database_1("temp.json", std::ios::out);
  Database_1 << "{" << std::endl;
  int N = 0;
  switch (type) {
  case SERVICE: //��������� ���������, � ����� ��� 2 �������
  {
    N = 2;
    break;
  }
  case LOG: //��������� � �������, 3 �������
  {
    N = 3;
    break;
  }
  case NMEA: //��������� � ������������, 7 �������
  {
    N = 7;
    break;
  }
  }
  for (int i = 0; i < N; i++) {
    Sleep(30);
    char msg[60] = {};
    recv(Sock, msg, sizeof(msg), NULL);
    //std::cout << msg << std::endl;
    Database_1 << msg << std::endl;
  }
  switch (type) {
  case SERVICE: 
  {
    ParseService();
    break;
  }
  case LOG: 
  {
    ParseLogin();
    break;
  }
  case NMEA: 
  {
    ParseNMEA();
    break;
  }
  }
};


void CONTROLLER::ReceiveInAThread(bool& flag) {
  while (flag) {
    ReceiveSingle();
  }
};


void CONTROLLER::SwitchReceivingSocket(bool& flag) {
  if (flag) {
    RecFlag = true;
    std::thread NMEA(&CONTROLLER::ReceiveInAThread, this, std::ref(RecFlag));
    NMEA.detach();
  }
  else {
    RecFlag = false;
    //std::cout << "������ �������!" << std::endl;
  }
};


void CONTROLLER::SendSingle(int type) {
  FormJSON(type);
  //std::cout << "Message formed!" << std::endl;
  std::ifstream in;
  in.open("send.json");
  std::string msg1;
  char buff[60];
  for (int i = 0; i < 3; i++) {
    in.getline(buff, 60);
    msg1 = buff;
    int msg_size = msg1.size();
    //std::cout << msg1 << std::endl;
    send(Sock, msg1.c_str(), msg_size, NULL);
    Sleep(30);
  };
  //std::cout << "Message sent!" << std::endl;
}


void CONTROLLER::ParseService() {
  std::ifstream DB;
  DB.open("temp.json", std::ios::app);
  std::string buf;
  if (DB.is_open()) {
    for (int i = 0; i < 2; i++) {
      getline(DB, buf);
      if (i == 1) {
        for (int i = 0; i < 3; i++)
          buf.erase(0, buf.find_first_of("\"") + 1);
        buf.erase(buf.find_first_of("\""), buf.npos);
        int type = std::stoi(buf);
        switch (type) {
        case BEGIN: //������ ������� � ���, ��� ����� ������ ������ � �������� (� ���?)
        {
          SendSingle(GOODCONNECT);
          break;
        }
        case STARTSEND: //������ �������, ��� ������ ����� ���������� ������
        {
          SendSingle(GOODAUTH); //����������������, ���� ���
          break;
        }
        case STOPSEND: //�������� ���������� ������
        {
          SendSingle(ENDCONNECT);
          bool off = false;
          SwitchReceivingSocket(off);
          ReceiveSingle();
          break;
        }
        case END: //������ ������� � ���, ��� ����� ��������� ������ � ��������
        {
          //std::cout << "��������� � �������� ��������" << std::endl;
          break;
        }
        default: //��� �� �� �������
        {
          SendSingle(ERR);
          break;
        }
        }
      }
    }
  }
  Sleep(940);
};


void CONTROLLER::ParseLogin() {
  std::ifstream DB;
  DB.open("temp.json");
  std::string login, pass, buf;
  if (DB.is_open()) {
    for (int i = 0; i < 3; i++) {
      getline(DB, buf);
      if (i == 0) continue;
      for (int i = 0; i < 3; i++) buf.erase(0, buf.find_first_of("\"") + 1);
      buf.erase(buf.find_first_of("\""), buf.npos);
      if (i == 1) login = buf;
      if (i == 2) pass = buf;
    }
    bool found = FindLogs("logusers.txt", login, pass);
    if (found) SendSingle(GOODAUTH);
    else SendSingle(ERR);
    DB.close();
  }
  Sleep(910);
};


void CONTROLLER::ParseNMEA() {
  std::ifstream DB;
  DB.open("temp.json");
  std::string login, pass, buf;

  if (DB.is_open()) {
    std::string Ranswer, answer;
    while (getline(DB, answer))
    {
      if (answer.size() == 0) continue;
      answer.erase(0, 1);
      if (answer.size() == 0) continue;
      for (int i = 0; i < 3; i++)
      {
        answer.erase(0, answer.find_first_of("\"") + 1);
      }
      if (answer.size() == 0) continue;
      else {
        if (answer.find_first_of("\"") != -1) {
          answer.erase(answer.find_first_of("\""), answer.npos);
        }
      }
      Ranswer += answer;
      Ranswer += " ";
    }
    //std::cout << std::endl << "������ ������: " << Ranswer.size() << std::endl;
    if (Ranswer.size() == 38) Write(Ranswer);
  }
  Sleep(500);
};


bool CONTROLLER::Authorisation(std::string login, std::string password) {
  if (FindLogs("logadmins.txt", login, password)) return 1;
  else return 0;
}


void CONTROLLER::CloseConnection() {
  closesocket(Sock);
};


void FormJSON(int type) {
  std::ofstream fout("send.json");
  if (fout.is_open()) {
    fout << "{\n";
    fout << " \"Message\" : \"" << type << "\" \n";
    fout << "}";
    fout.close();
  }
};


bool FindLogs(std::string filename, std::string login, std::string password) {
  std::ifstream file(filename);
  std::string tmp;
  while (getline(file, tmp))
  {
    size_t pos = tmp.find(login);
    if (pos != std::string::npos)
    {
      size_t pos2 = tmp.find(password);
      if (pos2 != std::string::npos || password == "") return true;
    }
  }
  file.close();
  return false;
};


void Write(std::string NMEA) {
  std::string filename = "userDB.txt";
  std::ofstream out;
  out.open(filename, std::ios::app);
  out << NMEA << std::endl;
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


void CONTROLLER::AddUser() {
  std::cout << "������� ����� ������������" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;

  if (FindLogs("logusers.txt", templ, "")) {
    std::cout << "������: ������������ � ����� ������� ��� ����" << std::endl;
    AddUser();
  }
  else {
    std::cout << "������� ������ ������������" << std::endl;
    std::string pass;
    std::cin >> pass;
    std::cout << "���������� ������..." << std::endl;
    AddNewUser(FormId(), templ, pass);

    if (FindLogs("logusers.txt", templ, "")) {
      std::cout << "������ ������� ��������" << std::endl;
    }
    else {
      std::cout << "������: �� ������� �������� ������" << std::endl;
    }
  }
};


std::string FormId() {
  std::ifstream f;
  f.open("logusers.txt");
  if (!f.is_open())
  {
    //std::cerr << "Error open file" << std::endl;
  }
  std::string line;
  int lines_count = 0;
  while (getline(f, line))
  {
    if (!std::all_of(line.begin(), line.end(), isspace)) ++lines_count;
  }

  std::cout << "Not empty lines - " << lines_count << std::endl;
  lines_count++;
  std::string answer = {};
  if (lines_count < 10) answer += "0";
  answer += std::to_string(lines_count);
  return answer;
}


void AddNewUser(std::string id, std::string login, std::string pass) {
  std::ofstream out;
  out.open("logusers.txt", std::ios::app);
  out << "id=" << id << " " << login << " password=" << pass << "\n";
  out.close();
}


int LineOfLogin(std::string filename, std::string login) {
  std::ifstream f;
  f.open(filename);
  if (!f.is_open())
  {
    //std::cout << "Error open file" << std::endl;
  }
  std::string::size_type n;
  std::string line;
  int lines_count = 0;
  while (getline(f, line))
  {
    if (!std::all_of(line.begin(), line.end(), isspace)) ++lines_count;
    n = line.find(login, 0);
    if (n == 12) {
      std::cout << "����� ������ � " << lines_count << " ������" << std::endl;
      return lines_count;
    }
    else continue;
  }
  f.close();
};


void DeleteLineWithIdChanging(std::string filename, int number) {
  std::ifstream f;
  f.open(filename);
  std::ofstream out;
  out.open("out.txt");

  if (!f.is_open())
  {
    //std::cout << "Error open file" << std::endl;
  }
  bool isdeleted = 0;
  int lines_count = 0;
  std::string line;

  while (getline(f, line))
  {
    if (!std::all_of(line.begin(), line.end(), isspace)) ++lines_count;
    if (lines_count == number) { //������ ��������� �
      isdeleted = 1;
    }
    else { //� ��� ��������� ������� ������� � ����
      if (isdeleted) { //����� ��������� id, ������ ��� � ������� ���������� ��������
        line.erase(3, 2);
        line.insert(3, std::to_string(lines_count - 1));
        if (lines_count < 10) line.insert(3, "0");
        out << line << std::endl;
      }
      else {
        out << line << std::endl;
      }
    }
  }
  f.close();
  out.close();

  //����� � ��������� ������� ����� � ������� � ��������������� ����� ������
  if (remove("logusers.txt")) {
    //std::cout << "������: �� ������� ������� ����" << std::endl;
  }
  int result;
  char oldname[] = "out.txt";
  char newname[] = "logusers.txt";
  result = rename(oldname, newname);
  if (result == 0) {
    //std::cout << "�������������� ������� ���������" << std::endl;
    Sleep(1000);
  }
  else {
    //std::cout << "������: �� ������� ������������� ����" << std::endl;
    Sleep(1000);
  }
};


void CONTROLLER::DeleteUser() {
  //������� ������������
    /*
    ������� ���� � �������� ��������
    ����� ������ � ������ ������� �������
    ������� ��� ������
    �������� ���� ���� �������������, ������� ����, �� 1
    ������� ���� � �������� ��������
    */
  std::cout << "������� ����� ������������" << std::endl;
  std::string login;
  std::cin >> login;
  std::string templ = "login=";
  templ += login;

  if (FindLogs("logusers.txt", templ, "")) {
    int line = LineOfLogin("logusers.txt", login);
    //std::cout << "���������� � ��������" << std::endl;
    DeleteLineWithIdChanging("logusers.txt", line);
    Sleep(1000);
  }
  else {
    std::cout << "������: ������������ � ����� ������� ��� � �������" << std::endl;
    Sleep(1000);
    DeleteUser();
  }
};