#include "Opcode.h"

/*
* This c'tor checks to which opcode the Node match and run that opcode.
* Input:
* opcode - the opcode Node.
* mem - the memory area to run the opcode.
*/
Opcode::Opcode(AstNode* opcode, MemStore* mem) {
	this->_opcode = opcode;
	this->_mem = mem;

	//check to which opcode the Node match
	switch (Utilities::getOpcode(opcode->getData())) {
	case Opcodes::Mov:
		Mov();
		break;
	case Opcodes::Sub:
		Sub();
		break;
	case Opcodes::Add:
		Add();
		break;
	case Opcodes::Mul:
		Mul();
		break;
	case Opcodes::Div:
		Div();
		break;
	case Opcodes::Inc:
		Inc();
		break;
	case Opcodes::Dec:
		Dec();
		break;
	case Opcodes::Or:
		Or();
		break;
	case Opcodes::And:
		And();
		break;
	case Opcodes::Xor:
		Xor();
		break;
	case Opcodes::Not:
		Not();
		break;
	case Opcodes::Nop:
		Nop();
		break;
	case Opcodes::Shl:
		Shl();
		break;
	case Opcodes::Shr:
		Shr();
		break;
	case Opcodes::Rol:
		Rol();
		break;
	case Opcodes::Ror:
		Ror();
		break;
	case Opcodes::Pop:
		Pop();
		break;
	case Opcodes::Push:
		Push();
		break;

	default:
		cerr << "ERROR";
		break;
	}
}

/* MOV opcode */
void Opcode::Mov() {
	//check if the user enter the correct syntax of the opcode
	if(Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);		
		if (value->handler() == 0)
			_mem->_flags.ZF = 1;
		_mem->setRegister(Utilities::getParam(_opcode, 0), value->handler());
	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* ADD opcode */
void Opcode::Add() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getRegister(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		_mem->setRegister(Utilities::getParam(_opcode, 0), old_value + value->handler());

	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* SUB opcode */
void Opcode::Sub() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getRegister(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		if (old_value - value->handler() == 0)
			_mem->_flags.ZF = 1;
		_mem->setRegister(Utilities::getParam(_opcode, 0), old_value - value->handler());

	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* MUL opcode */
void Opcode::Mul() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		old_value = _mem->getRegister(EAX);
		ValuesHandler* value = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);
		if (value->handler() == 0)
			_mem->_flags.ZF = 1;
		_mem->setRegister(EAX, old_value * value->handler());

	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* DIV opcode */
void Opcode::Div() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		old_value = _mem->getRegister(EAX);
		ValuesHandler* value = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);
		if (old_value % value->handler() != 0)
			_mem->setRegister(EDX, old_value % value->handler());
		_mem->setRegister(EAX, old_value / value->handler());

	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* INC opcode */
void Opcode::Inc() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {		
		_mem->setRegister(_opcode->getBranches()[0]->getData(), _mem->getRegister(_opcode->getBranches()[0]->getData()) + 1);
	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* DEC opcode */
void Opcode::Dec() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		if (_mem->getRegister(_opcode->getBranches()[0]->getData()) - 1 == 0)
			_mem->_flags.ZF = 1;
		_mem->setRegister(_opcode->getBranches()[0]->getData(), _mem->getRegister(_opcode->getBranches()[0]->getData()) - 1);
	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* OR opcode */
void Opcode::Or(){
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getRegister(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		_mem->setRegister(Utilities::getParam(_opcode, 0), old_value | value->handler());

	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* AND opcode */
void Opcode::And() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getRegister(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		if ((old_value & value->handler()) == 0)
			_mem->_flags.ZF = 1;
		_mem->setRegister(Utilities::getParam(_opcode, 0), old_value & value->handler());

	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* XOR opcode */
void Opcode::Xor() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getRegister(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		if ((old_value ^ value->handler()) == 0)
			_mem->_flags.ZF = 1;
		_mem->setRegister(Utilities::getParam(_opcode, 0), old_value ^ value->handler());

	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* NOT opcode */
void Opcode::Not() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		_mem->setRegister(_opcode->getBranches()[0]->getData(), ~_mem->getRegister(_opcode->getBranches()[0]->getData()));
	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* NOP opcode */
void Opcode::Nop() {

	//check if the user enter the correct syntax of the opcode
	if (!Utilities::validOperators(_opcode, 0))
		throw SyntaxError("Opcode Error - opcode syntax not valid.");	
}

/* SHL opcode */
void Opcode::Shl() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getRegister(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		if ((old_value << value->handler()) == 0)
			_mem->_flags.ZF = 1;
		_mem->setRegister(Utilities::getParam(_opcode, 0), old_value << value->handler());

	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* SHR opcode */
void Opcode::Shr() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getRegister(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		if ((old_value >> value->handler()) == 0)
			_mem->_flags.ZF = 1;
		_mem->setRegister(Utilities::getParam(_opcode, 0), old_value >> value->handler());

	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* ROR opcode */
void Opcode::Rol() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getRegister(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		if ((old_value << value->handler()) == 0)
			_mem->_flags.ZF = 1;
		_mem->setRegister(Utilities::getParam(_opcode, 0), old_value << value->handler());

	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

/* ROR opcode */
void Opcode::Ror() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getRegister(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		if ((old_value >> value->handler()) == 0)
			_mem->_flags.ZF = 1;
		_mem->setRegister(Utilities::getParam(_opcode, 0), old_value >> value->handler());

	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

void Opcode::Push() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* value = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);
		_mem->push(value->handler());
	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}

void Opcode::Pop() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		_mem->setRegister(_opcode->getBranches()[0]->getData(), _mem->pop());
	}
	else {
		throw SyntaxError("Opcode Error - opcode syntax not valid.");
	}
}
