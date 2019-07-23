#pragma once

#include <mutex>
std::mutex NMEAMutex; //отвечает за доступ к файлу с NMEA-строками (history.txt)
std::mutex JSONMutex; //отвечает за доступ к файлу temp.json, куда записываются все json-ы
