#include "pch.h"
#include "Visualiser.h"

void Refill() {
	std::ifstream infile;
	infile.open("userDB.txt");

	std::ifstream donor;
	donor.open("mapscript.js");

	std::string bufferdonor, bufferdb;

	std::ofstream outfile;
	outfile.open("out.js");

	std::string issimilar = "";

	while (getline(donor, bufferdonor)) {
		if (bufferdonor.find("center") != bufferdonor.npos) {
			getline(infile, bufferdb);
			if (bufferdb != "") outfile << GenCenter(bufferdb) << std::endl;
			continue;
		}
		if (bufferdonor.find("lat") != bufferdonor.npos) {
			while (getline(infile, bufferdb)) {
				std::string result = Parse(bufferdb);
				if (result != issimilar)
				{
					outfile << result << std::endl;
					issimilar = result;
				}
			}
			continue;
		}
		outfile << bufferdonor << std::endl;
	}

	infile.close();
	donor.close();
	outfile.close();
};


void ClearScript() {
	std::ifstream infile;
	infile.open("mapscript.js");

	std::string buffer;

	std::ofstream outfile;
	outfile.open("out.js");

	while (getline(infile, buffer)) {
		if (buffer.find("center") == buffer.npos && buffer.find("[") == buffer.npos && buffer.find("]") == buffer.npos && buffer.find("lat") == buffer.npos)
		{
			outfile << buffer << std::endl;
		}
	}
	infile.close();
	outfile.close();
};


std::string Parse(std::string aim) {
	if (aim == "") return "";
	std::string* mass = new std::string[2];
	mass[0] = aim.substr(10, 7);
	mass[1] = aim.substr(20, 7);

	for (int i = 0; i < 2; i++) {
		if (mass[i][0] == '0') mass[i].erase(0, 1);
		mass[i].erase(mass[i].find("."), 1);
		mass[i].insert(2, ".");
		if (mass[i].size() > 6) mass[i].erase(6, 1);
	}

	std::string out = "{lat: ";
	out += mass[0];
	out += ", lng: ";
	out += mass[1];
	out += "},";

	return out;
};


std::string GenCenter(std::string aim) { // center: {lat: 51.65, lng: 39.22},
	std::string* mass = new std::string[2];
	mass[0] = aim.substr(10, 7);
	mass[1] = aim.substr(20, 7);

	for (int i = 0; i < 2; i++) {
		if (mass[i][0] == '0') mass[i].erase(0, 1);
		mass[i].erase(mass[i].find("."), 1);
		mass[i].insert(2, ".");
		while (mass[i].size() > 5) mass[i].erase(5, 1);
	}
	std::string out = "center: {lat: ";
	out += mass[0];
	out += ", lng: ";
	out += mass[1];
	out += "},";

	return out;
}