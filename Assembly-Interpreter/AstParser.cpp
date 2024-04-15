#include "AstParser.h"

AstParser::AstParser(queue<string> tokens) {
	int param = 0;
	int i = 0;
	AstNode* tmp = nullptr;

	if (Utilities::isOpcode(tokens.front())) {
		AstNode* node = new AstNode(tokens.front(), OPCODE);
		this->_branches.push_back(node);
		tokens.pop();
	}
	else {
		throw SyntaxError("Parser ERROR - Opcode not found");
	}
	
	while (!tokens.empty()) {

		if (Utilities::isOperator(tokens.front()[0])) {
			AstNode* node = new AstNode(tokens.front(), OPERATOR);
			this->_branches[0]->add(node);
		}
		else {
			AstNode* node = new AstNode(tokens.front(), TOKEN);
			if (this->_branches[0]->getBranches().size() > 0) {
				if (Utilities::isOperator(this->_branches[0]->getBranches()[0]->getData()[0])) {					
					if (tmp) {
						this->_branches[0]->getBranches()[0]->add(tmp);
						tmp = nullptr;
					}
					this->_branches[0]->getBranches()[0]->add(node);
					param = 0;
				}
				else {
					throw SyntaxError("Parser ERROR - Operator not found");
				}
			}
			else {
				if (param != 0) {
					throw SyntaxError("Parser ERROR - Operator not found");
				}
				tmp = node;
				param++;
			}
		}

		tokens.pop();
	}
}

std::vector<AstNode*>AstParser::getBranches() {
	return this->_branches;
}

void AstParser::printTree() {
	int i = 0;
	for (i = 0; i < _branches[0]->getBranches()[0]->getBranches().size(); i++) {
		cout << _branches[0]->getBranches()[0]->getBranches()[i]->getData() << "#";
	}
	cout << std::endl;
}