#pragma once

#include <mutex>
std::mutex NMEAMutex; //�������� �� ������ � ����� � NMEA-�������� (history.txt)
std::mutex JSONMutex; //�������� �� ������ � ����� temp.json, ���� ������������ ��� json-�
