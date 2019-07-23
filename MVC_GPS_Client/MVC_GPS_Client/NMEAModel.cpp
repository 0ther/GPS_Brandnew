#include "pch.h"
#include "NMEAModel.h"


void ManModel::Update() {
	SYSTEMTIME st;
	GetLocalTime(&st);
	if ((st.wMinute*st.wSecond) % 3 == 0) Latitude += 0.0001;
	else Longitude += 0.0001;
};


//Методы, отвечающие за генерацию сообщений
void NMEAMODEL::Generate_NMEA() { //генерирует и записывает в файл
	this->Update();
	SYSTEMTIME st;
	GetLocalTime(&st);

	std::string escape = "$GPRMC,";
	if (st.wHour < 10) escape += "0";
	escape += std::to_string(st.wHour);
	if (st.wMinute < 10) escape += "0";
	escape += std::to_string(st.wMinute);
	if (st.wSecond < 10) escape += "0";
	escape += std::to_string(st.wSecond);
	escape += ".";
	if (st.wMilliseconds < 100 && st.wMilliseconds >= 10) escape += "0";
	if (st.wMilliseconds < 10 && st.wMilliseconds >= 0) escape += "00";
	escape += std::to_string(st.wMilliseconds);
	escape.pop_back();
	escape += ",A,";

	//Пишем широту
	double lattemp = abs(this->Get_Latitude());
	double latsec = lattemp;
	escape += std::to_string(int(lattemp)); //Записали

	lattemp -= int(lattemp); //Тут выделили дробную часть числа
	lattemp *= 100; //Тут умножили дробную часть на 100, чтобы отсечь два знака
	lattemp = int(lattemp);
	escape += std::to_string(lattemp); //Записали минуты
	escape.erase(escape.find_last_not_of('0'), std::string::npos); //Убрали лишние нули
	escape += ".";

	latsec *= 10000;
	if (int(latsec) % 100 < 10) escape += "0";
	latsec = int(latsec) % 100;
	escape += std::to_string(latsec); //Записали секунды
	escape.erase(escape.find_last_not_of('0'), std::string::npos); //Убрали лишние нули

	if (this->Get_Latitude() > 0) {
		escape += ",N,";
	}
	else {
		escape += ",S,";
	}

	//Пишем долготу
	double lontemp = abs(this->Get_Longitude());
	double lonsec = lontemp;
	if (lontemp < 100) escape += "0";
	escape += std::to_string(int(lontemp)); //Записали

	lontemp -= int(lontemp); //Тут выделили дробную часть числа
	lontemp *= 100; //Тут умножили дробную часть на 100, чтобы отсечь два знака
	lontemp = int(lontemp);
	escape += std::to_string(lontemp); //Записали минуты
	escape.erase(escape.find_last_not_of('0'), std::string::npos); //Убрали лишние нули
	escape += ".";

	lonsec *= 10000;
	if (int(lonsec) % 100 < 10) escape += "0";
	lonsec = int(lonsec) % 100;
	escape += std::to_string(lonsec);
	escape.erase(escape.find_last_not_of('0'), std::string::npos); //Убрали лишние нули


	if (this->Get_Longitude() > 0) {
		escape += ",E,";
	}
	else {
		escape += ",W,";
	}

	escape += "0.x,x.x,";
	if (st.wDay < 10) escape += "0";
	escape += std::to_string(st.wDay);
	if (st.wMonth < 10) escape += "0";
	escape += std::to_string(st.wMonth);
	escape += std::to_string(st.wYear);
	escape.erase(escape.size() - 4, 2);
	escape += ",x.x,a,a*hh<CR><LF>";


	std::lock_guard<std::mutex> lock(NMEAMutex);
	std::ofstream out;
	out.open("history.txt", std::ios::app);
	out << escape << std::endl;
	out.close();
};


RMC NMEAMODEL::Generate_RMC(std::string input) { //сюда как аргумент передаётся функция GetLastLine("history.txt")
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
	return A;
};


void NMEAMODEL::GenerateGPSJSON() {//А сюда как аргумент - GenerateRMC()
	RMC input = Generate_RMC(GetLastLine("history.txt"));
	std::lock_guard<std::mutex> lock(JSONMutex);
	std::ofstream fout("temp.json");
	if (fout.is_open()) {
		fout << "{\n";
		fout << " \"Type\" : \"" << NMEA << "\", \n";
		fout << " \"Time\" : \"" << input.Time << "\", \n";
		fout << " \"Latitude\" : \"" << input.Latitude << "\", \n";
		fout << " \"Latitude Flag\" : \"" << input.LaFlag << "\", \n";
		fout << " \"Longitude\" : \"" << input.Longitude << "\", \n";
		fout << " \"Longitude Flag\" : \"" << input.LoFlag << "\", \n";
		fout << " \"Date\" : \"" << input.Date << "\" \n";
		fout << "}";
		fout.close();
	}
}; 

//Вспомогательные функции
//Пробежать файл до последней строки

std::string NMEAMODEL::GetLastLine(std::string filename)
{
	std::lock_guard<std::mutex> lock(NMEAMutex);
	std::ifstream inClientFile(filename, std::ios::in);
	if (!inClientFile)
	{
		exit(1);
	}
	int counter = 0;
	std::string str, buf;
	while (getline(inClientFile, buf)) {
		str = buf;
		counter++;
	};
	inClientFile.close();
	if (counter > 100) inClientFile.clear();
	return str;
}