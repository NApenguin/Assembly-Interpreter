#include "Opcode.h"

/*
* This c'tor init the opcode settings;
* Input:
* opcode - the opcode Node.
* mem - the memory area to run the opcode.
*/
Opcode::Opcode(AstNode* opcode, MemStore* mem) {
	this->_opcode = opcode;
	this->_mem = mem;
}

/*
* This function checks to which opcode the Node match and run that opcode.
* Output: NULL.
*/
void Opcode::run()
{
	//check to which opcode the Node match
	switch (Utilities::getOpcode(_opcode->getData())) {
	case Opcodes::MOV:
		MOV();
		break;
	case Opcodes::SUB:
		SUB();
		break;
	case Opcodes::ADD:
		ADD();
		break;
	case Opcodes::MUL:
		MUL();
		break;
	case Opcodes::DIV:
		DIV();
		break;
	case Opcodes::INC:
		INC();
		break;
	case Opcodes::DEC:
		DEC();
		break;
	case Opcodes::OR:
		OR();
		break;
	case Opcodes::AND:
		AND();
		break;
	case Opcodes::XOR:
		XOR();
		break;
	case Opcodes::NOT:
		NOT();
		break;
	case Opcodes::NOP:
		NOP();
		break;
	case Opcodes::SHL:
		SHL();
		break;
	case Opcodes::SHR:
		SHR();
		break;
	case Opcodes::ROL:
		ROL();
		break;
	case Opcodes::ROR:
		ROR();
		break;
	case Opcodes::POP:
		POP();
		break;
	case Opcodes::PUSH:
		PUSH();
		break;
	case Opcodes::CMP:
		CMP();
		break;
	case Opcodes::JNZ:
		JNZ();
		break;
	case Opcodes::JZ:
		JZ();
		break;
	case Opcodes::JNE:
		JNE();
		break;
	case Opcodes::JE:
		JE();
		break;
	case Opcodes::JNS:
		JNS();
		break;
	case Opcodes::JS:
		JS();
		break;
	case Opcodes::JNO:
		JNO();
		break;
	case Opcodes::JO:
		JO();
		break;
	case Opcodes::JNP:
		JNP();
		break;
	case Opcodes::JP:
		JP();
		break;	
	case Opcodes::JAE:
		JAE();
		break;
	case Opcodes::JBE:
		JBE();
		break;
	case Opcodes::CLI:
		CLI();
		break;
	case Opcodes::STI:
		STI();
		break;
	case Opcodes::LOOP:
		LOOP();
		break;
	case Opcodes::XCHG:
		XCHG();
		break;
	case Opcodes::INT:
		INT();
		break;

	default:
		//check if the user enter correct lable syntax
		if (_opcode->getBranches().size() == 1 && _opcode->getBranches()[0]->getBranches().size() == 0) {
			LABLE();
		}
		//check if the user enter correct lable syntax
		else if (_opcode->getBranches().size() == 1 && _opcode->getBranches()[0]->getBranches().size() == 1) {
			VARS();
		}
		break;
	}
}

/* LABLES */
void Opcode::LABLE() {
	//check if the lable syntax is valid
	if (_opcode->getBranches()[0]->getData() == ":")
		//add new lable
		_mem->addIdentifier(_opcode->getData(), _mem->getRegister(EIP));
	else
		throw SyntaxError("LableError - Invalid syntax");
}

/* VARS */
void Opcode::VARS() {
	//check if the var syntax is valid
	if (_opcode->getBranches()[0]->getBranches()[0]->getBranches().size() == 0) {
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 0), _mem);
		_mem->addVar(_opcode->getData(), value->handler(false), Utilities::getSignSize(_opcode->getBranches()[0]->getData()));
	}
	else {
		throw SyntaxError("VarError - Invalid syntax");
	}
}

