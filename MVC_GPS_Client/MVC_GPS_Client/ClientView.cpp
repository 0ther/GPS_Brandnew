#include "pch.h"
#include "ClientView.h"


void INTERFACE::Login() {
	if (Authorisation()) Conversation();
	else GiveMessageToUser(ERR);
};

void INTERFACE::Conversation() {
	std::cout << "Клиент авторизован и передаёт геоданные" << std::endl;
	std::cout << "Чтобы выйти, введите 1" << std::endl;
	int out;
	std::cin >> out;
	if (out == 1) Quit();
	else {
		std::cout << "Некорректный ввод" << std::endl;
		Conversation();
	}
};

void INTERFACE::Quit() {
	Disconnect();
  GiveMessageToUser(LOGOUT);
  GiveMessageToUser(END);
}