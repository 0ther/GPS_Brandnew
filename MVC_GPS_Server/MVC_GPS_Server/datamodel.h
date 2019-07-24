#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include "log.h"

enum MESSAGESFROMSERVER { ERR, GOODCONNECT, GOODAUTH, ENDCONNECT, ACKNOWLEDGE }; //���� ���������, ���������� � �������
enum MESSAGETYPES { SERVICE, LOG, NMEA }; //��� ��� �� ������
enum SERVICETYPES { BEGIN, STARTSEND, STOPSEND, End, FATALERR }; //��� ���� ��������� ���������
enum INTMSGTYPES { ERRTYPE, SUCREG, SUCCON, BADCON, LOGOUT, END }; //��� ��� � ������������

enum SELECTIONTYPES { TOP, BOTTOM, ALL };

class DATAMODEL {
public:
	void Refresh(); //������� ������� ������ ������
	void Select();
	void Selection(SELECTIONTYPES); //������ ������� �� �������
	void Visualise(std::string, std::string); //������������� ������ � �������
};


