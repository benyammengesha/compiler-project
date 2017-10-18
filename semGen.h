#include "Parser.h"
#include <string>
using namespace std;

#define IN 0
#define OUT 1
#define LOCAL 2

struct symbols {
	char* name;
	int type;
	int variableNumber;
	symbols *next;
	symbols(char *name, int type, int variableNumber) { this->name = name; this->type = type; this->variableNumber = variableNumber; }
};

symbols* lookup(char *name);
void insert(char *name, int type, int variableNumber);
void semanticVariables(treenode *t);
void semanticProgram(treenode *t);
void codeGenerator(treenode *t);