#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include <string>
#include <regex>
#include <iostream>
#include <mutex>

#include "rapidjson/document.h"//� ��� � ������� ����������
#include "rapidjson/stringbuffer.h"
//#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "RMC_Parser.h" //� ��� � ���������� ��� �������; ��� ��������� RMC, ������ � �������� �������� ������ �� �����������

//#pragma comment(lib, "LibRMCParser.lib")

using namespace rapidjson;

enum FATALSINCLIENT { WSAStartupError, FailConnect, FailMessageExchange }; //��� ��� � ������������
enum MESSAGESFROMSERVER { ERR, GOODCONNECT, GOODAUTH, ENDCONNECT, ACKNOWLEDGE }; //���� ���������, ���������� � �������
enum MESSAGETYPES { SERVICE, LOG, RMCMSG }; //��� ��� �� ������
enum SERVICETYPES { BEGIN, STARTSEND, STOPSEND, End, FATALERR }; //��� ���� ��������� ���������
enum INTMSGTYPES { ERRTYPE, SUCREG, SUCCON, BADCON, LOGOUT, END }; //��� ��� � ������������
enum FATALERROSINSERVER { LOGLIMIT, SUDDENOUT, NOAPPROVE, FORMERROR }; //��� ��� �� ������


class ManModel {
public:
  ManModel() { Latitude = 51.6592; Longitude = 39.2269; };
	ManModel(float a, float b) { Latitude = a; Longitude = b; }
	void Update();
	float Get_Latitude() { return Latitude; };
	float Get_Longitude() { return Longitude; };
  void Set_Latitude(float A) { Latitude = A; }
  void Set_Longitude(float A) { Longitude = A; }
private:
	float Latitude;
	float Longitude;
};


class DATAMODEL : public ManModel {
public:
	DATAMODEL() : ManModel() {};
	DATAMODEL(float a, float b) : ManModel(a, b) {};

	//������, ���������� �� �������� ��������� ������ �����
	void Generate_NMEA();
	//RMC Parse_RMC(std::string);
	void GenerateGPSJSON();

  std::mutex NMEAMutex; //�������� �� ������ � ����� � NMEA-�������� (history.txt)
  std::mutex JSONMutex; //�������� �� ������ � ����� temp.json, ���� ������������ ��� json-�
  std::string GetLastLine(std::string);
};

rapidjson::Document toJSON();