#pragma once
#include "Pch.h"
#include "Utilities.h"
#include "RegisterError.h"
#include "StackError.h"
#include "Opcode.h"

class Opcode;

//Flags register settings
struct Flags {
	bool CF;
	bool ZF;
	bool SF;
	bool OF;
	bool PF;
	bool AF;
	bool IF;
	bool DF;
};

/*
* This class managed all the Interpreter memory, such as Registers etc...
*/
class MemStore {
public:
	MemStore();
	void setRegister(string reg, unsigned int value);
	unsigned int getRegister(string reg);
	int getRegisterSize(string reg);
	bool isRegister(string reg);
	void push(unsigned int value);
	unsigned int pop();
	void cleanFlags();
	
	void addToHistory(Opcode* opcode, string line);
	void removeFromHistory();
	Opcode* getFromHistory(unsigned int place);
	void incEIP();
	void editValue(string name, unsigned int value);
	unsigned int getValue(string name);
	int getValueSize(string name);
	void checkSize(string name_1, string name_2);

	void printMemory();
	void printHistory();
	void jmp(unsigned int place);

	void addIdentifier(string name, unsigned int value);
	unsigned int getIdentifier(string name);
	bool isID(string name);

	void addVar(string name, unsigned int value, int size);
	tuple<int, unsigned int> getVar(string name);
	void setVar(string name, unsigned int value);
	int getVarSize(string name);

	bool isVar(string name);

	struct Flags _flags;

private:
	map<tuple<string, string, string, string>, unsigned int> _registers;
	vector<unsigned int> _stack;
	vector<tuple<Opcode*, string>> _history;
	map<string, unsigned int> _identifiers;
	map<string, tuple<int, unsigned int>> _variables;

};
