#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>

enum MESSAGESFROMSERVER { ERR, GOODCONNECT, GOODAUTH, ENDCONNECT }; //Типы сообщений, приходящих с сервера
enum MESSAGETYPES { SERVICE, LOG, NMEA }; //Это идёт на сервер
enum SERVICETYPES { BEGIN, STARTSEND, STOPSEND, End }; //Это типы сервисных сообщений
enum INTMSGTYPES { ERRTYPE, SUCREG, SUCCON, BADCON, LOGOUT, END }; //Это идёт к пользователю

enum SELECTIONTYPES {TOP, BOTTOM, ALL};

class DATAMODEL {
public:
  void Refresh(); //Удаляет слишком старые записи
  void Select();
  void Selection(SELECTIONTYPES); //Делает выборку по запросу
  void Visualise(std::string, std::string); //Визуализирует данные в консоли
};


class JSONPARSER {

};