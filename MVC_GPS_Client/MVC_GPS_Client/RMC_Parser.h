#include <string>
#include <regex>
#pragma once

struct RMC {
	std::string Preamble, Time, Latitude, Longitude, Date, LaFlag, LoFlag, Sec1, Sec2;
};

RMC Parse_RMC(std::string input);

bool CheckIntegrity(RMC input);