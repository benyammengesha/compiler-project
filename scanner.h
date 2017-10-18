#prepared by Benyam Mengesha, Archana Patel, and Syltinsy Jenkins
#include <string>
using namespace std;

#define StartType 33
#define SpaceType 34
#define EndStatementType 35
#define EndConditionalType 36
#define VariableBlockType 37
#define AssignmentType 38
#define EqualsType 39
#define MultiplyType 40
#define DivideType 41
#define AddType 42
#define SubtractType 43
#define NumberType 44
#define IdentifierType 45
#define WhileType 46
#define IfType 47
#define EOFType 48
#define LeftParType 49
#define RightParType 50
#define ElseType 51
#define GreaterThanType 52
#define ErrorType 53

#define Start 54
#define Space 55
#define EndStatement 56
#define EndConditional 57
#define VariableBlock 58
#define Assignment 59
#define Equals 60
#define Multiply 61
#define Divide 62
#define Add 63
#define Subtract 64
#define Number 65
#define Identifier 66
#define While 67
#define If 68
#define Eof 69
#define LeftPar 70
#define RightPar 71
#define Else 72
#define GreaterThan 73
#define Error 74

struct token {
	int type;
	int lineno;
	char *name;
};

token scanner();
void scannerError(token t);
