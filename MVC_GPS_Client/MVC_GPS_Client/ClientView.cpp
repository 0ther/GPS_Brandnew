#include "pch.h"
#include "ClientView.h"


void INTERFACE::Login() {
	if (Authorisation()) Conversation();
	else GiveMessageToUser(ERR);
};

void INTERFACE::Conversation() {
	std::cout << "������ ����������� � ������� ���������" << std::endl;
	std::cout << "����� �����, ������� 1" << std::endl;
	int out;
	std::cin >> out;
	if (out == 1) Quit();
	else {
		std::cout << "������������ ����" << std::endl;
		Conversation();
	}
};

void INTERFACE::Quit() {
	Disconnect();
  GiveMessageToUser(LOGOUT);
  GiveMessageToUser(END);
}