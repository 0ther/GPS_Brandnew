#include "stdafx.h"
#include "RMC_Parser.h"


bool CheckIntegrity(RMC input) {
	//$GPRMC hhmmss.ss GGMM.MM P gggmm.mm J v.v,_b.b ddmmyy x.x,n,m*hh<CR><LF>
	if (input.Preamble != "$GPRMC" || input.Sec1 != "0.x,x.x," || input.Sec2 != "x.x,a,a*hh<CR><LF>") {
		//Ошибка формирования статических последовательностей
		return false;
	}
	if (input.LaFlag != "N" && input.LaFlag != "S" || input.LoFlag != "W" && input.LoFlag != "E") {
		//Ошибка формирования флагов
		return false;
	}
	//Регулярочки для остального? Даааааааааа
	std::cmatch Result;
	std::regex exprdate("[0-9]{6}");
	std::regex expr("([0-9]*.[0-9]{2})");

	if (!std::regex_match(input.Latitude.c_str(), Result, expr)) {
		return false;
	}
	if (!std::regex_match(input.Longitude.c_str(), Result, expr)) {
		return false;
	}
	if (!std::regex_match(input.Date.c_str(), Result, exprdate)) {
		return false;
	}
	if (!std::regex_match(input.Time.c_str(), Result, expr)) {
		return false;
	}
	return true;
}


RMC Parse_RMC(std::string input) { //сюда как аргумент передаётся функция GetLastLine("history.txt")
	RMC A;
	A.Preamble = input.substr(0, 6);
	A.Time = input.substr(7, 9);
	A.Latitude = input.substr(19, 7);
	A.LaFlag = input.substr(27, 1);
	A.Longitude = input.substr(29, 8);
	A.LoFlag = input.substr(38, 1);
	A.Sec1 = input.substr(40, 8);
	A.Date = input.substr(48, 6);
	A.Sec2 = input.substr(55, 18);
	if (CheckIntegrity(A))	return A;
	else return RMC();
};