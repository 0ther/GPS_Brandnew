#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include <string>
#include <mutex>


enum MESSAGESFROMSERVER { ERR, GOODCONNECT, GOODAUTH, ENDCONNECT, ACKNOWLEDGE }; //���� ���������, ���������� � �������
enum MESSAGETYPES { SERVICE, LOG, NMEA }; //��� ��� �� ������
enum SERVICETYPES { BEGIN, STARTSEND, STOPSEND, End, FATALERR }; //��� ���� ��������� ���������
enum INTMSGTYPES { ERRTYPE, SUCREG, SUCCON, BADCON, LOGOUT, END }; //��� ��� � ������������


struct RMC {
	std::string Preamble, Time, Latitude, Longitude, Date, LaFlag, LoFlag, Sec1, Sec2;
};


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


class NMEAMODEL : public ManModel {
public:
	NMEAMODEL() : ManModel() {};
	NMEAMODEL(float a, float b) : ManModel(a, b) {};

	//������, ���������� �� �������� ��������� ������ �����
	void Generate_NMEA();
	RMC Generate_RMC(std::string);
	void GenerateGPSJSON();

  std::mutex NMEAMutex; //�������� �� ������ � ����� � NMEA-�������� (history.txt)
  std::mutex JSONMutex; //�������� �� ������ � ����� temp.json, ���� ������������ ��� json-�
  std::string GetLastLine(std::string);
};