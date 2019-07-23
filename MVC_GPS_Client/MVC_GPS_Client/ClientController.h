#pragma once
#include <iostream>
#include <string>
#include <fstream>
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "NMEAModel.h"
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

class CONTROLLER : public NMEAMODEL {
private:
  bool GenFlag = 0;
  bool SenFlag = 0;
public:
	CONTROLLER() : NMEAMODEL() {};
  CONTROLLER(float a, float b) : NMEAMODEL(a, b) {};
	//�������, ����������� � ������� � ��������
	void Connect();
	void Disconnect();

	void SendSingle(int); //��������������� ���
	int ReceiveSingle();
  void GenerateServJSON(int);
	void GenerateLogJSON(std::string, std::string);
  void NMEAInAThread(bool&); //1
  void JSONInAThread(bool&);
  //������, ���������� �� �������� �������
  void SwitchNMEAThread(bool&); //����� ��������� ����� NMEA //2
  void SwitchJSONThread(bool&); //�����, ������������ ��������� � JSON

	//�������, ����������� � ������� � ��������������
	void GiveMessageToUser(int);
	int Authorisation();

	void SetSocket(SOCKET A) { MyServer = A; };
private:
	SOCKET MyServer = NULL;
};