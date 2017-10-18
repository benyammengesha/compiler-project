#prepared by Benyam Mengesha, Archana Patel, and Syltinsy Jenkins
#include "semGen.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

symbols *symbolTable;
int numVars = 0;
int stackSize = 0;
static int varType;

symbols* lookup(char *name) {
	symbols *s;
	for (s = symbolTable; s != NULL; s = s->next) {
		if (!strcmp(name, s->name)) return s;
	}
	return NULL;
}

void insert(char *name, int type, int variableNumber) {
	symbols *s = new symbols(name, type, variableNumber);
	s->next = symbolTable;
	symbolTable = s;
}

void error(treenode *t) {
	if (t == NULL) {
		fprintf(stderr, "\t ln line no:  \"%d\"\n", t->lineno, t->name);
		fprintf(stderr, " error ");
		abort();

	}
}

int getlabel() {
	static int label = 0;
	return (++label);
}

int assignNumber() {
	static int variableNumber = 0;
	return (++variableNumber);
}
void semanticVariables(treenode *t){
	switch (t->type)
	{
	case ProgType:
		varType = OUT;
		semanticVariables(t->children[0]);
		varType = IN;
		semanticVariables(t->children[1]);
		varType = LOCAL;
		semanticVariables(t->children[2]);
		semanticVariables(t->children[3]);
		break;
	case IdentifierType:
		t->variableNumber = assignNumber();
		t->decl = (lookup(t->name) == NULL);
		numVars++;
		if (lookup(t->name) == NULL)
		{
			insert(t->name, varType, t->variableNumber);
		}
		else {
			error(t);
			fprintf(stdout, "\t  ln line no:  \"%d\"\n", t->lineno);
			fprintf(stdout, "\t ERROR REDECLARED VARIABLE  ", t->name);
			/*
			cout << endl;
			cout << "\t ERROR PRODUCTION "<< endl;
			*/
		}
		break;
	case SpaceType:
		semanticVariables(t->children[0]);
		semanticVariables(t->children[1]);

		break;
	case EpsilonType:
		break;
	}
	//Parses through the tree view for all top level variables
}

//semantic analyzer check for any error 
void semanticProgram(treenode *t)
{
	switch (t->type)
	{

	case IdentifierType:
		if (lookup(t->name) == NULL)
		{
			error(t);
			abort();
			fprintf(stderr, "\t ln line no:  \"%d\"\n", t->lineno);
			fprintf(stderr, "\t  ERROR - UNDECLARED VARIABLE NAME ", t->lineno);

		}
		else
		{
			symbols *s = lookup(t->name);
			t->variableNumber = s->variableNumber;
			stackSize++;
		}
		break;
	case NumberType:
		stackSize++;
		break;
	case AddType:
		semanticProgram(t->children[0]);
		semanticProgram(t->children[1]);
		break;
	case SubtractType:
		t->label1 = getlabel();
		semanticProgram(t->children[0]);
		semanticProgram(t->children[1]);
		break;
	case MultiplyType:
		semanticProgram(t->children[0]);
		semanticProgram(t->children[1]);
		break;
	case DivideType:
		semanticProgram(t->children[0]);
		semanticProgram(t->children[1]);
		break;
	case ProgType:
		stackSize = 0;
		semanticProgram(t->children[3]);
		break;
	case GreaterThanType:
		t->label1 = getlabel();
		semanticProgram(t->children[0]);
		t->label2 = getlabel();
		semanticProgram(t->children[1]);
		break;
	case IfType:
		t->label1 = getlabel();
		t->label2 = getlabel();
		semanticProgram(t->children[0]);
		semanticProgram(t->children[1]);
		semanticProgram(t->children[2]);
		break;
	case WhileType:
		t->label1 = getlabel();
		t->label2 = getlabel();
		semanticProgram(t->children[0]);
		semanticProgram(t->children[1]);
		break;
	case EpsilonType:
		break;
	case  AssignmentType:
		semanticProgram(t->children[0]);
		semanticProgram(t->children[1]);
		break;
	case EqualsType:
		t->label1 = getlabel();
		t->label2 = getlabel();
		semanticProgram(t->children[0]);
		semanticProgram(t->children[1]);
		break;
	case EndStatementType:
		semanticProgram(t->children[0]);
		semanticProgram(t->children[1]);
		break;

	}
}
//Parses through the tree view for all operators

