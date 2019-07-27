#pragma once
#include <iostream>
#include <string>
#include <fstream>
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "DataModel.h"
//#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

class CONTROLLER : public DATAMODEL {
private:
  bool GenFlag = 0;
  bool SenFlag = 0;
public:
	CONTROLLER() : DATAMODEL() {};
  CONTROLLER(float a, float b) : DATAMODEL(a, b) {};
	//Функции, относящиеся к общению с сервером
	void Connect();
	void Disconnect();

	void SendSingle(int); //параметризуется тип
	int ReceiveSingle();
  void GenerateServJSON(int);
	void GenerateLogJSON(std::string, std::string);
	void GenerateFATAL(int, int);
  void NMEAInAThread(bool&); //1
  void JSONInAThread(bool&);
  //Методы, отвечающие за создание потоков
  void SwitchNMEAThread(bool&); //поток генерации строк NMEA //2
  void SwitchJSONThread(bool&); //поток, записывающий структуру в JSON

	//Функции, относящиеся к общению с Представлением
	void GiveMessageToUser(int);
	void ThrowFatal(int);
	int Authorisation();

	void SetSocket(SOCKET A) { MyServer = A; };
private:
	SOCKET MyServer = NULL;
	int AuthTries = 5;
};
