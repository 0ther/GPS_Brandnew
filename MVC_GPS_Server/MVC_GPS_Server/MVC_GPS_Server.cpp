#include "pch.h"
#include <iostream>
#include "serverview.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	ClearLog();
	WriteIntoLog("Запуск программы");
	INTERFACE INT1;
	INT1.Login();
	WriteIntoLog("Закрытие программы");
	CloseLog();
	system("pause");
	return 0;
}

