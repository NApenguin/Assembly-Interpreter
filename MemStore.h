#pragma once
#include "Pch.h"
#include "Utilities.h"
#include "RegisterError.h"

/*
* This class managed all the Interpreter memory, such as Registers etc...
*/
class MemStore {
public:
	MemStore();
	void setRegister(string reg, int value);
	int getRegister(string reg);
	bool isRegister(string reg);

	void printMemory();
private:
	map<string, int> _registers;
};