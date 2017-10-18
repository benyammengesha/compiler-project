#prepared by Benyam Mengesha, Archana Patel, and Syltinsy Jenkins
#include "scanner.h"
#include <string>
using namespace std;

#define EpsilonType 75
#define ProgType 76
#define clearmatch(x) (delete match(x))

struct treenode : token {
	int arity;
	bool decl;
	int variableNumber;
	int label1;
	int label2;
	treenode *children[4];
	treenode(int type, char* name, int lineno, int arity) {
		this->type = type;
		this->name = name;
		this->lineno = lineno;
		this->arity = arity;
	}
	treenode(token t) : treenode(t.type, t.name, t.lineno, 0) {}

	treenode *connect(int arity, treenode *c1, treenode *c2, treenode *c3, treenode *c4);
};


treenode *prog();
treenode *varlist();
treenode *statlist();
treenode *statement();
treenode *assign();
treenode *ifstat();
treenode *whilestat();
treenode *expr();
treenode *exprtail(treenode *N);
treenode *aexp();
treenode *aexptail(treenode *N);
treenode *mexp();
treenode *mexptail(treenode *N);
treenode *atom();