/* MOV opcode */
void Opcode::MOV() {
	//check if the user enter the correct syntax of the opcode
	if(Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		_mem->checkSize(Utilities::getParam(_opcode, 0), Utilities::getParam(_opcode, 1));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);		
		_mem->editValue(Utilities::getParam(_opcode, 0), value->handler(false));
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* ADD opcode */
void Opcode::ADD() {
	unsigned int old_value = 0, num = 0;
	unsigned int size = 0, max = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		_mem->checkSize(Utilities::getParam(_opcode, 0), Utilities::getParam(_opcode, 1));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		old_value = _mem->getValue(Utilities::getParam(_opcode, 0));
		size = _mem->getValueSize(Utilities::getParam(_opcode, 0));
		num = value->handler(false) + old_value;
		max = static_cast<uint64_t>(pow(2, 8 * size)) - 1;
		//check if the value is out of range
		if ((max < num && (value->handler(false) - 1) < max) || num == 0) {
			num = max & num;
			_mem->_flags.CF = true;
		}
		else {
			_mem->_flags.CF = false;
		}
		_mem->editValue(Utilities::getParam(_opcode, 0), num);

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* SUB opcode */
void Opcode::SUB() {
	unsigned int old_value = 0, num = 0;
	unsigned int size, max;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		_mem->checkSize(Utilities::getParam(_opcode, 0), Utilities::getParam(_opcode, 1));
		old_value = _mem->getRegister(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		size = _mem->getValueSize(Utilities::getParam(_opcode, 0));
		num = old_value - value->handler(false);
		max = static_cast<uint64_t>(pow(2, 8 * size)) - 1;

		//check if the value is out of range
		if (max < num || num == max) {
			num = max & num;
			_mem->_flags.CF = true;
		}
		else {
			_mem->_flags.CF = false;
		}

		_mem->editValue(Utilities::getParam(_opcode, 0), num);

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* MUL opcode */
void Opcode::MUL() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		old_value = _mem->getRegister(EAX);
		ValuesHandler* value = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);
		_mem->editValue(EAX, old_value * value->handler(false));

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* DIV opcode */
void Opcode::DIV() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		old_value = _mem->getRegister(EAX);
		ValuesHandler* value = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the division is valid
		if (value->handler(false) == 0)
			Interrupts::INT_0(_mem);

		if (old_value % value->handler(false) != 0)
			_mem->setRegister(EDX, old_value % value->handler(false));
		_mem->setRegister(EAX, old_value / value->handler(false));

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* INC opcode */
void Opcode::INC() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {		
		_mem->editValue(_opcode->getBranches()[0]->getData(), _mem->getValue(_opcode->getBranches()[0]->getData()) + 1);
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* DEC opcode */
void Opcode::DEC() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		_mem->editValue(_opcode->getBranches()[0]->getData(), _mem->getValue(_opcode->getBranches()[0]->getData()) - 1);
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* OR opcode */
void Opcode::OR(){
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		_mem->checkSize(Utilities::getParam(_opcode, 0), Utilities::getParam(_opcode, 1));
		old_value = _mem->getValue(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		_mem->editValue(Utilities::getParam(_opcode, 0), old_value | value->handler(false));

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* AND opcode */
void Opcode::AND() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		_mem->checkSize(Utilities::getParam(_opcode, 0), Utilities::getParam(_opcode, 1));
		old_value = _mem->getValue(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		_mem->editValue(Utilities::getParam(_opcode, 0), old_value & value->handler(false));

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* XOR opcode */
void Opcode::XOR() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		_mem->checkSize(Utilities::getParam(_opcode, 0), Utilities::getParam(_opcode, 1));
		old_value = _mem->getValue(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		_mem->editValue(Utilities::getParam(_opcode, 0), old_value ^ value->handler(false));

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* NOT opcode */
void Opcode::NOT() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		_mem->editValue(_opcode->getBranches()[0]->getData(), ~_mem->getValue(_opcode->getBranches()[0]->getData()));
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* NOP opcode */
void Opcode::NOP() {

	//check if the user enter the correct syntax of the opcode
	if (!Utilities::validOperators(_opcode, 0))
		throw SyntaxError("OpcodeError - opcode syntax not valid.");	
}

/* SHL opcode */
void Opcode::SHL() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getValue(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		_mem->editValue(Utilities::getParam(_opcode, 0), old_value << value->handler(false));

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* SHR opcode */
void Opcode::SHR() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getValue(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		_mem->editValue(Utilities::getParam(_opcode, 0), old_value >> value->handler(false));

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* ROR opcode */
void Opcode::ROL() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getValue(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		_mem->editValue(Utilities::getParam(_opcode, 0), old_value << value->handler(false));

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* ROR opcode */
void Opcode::ROR() {
	int old_value = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		old_value = _mem->getValue(Utilities::getParam(_opcode, 0));
		ValuesHandler* value = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		_mem->editValue(Utilities::getParam(_opcode, 0), old_value >> value->handler(false));

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* PUSH opcode */
void Opcode::PUSH() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* value = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);
		_mem->push(value->handler(false));
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* POP opcode */
void Opcode::POP() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		_mem->editValue(_opcode->getBranches()[0]->getData(), _mem->pop());
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* CMP opcode */
void Opcode::CMP() {
	int value_1 = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		_mem->checkSize(Utilities::getParam(_opcode, 0), Utilities::getParam(_opcode, 1));
		value_1 = _mem->getValue(Utilities::getParam(_opcode, 0));
		ValuesHandler* value_2 = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);

		//check if the numbers are equal
		if (value_1 == value_2->handler(false)) {
			_mem->_flags.ZF = true;
		}
		else{
			_mem->_flags.ZF = false;
		}
		//check if the numbers are equal
		if (value_1 < value_2->handler(false)) {
			_mem->_flags.SF = true;
		}
		else {
			_mem->_flags.SF = false;
		}

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JNZ opcode */
void Opcode::JNZ() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag ZF is 0
		if (!_mem->_flags.ZF){
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JZ opcode */
void Opcode::JZ() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag ZF is 1
		if (_mem->_flags.ZF) {
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JE opcode */
void Opcode::JE() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag ZF is 1
		if (_mem->_flags.ZF) {
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JNE opcode */
void Opcode::JNE() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag ZF is 0
		if (!_mem->_flags.ZF) {
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JS opcode */
void Opcode::JS() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag SF is 1
		if (_mem->_flags.SF) {
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JNS opcode */
void Opcode::JNS() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag SF is 0
		if (!_mem->_flags.SF) {
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JO opcode */
void Opcode::JO() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag OF is 1
		if (_mem->_flags.OF) {
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JNO opcode */
void Opcode::JNO() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag OF is 0
		if (!_mem->_flags.OF) {
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JP opcode */
void Opcode::JP() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag PF is 1
		if (_mem->_flags.PF) {
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JNP opcode */
void Opcode::JNP() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag PF is 0
		if (!_mem->_flags.PF) {
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JAE opcode */
void Opcode::JAE() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag PF is 0 or the ZF set to 1
		if (!_mem->_flags.SF || _mem->_flags.ZF) {
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* JBE opcode */
void Opcode::JBE() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the flag PF is 1 or the ZF set to 1
		if (_mem->_flags.SF || _mem->_flags.ZF) {
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* CLI opcode */
void Opcode::CLI() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 0)) {

		//set the IF flag to 0
		_mem->_flags.IF = false;
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}	
}

void Opcode::STI() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 0)) {

		//set the IF flag to 1
		_mem->_flags.IF = true;
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* LOOP opcode */
void Opcode::LOOP() {

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);

		//check if the ECX register equals to 0
		if (_mem->getRegister(ECX) != 0) {
			_mem->setRegister(ECX, _mem->getRegister(ECX) - 1);
			_mem->jmp(place->handler(true));
		}
	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* XCHG opcode */
void Opcode::XCHG() {
	unsigned int tmp = 0;

	//check if the user enter the correct syntax of the opcode
	if (Utilities::validOperators(_opcode, 1) && Utilities::validparams(_opcode, 2)) {
		//check if the registers has the same size
		_mem->checkSize(Utilities::getParam(_opcode, 0), Utilities::getParam(_opcode, 1));
		ValuesHandler* reg_1 = new ValuesHandler(Utilities::getParam(_opcode, 0), _mem);
		ValuesHandler* reg_2 = new ValuesHandler(Utilities::getParam(_opcode, 1), _mem);
		tmp = reg_1->handler(false);
		_mem->setRegister(Utilities::getParam(_opcode, 0), reg_2->handler(false));
		_mem->setRegister(Utilities::getParam(_opcode, 1), tmp);

	}
	else {
		throw SyntaxError("OpcodeError - opcode syntax not valid.");
	}
}

/* Main Interrupts Syntax */
void Opcode::INT() {

	//check if the user enter the correct syntax of the interrupts
	if (Utilities::validOperators(_opcode, 1)) {
		ValuesHandler* place = new ValuesHandler(_opcode->getBranches()[0]->getData(), _mem);
		Interrupts* interrupts = new Interrupts(_mem);

		interrupts->interruptsHandler(place->handler(false));
	}
	else {
		throw SyntaxError("IntError - Interrupt syntax not valid.");
	}
}
