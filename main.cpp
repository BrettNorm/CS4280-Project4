/*
*** Author: Brett Bax
*** Assignment: Project 3
*** Class: CS4280
***
*** main.cpp is the main driver for Project 3.
*/


#include "parser.h"
#include "makeTree.h"
#include "statSem.h"

using namespace std;

int main(int argc, char *argv[]) {
	
	ifstream inFile;
	string file;

	if(argc == 2){
		file = argv[1];
		file.append(".sp2022");

	} else if(argc > 2){ 
		cout << "main.cpp: Too many arguments. 2 or less, please.\n";
		cout << "USAGE: 	   './statSem'	(hit enter, then keyboard input to userInputFile),\n";
		cout << "	or './statSem [fileName]'	(.sp2022 extension added automatically),\n";
		cout << "  	or './statSem < [fileName.ext]'\n";
		return 1;
	}
	cout << "\nOpening file stream...\n\n";
	inFile.open(file.c_str());

	node *parseTree = parser(inFile);
	semCheck(parseTree, 0);

	inFile.close();
	cout << "\nFile stream closed. Bye!\n\n";
		
	return 0;
}
