#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include <string>
#include <regex>
#include <iostream>
#include <mutex>

#include "RMC_Parser.h" //А вот и библиотека для функции

#pragma comment(lib, "LibRMCParser.lib")


enum MESSAGESFROMSERVER { ERR, GOODCONNECT, GOODAUTH, ENDCONNECT, ACKNOWLEDGE }; //Типы сообщений, приходящих с сервера
enum MESSAGETYPES { SERVICE, LOG, NMEA }; //Это идёт на сервер
enum SERVICETYPES { BEGIN, STARTSEND, STOPSEND, End, FATALERR }; //Это типы сервисных сообщений
enum INTMSGTYPES { ERRTYPE, SUCREG, SUCCON, BADCON, LOGOUT, END }; //Это идёт к пользователю
enum FATALERROS { LOGLIMIT, SUDDENOUT, NOAPPROVE, FORMERROR };

/*
struct RMC {
	std::string Preamble, Time, Latitude, Longitude, Date, LaFlag, LoFlag, Sec1, Sec2;
};

*/


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

	//Методы, отвечающие за создание сообщений разных типов
	void Generate_NMEA();
	//RMC Parse_RMC(std::string);
	void GenerateGPSJSON();

  std::mutex NMEAMutex; //отвечает за доступ к файлу с NMEA-строками (history.txt)
  std::mutex JSONMutex; //отвечает за доступ к файлу temp.json, куда записываются все json-ы
  std::string GetLastLine(std::string);
};