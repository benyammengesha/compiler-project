#prepared by Benyam Mengesha, Archana Patel, and Syltinsy Jenkins



#define _CRT_SECURE_NO_WARNINGS
#include <regex>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
//#include "Parser.h"
#include "semGen.h"
using namespace std;

void printTree(treenode *t) {
	fprintf(stdout, "( %s : %d : %d ", t->name, t->type, t->arity);
	int arity = t->arity;
	if (t->children[0] != NULL && arity > 0) {
		printTree(t->children[0]);
	}
	if (t->children[1] != NULL && arity > 1) {
		printTree(t->children[1]);
	}
	if (t->children[2] != NULL && arity > 2) {
		printTree(t->children[2]);
	}
	if (t->children[3] != NULL && arity > 3) {
		printTree(t->children[3]);
	}
	fprintf(stdout, ")");
}
void main(int argc, char* argv[]) {
	if (argc < 2) {
		// Tell the user how to run the program
		std::cerr << "Usage: " << argv[0] << " <input_Simpl_file> \n"
			<< "This program takes in simple program with a .sim file extension and produces a jasmine .j file.\n"
			<< std::endl;

		//system("pause");
		//	return 1;
	}



	string in_filename = argv[1];// "tstSem.sim";
	//string out_filename  = argv[2]; //"file.txt";
	//string in_filename =  "tstSem.sim";


	size_t index_of_dot = in_filename.find_last_of(".");
	string out_filename = in_filename.substr(0, index_of_dot) + ".j";

	/*ifstream in_fs = ifstream(in_filename.c_str());
	ofstream out_fs = ofstream(out_filename.c_str());

	streambuf *cinbuf = cin.rdbuf();
	cin.rdbuf(in_fs.rdbuf());
	cout.rdbuf(out_fs.rdbuf());
	*/

	FILE *fi; // file pointer
	fi = freopen(in_filename.c_str(), "r", stdin);
	FILE *fo; // file pointer
	fo = freopen(out_filename.c_str(), "w", stdout);
	treenode *t = prog();
	//printTree(t);
	//freopen(" glob", "r", stdout);
	semanticVariables(t);
	semanticProgram(t);
	codeGenerator(t);

	//system("pause");
	//return 0;

}
