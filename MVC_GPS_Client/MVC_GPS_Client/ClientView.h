#pragma once
#include <string>
#include <iostream>
#include "ClientController.h"
//#include "Locks.h"

class INTERFACE : public CONTROLLER {
public:
  INTERFACE() : CONTROLLER() {};
  INTERFACE(float a, float b) : CONTROLLER(a, b) {};
	void Login();
	void Conversation();
	void Quit();
};