void codeGenerator(treenode *t)
{
	switch (t->type)
	{
		fprintf(stdout, "inside cod gen=%d\n,t->type");
	case ProgType:
		fprintf(stdout, "; COSC 561 Simpl Compiler V1\n");
		fprintf(stdout, ".class public %s\n", t->children[0]->name);
		fprintf(stdout, ".super java/lang/Object\n");
		fprintf(stdout, ".method public static main([Ljava/lang/String;)V\n");
		codeGenerator(t->children[0]);
		codeGenerator(t->children[1]);
		codeGenerator(t->children[2]);
		codeGenerator(t->children[3]);
		fprintf(stdout, "\tiload %d\n", t->children[0]->variableNumber);
		fprintf(stdout, "\tinvokestatic simpl/outputNum(I)V\n");
		fprintf(stdout, "\treturn\n");
		fprintf(stdout, ".limit stack %d\n", stackSize + 1);
		fprintf(stdout, ".limit locals %d\n", numVars + 1);
		fprintf(stdout, ".end method\n");
		break;
	case IdentifierType:
		if (t->decl == true)
		{
			t->decl = false;
			symbols *s = lookup(t->name);
			switch (s->type)
			{

			case OUT:
				fprintf(stdout, "\tldc \"%s\"\n", t->name);
				fprintf(stdout, "\tinvokestatic simpl/registerIO(Ljava/lang/String;)V\n");
				fprintf(stdout, "\tsipush 0\n");
				fprintf(stdout, "\tistore %d\n", t->variableNumber);
				break;
			case IN:
				fprintf(stdout, "\tldc \"%s\"\n", t->name);
				fprintf(stdout, "\tinvokestatic simpl/inputNum(Ljava/lang/String;)I\n");
				fprintf(stdout, "\tistore %d\n", t->variableNumber);
				break;
			case LOCAL:
				fprintf(stdout, "\tsipush 0\n");
				fprintf(stdout, "\tistore %d\n", t->variableNumber);
				break;
			}
		}
		else {
			fprintf(stdout, "\tiload %d\n", t->variableNumber);
		}
		break;
	case SpaceType:
		codeGenerator(t->children[0]);
		codeGenerator(t->children[1]);
		break;
	case EpsilonType:
		break;
	case EndStatementType://!
		codeGenerator(t->children[0]);
		codeGenerator(t->children[1]);
		break;
	case IfType:
		codeGenerator(t->children[0]);
		fprintf(stdout, "\tifeq L%d", t->label1);
		fprintf(stdout, "\n");
		codeGenerator(t->children[1]);
		fprintf(stdout, "\tgoto L%d", t->label2); //**********************
		fprintf(stdout, "\n");
		fprintf(stdout, " L%d:", t->label1);// getlabel());
		fprintf(stdout, "\n");
		//cout << "\tgoto L" << getlabel() << endl;
		//cout << "L" << getlabel() << endl;
		codeGenerator(t->children[2]);
		fprintf(stdout, " L%d:", t->label2);// getlabel());
		fprintf(stdout, "\n");
		//cout << "L" << getlabel() << endl;
		break;
	case WhileType:
		fprintf(stdout, " L%d:", t->label1);
		fprintf(stdout, "\n");
		//cout << "L" << getlabel() << endl;
		codeGenerator(t->children[0]);
		fprintf(stdout, "\tifeq L%d", t->label2);
		fprintf(stdout, "\n");
		codeGenerator(t->children[1]);
		fprintf(stdout, "\tgoto L%d", t->label1);
		fprintf(stdout, "\n");
		fprintf(stdout, " L%d:", t->label2);
		fprintf(stdout, "\n");

		//cout << "\tgoto L" << getlabel() << endl;
		//cout << "L" << getlabel() << endl;
		break;
	case AssignmentType:
		codeGenerator(t->children[1]);
		fprintf(stdout, "\tistore %d\n", t->children[0]->variableNumber);
		break;
	case GreaterThanType:
		codeGenerator(t->children[0]);
		codeGenerator(t->children[1]);
		fprintf(stdout, "\tif_icmpgt L%d", t->label1);
		fprintf(stdout, "\n");
		fprintf(stdout, "\tsipush 0\n");
		fprintf(stdout, "\tgoto L%d", t->label2);
		fprintf(stdout, "\n");
		fprintf(stdout, " L%d:", t->label1);
		fprintf(stdout, "\n");
		//cout << "\tgoto L" << getlabel()<< endl;
		//cout << "L" << getlabel()<< endl;
		fprintf(stdout, "\tsipush 1\n");
		fprintf(stdout, " L%d:", t->label2);
		fprintf(stdout, "\n");

		//cout << "L" << getlabel() << endl;
		break;
	case EqualsType:
		codeGenerator(t->children[0]);
		codeGenerator(t->children[1]);
		fprintf(stdout, "\tif_icmpeq L%d", t->label1);
		fprintf(stdout, "\n");
		fprintf(stdout, "\tsipush 0\n");
		fprintf(stdout, "\tgoto L%d", t->label2);
		fprintf(stdout, "\n");
		fprintf(stdout, " L%d:", t->label1);
		fprintf(stdout, "\n");
		//cout << "\tgoto L" <<  getlabel() << endl;
		//cout << "L" <<  getlabel() << endl;
		fprintf(stdout, "\tsipush 1\n");
		fprintf(stdout, "\n");
		fprintf(stdout, " L%d:", t->label2);
		fprintf(stdout, "\n");
		//cout << "L" <<  getlabel() << endl;
		break;
	case AddType:
		codeGenerator(t->children[0]);
		codeGenerator(t->children[1]);
		fprintf(stdout, "\tiadd \n");
		break;
	case SubtractType:
		codeGenerator(t->children[0]);
		codeGenerator(t->children[1]);
		fprintf(stdout, "\tisub \n");
		fprintf(stdout, "\tdup \n");
		fprintf(stdout, "\tifge \n", t->label1);
		fprintf(stdout, "\tpop \n");
		fprintf(stdout, "\tsipush 0\n");
		fprintf(stdout, " L%d:", t->label1);
		fprintf(stdout, "\n");
		//cout << "L" << getlabel() << "\n";
		break;
	case MultiplyType:
		codeGenerator(t->children[0]);
		codeGenerator(t->children[1]);
		fprintf(stdout, "\timul \n");
		break;
	case DivideType:
		codeGenerator(t->children[0]);
		codeGenerator(t->children[1]);
		fprintf(stdout, "\tidiv \n");
		break;
	case NumberType:
		fprintf(stdout, "\tsipush %s\n", t->name);
		break;
	}
}
