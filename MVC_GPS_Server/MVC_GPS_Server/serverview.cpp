#include "pch.h"
#include "serverview.h"

void INTERFACE::Login() {
	std::string login, password;
	std::cout << "������� �����: ";
	getline(std::cin, login);
	std::cout << "������� ������: ";
	getline(std::cin, password);
	std::cout << std::endl;
	if (CheckLogData(login, password)) {
		std::cout << "�������� ��������" << std::endl;
		if (Authorisation(login, password)) {
			std::cout << "����������� ��������, �������� ������" << std::endl;
			std::thread StartServer(&CONTROLLER::Connect, this);
			StartServer.detach();
			Console();
		}
	}
	else {
		std::cout << "������ �����������" << std::endl;
		exit(1);
	}
};


void INTERFACE::Console() { //���������� ������� ����� �������� �� ������� �������
	std::cout << "������� ��������������, ������ 1.0" << std::endl;
	std::cout << "��������� �����:" << std::endl;
	std::cout << "1. �������� ������������" << std::endl;
	std::cout << "2. ������� ������������" << std::endl;
	std::cout << "3. ������� ������ ������ �� ���� ������" << std::endl;
	std::cout << "4. ����������� ������ �� ���� ������" << std::endl;
	std::cout << "5. ��������������� ������ �� ���� ������" << std::endl;
	std::cout << "6. �������������" << std::endl;
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
	//������������� ������
	//��������� ����������
	bool finish = false;
	SwitchReceivingSocket(finish);
	WSACleanup();
	CloseConnection();
};


