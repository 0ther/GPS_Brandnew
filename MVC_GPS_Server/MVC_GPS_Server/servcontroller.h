#pragma once
//����� ���������� ��������, ���������� ��������� � ��� � ��������
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <algorithm>
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include "datamodel.h"


enum FATALSINSERVER { WSAStartupError, FailConnect }; //��� ��� � ������������
enum FATALERRORSOFCLIENT { LOGLIMIT, SUDDENOUT, NOAPPROVE, FORMERROR };

class CONTROLLER : public DATAMODEL {
public:
	void ThrowFatal(int);
	bool Authorisation(std::string, std::string);
	void Connect();
	void CloseConnection();

	void SendSingle(int);
	void ParseService();
	void ParseLogin();
	void ParseNMEA();

	void ReceiveRestOfJSON(int);
	void ReceiveSingle();
	void ReceiveInAThread(bool&);
	void SwitchReceivingSocket(bool&);

	void AddUser();
	void DeleteUser();
private:
	SOCKET Sock;
	bool RecFlag = 0;
};



bool CheckLogData(std::string, std::string); //��������� ����� � ������ �� ������������ ��������
void FormJSON(int);
bool FindLogs(std::string, std::string, std::string); //��� ����� ����� � ���������� ������ � ������
void Write(std::string);
void AddNewUser(std::string, std::string, std::string);
std::string FormId();

static int counter = 7;