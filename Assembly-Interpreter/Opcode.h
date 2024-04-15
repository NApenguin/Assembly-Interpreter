#pragma once
#include "Utilities.h"
#include "Pch.h"
#include "AstNode.h"
#include "SyntaxError.h"
#include "MemStore.h"
#include "ValuesHandler.h"

class Opcode
{
public:
	Opcode(AstNode* opcode, MemStore* mem);

private:
	AstNode* _opcode;
	MemStore* _mem;

	void Mov();
	void Add();
	void Sub();
	void Mul();
	void Div();
	void Inc();
	void Dec();
	void Or();
	void And();
	void Xor();
	void Not();
	void Nop();
	void Shl();
	void Shr();
	void Rol();
	void Ror();
};
