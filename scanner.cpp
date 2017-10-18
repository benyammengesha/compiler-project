#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "scanner.h"
using namespace std;

int line = 1;

bool letter(char a1) {

	return(a1 >= 'a' && a1 <= 'z');
}

bool number(char a1) {
	return (a1 >= '0' && a1 <= '9');
}

void scannerError(token t) {
	fprintf(stderr, "Scanner error, line %d, token %s \n", t.lineno, t.name);
	abort();
}

token scanner() {
	char x;
	token r;
	int size, i;
	int state;
	static token RW[] = { { IfType, 0, "if" }, { WhileType, 0, "while" } };
	state = Start;
	while (true) {
		switch (state) {
		case Start:
			switch (x = getc(stdin)) {
			case '!':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = EndStatement;
				break;

			case '?':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0'; state = EndConditional;
				break;

			case '|':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = VariableBlock;
				break;

			case '<':
				r.name = (char*)malloc(size = 1);
				*(r.name) = x;
				state = Assignment;
				break;

			case '=':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = Equals;
				break;

			case '*':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = Multiply;
				break;

			case '/':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = Divide;
				break;

			case '+':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = Add;
				break;

			case '-':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = Subtract;
				break;

			case EOF:
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = Eof;
				break;

			case '(':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = LeftPar;
				break;

			case ')':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = RightPar;
				break;

			case ':':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = Else;
				break;

			case '>':
				r.name = (char*)malloc(size = 2);
				*(r.name) = x;
				*(r.name + 1) = '\0';
				state = GreaterThan;
				break;

			default:
				if (x == '\n') ++line;
				if (number(x)) {
					r.name = (char*)malloc(size = 1);
					*(r.name) = x;
					state = Number;
					break;
				}
				else if (letter(x)) {
					r.name = (char*)malloc(size = 1);
					*(r.name) = x;
					state = Identifier;
					break;
				}
				else if (x > ' ') {
					r.name = (char*)malloc(size = 2);
					*(r.name) = x;
					*(r.name + 1) = '\0';
					state = Error;
				}
				else state = Start;
				break;
			}
			break;

		case EndStatement: r.type = EndStatementType; r.lineno = line; return(r);

		case EndConditional: r.type = EndConditionalType; r.lineno = line; return(r);

		case VariableBlock: r.type = VariableBlockType; r.lineno = line; return(r);

		case Assignment:
			x = getc(stdin);
			if (x == '-') {
				r.name = (char*)realloc(r.name, 3);
				*(r.name + 1) = x;
				*(r.name + 2) = '\0';
				r.type = AssignmentType;
				r.lineno = line;
				return(r);
			}
			else {
				ungetc(x, stdin);
				r.name = (char*)realloc(r.name, 2);
				*(r.name + 1) = '\0';
				state = Error;
				break;
			}
			break;

		case Equals: r.type = EqualsType; r.lineno = line; return(r);

		case Multiply: r.type = MultiplyType; r.lineno = line; return(r);

		case Divide: r.type = DivideType; r.lineno = line; return(r);

		case Add: r.type = AddType; r.lineno = line; return (r);

		case Subtract: r.type = SubtractType; r.lineno = line; return (r);

		case Number:
			if (number(x = getc(stdin))) {
				r.name = (char*)realloc(r.name, ++size);
				*(r.name + size - 1) = x;
				state = Number;
			}
			else {
				r.name = (char*)realloc(r.name, ++size);
				*(r.name + size - 1) = '\0';
				ungetc(x, stdin);
				r.type = NumberType;
				r.lineno = line;
				return(r);
			}
			break;

		case Identifier:

			if (letter(x = getc(stdin)) || number(x) || x == '_') {
				r.name = (char*)realloc(r.name, ++size);
				*(r.name + size - 1) = x;
				state = Identifier;
			}
			else {
				r.name = (char*)realloc(r.name, ++size);
				*(r.name + size - 1) = '\0';
				ungetc(x, stdin);
				r.type = IdentifierType;
				r.lineno = line;
				for (i = 0; i < 2; i++) {
					if (!strcmp(r.name, RW[i].name)) {
						r.type = RW[i].type;
					}
				}
				return(r);
			}
			break;

		case Eof: r.type = EOFType; r.lineno = line; return (r);

		case LeftPar: r.type = LeftParType; r.lineno = line; return (r);

		case RightPar: r.type = RightParType; r.lineno = line; return (r);

		case Else: r.type = ElseType; r.lineno = line; return (r);

		case GreaterThan: r.type = GreaterThanType; r.lineno = line; return (r);

		case Error: r.type = ErrorType; r.lineno = line; return (r);
		}
	}
}

