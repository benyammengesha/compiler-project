#prepared by Benyam Mengesha, Archana Patel, and Syltinsy Jenkins
#include <regex>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "Parser.h"
using namespace std;

token Token;


treenode *epsilonNode = new treenode(EpsilonType, "epsilon", 0, 0);
treenode *progNode = new treenode(ProgType, "prog", 0, 0);


void parserError(token t) {
	fprintf(stdout, "Parser error, line %d, token %s \n", t.lineno, t.name);
	exit(EXIT_SUCCESS);
}

treenode *match(int tokenType) {
	treenode *temp;
	if (Token.type == tokenType) {
		temp = new treenode(Token);
		Token = scanner();
		return(temp);
	}
	else {
		parserError(Token);
		return(NULL);
	}
}

treenode *treenode::connect(int arity, treenode *c1, treenode *c2, treenode *c3, treenode *c4) {
	this->arity = arity;
	this->children[0] = c1;
	this->children[1] = c2;
	this->children[2] = c3;
	this->children[3] = c4;
	return this;
}

treenode *prog() {
	treenode *temp1, *temp2, *temp3, *temp4;
	Token = scanner();
	switch (Token.type)
	{
	case IdentifierType:
		temp1 = match(IdentifierType);
		clearmatch(LeftParType);
		temp2 = varlist();
		clearmatch(RightParType);
		clearmatch(VariableBlockType);
		temp3 = varlist();
		clearmatch(VariableBlockType);
		temp4 = statlist();
		clearmatch(EndStatementType);
		clearmatch(EOFType);
		return(progNode->connect(4, temp1, temp2, temp3, temp4));
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *varlist() {
	treenode *temp1, *temp2;
	treenode *spaceNode = new treenode(SpaceType, "space", 0, 0);
	switch (Token.type)
	{
	case IdentifierType:
		temp1 = match(IdentifierType);
		temp2 = varlist();
		return(spaceNode->connect(2, temp1, temp2, NULL, NULL));
	case RightParType:
	case VariableBlockType:
		return(epsilonNode);
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *statlist() {
	treenode *temp1, *temp2;
	treenode *statNode = new treenode(EndStatementType, "endstat", 0, 0);
	switch (Token.type)
	{
	case IdentifierType:
	case IfType:
	case WhileType:
		temp1 = statement();
		temp2 = statlist();
		return(statNode->connect(2, temp1, temp2, NULL, NULL));
	case EndStatementType:
	case ElseType:
		return(epsilonNode);
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *statement() {
	treenode *temp1;
	switch (Token.type)
	{
	case IdentifierType:
		temp1 = assign();
		return(temp1);
	case IfType:
		temp1 = ifstat();
		return(temp1);
	case WhileType:
		temp1 = whilestat();
		return(temp1);
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *assign() {
	treenode *temp1, *temp2, *temp3;
	switch (Token.type) {
	case IdentifierType:
		temp1 = match(IdentifierType);
		temp2 = match(AssignmentType);
		temp3 = expr();
		clearmatch(EndStatementType);
		return(temp2->connect(2, temp1, temp3, NULL, NULL));
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *ifstat() {
	treenode *temp1, *temp2, *temp3, *temp4;
	switch (Token.type) {
	case IfType:
		temp1 = match(IfType);
		temp2 = expr();
		clearmatch(EndConditionalType);
		temp3 = statlist();
		clearmatch(ElseType);
		temp4 = statlist();
		clearmatch(EndStatementType);
		return(temp1->connect(3, temp2, temp3, temp4, NULL));
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *whilestat() {
	treenode *temp1, *temp2, *temp3;
	switch (Token.type) {
	case WhileType:
		temp1 = match(WhileType);
		temp2 = expr();
		clearmatch(EndConditionalType);
		temp3 = statlist();
		clearmatch(EndStatementType);
		return(temp1->connect(2, temp2, temp3, NULL, NULL));
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *expr() {
	treenode *temp;
	switch (Token.type)
	{
	case IdentifierType:
	case NumberType:
	case LeftParType:
		temp = aexp();
		return(exprtail(temp));
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *exprtail(treenode *N) {
	treenode *temp1, *temp2;
	switch (Token.type)
	{
	case EqualsType:
		temp1 = match(EqualsType);
		temp2 = aexp();
		return(exprtail(temp1->connect(2, N, temp2, NULL, NULL)));
	case GreaterThanType:
		temp1 = match(GreaterThanType);
		temp2 = aexp();
		return(exprtail(temp1->connect(2, N, temp2, NULL, NULL)));
	case EndConditionalType:
	case EndStatementType:
	case RightParType:
		return(N);
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *aexp() {
	treenode *temp;
	switch (Token.type)
	{
	case IdentifierType:
	case NumberType:
	case LeftParType:
		temp = mexp();
		return(aexptail(temp));
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *aexptail(treenode *N) {
	treenode *temp1, *temp2;
	switch (Token.type)
	{
	case AddType:
		temp1 = match(AddType);
		temp2 = mexp();
		return(aexptail(temp1->connect(2, N, temp2, NULL, NULL)));
	case SubtractType:
		temp1 = match(SubtractType);
		temp2 = mexp();
		return(aexptail(temp1->connect(2, N, temp2, NULL, NULL)));
	case EqualsType:
	case GreaterThanType:
	case EndConditionalType:
	case EndStatementType:
	case RightParType:
		return(N);
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *mexp() {
	treenode *temp;
	switch (Token.type)
	{
	case IdentifierType:
	case NumberType:
	case LeftParType:
		temp = atom();
		return(mexptail(temp));
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *mexptail(treenode *N) {
	treenode *temp1, *temp2;
	switch (Token.type)
	{
	case MultiplyType:
		temp1 = match(MultiplyType);
		temp2 = atom();
		return(mexptail(temp1->connect(2, N, temp2, NULL, NULL)));
	case DivideType:
		temp1 = match(DivideType);
		temp2 = atom();
		return(mexptail(temp1->connect(2, N, temp2, NULL, NULL)));
	case AddType:
	case SubtractType:
	case EqualsType:
	case GreaterThanType:
	case EndConditionalType:
	case EndStatementType:
	case RightParType:
		return(N);
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}

treenode *atom() {
	treenode *temp;
	switch (Token.type) {
	case IdentifierType:
		temp = match(IdentifierType);
		return temp;
	case NumberType:
		temp = match(NumberType);
		return temp;
	case LeftParType:
		clearmatch(LeftParType);
		temp = expr();
		clearmatch(RightParType);
		return temp;
	default:
		if (Token.type == ErrorType) {
			scannerError(Token);
			return(NULL);
			break;
		}
		else {
			parserError(Token);
			return(NULL);
			break;
		}
	}
}
