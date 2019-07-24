#include "pch.h"
#include "log.h"


std::ofstream logfile;


void WriteIntoLog(std::string Message) {
	logfile << Message << std::endl;
}


void CloseLog() {
	logfile.close();
}


void ClearLog() {
	logfile.open("server.log");
	logfile.clear();
}