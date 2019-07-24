#include "pch.h"
#include "serverview.h"

void INTERFACE::Login() {
	std::string login, password;
	std::cout << "Введите логин: ";
	getline(std::cin, login);
	std::cout << "Введите пароль: ";
	getline(std::cin, password);
	std::cout << std::endl;
	if (CheckLogData(login, password)) {
		std::cout << "Проверка пройдена" << std::endl;
		if (Authorisation(login, password)) {
			std::cout << "Авторизация пройдена, запускаю сервер" << std::endl;
			std::thread StartServer(&CONTROLLER::Connect, this);
			StartServer.detach();
			Console();
		}
	}
	else {
		std::cout << "Ошибка авторизации" << std::endl;
		exit(1);
	}
};


void INTERFACE::Console() { //реализации функций можно потянуть из старого проекта
	std::cout << "Консоль администратора, версия 1.0" << std::endl;
	std::cout << "Доступные опции:" << std::endl;
	std::cout << "1. Добавить пользователя" << std::endl;
	std::cout << "2. Удалить пользователя" << std::endl;
	std::cout << "3. Удалить старые записи из базы данных" << std::endl;
	std::cout << "4. Просмотреть записи из базы данных" << std::endl;
	std::cout << "5. Визуализировать записи из базы данных" << std::endl;
	std::cout << "6. Разлогиниться" << std::endl;
	int choice;
	std::cin >> choice;
	switch (choice) {
	case 1:
	{
		AddUser();
		Sleep(5000);
		Console();
		break;
	}
	case 2:
	{
		DeleteUser();
		Sleep(5000);
		Console();
		break;
	}
	case 3:
		Refresh();
		Sleep(5000);
		Console();
		break;
	case 4:
		Select();
		Sleep(5000);
		Console();
		break;
	case 6:
		Quit();
		break;
	default:
		exit(1);
	}
};


void INTERFACE::Quit() {
	//Останавливаем потоки
	//Разрываем соединение
	bool finish = false;
	SwitchReceivingSocket(finish);
	WSACleanup();
	CloseConnection();
};


