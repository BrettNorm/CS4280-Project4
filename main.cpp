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
	ofstream outFile;
	ofstream tFile;
	string file;
	string outputFile;
	string input;

	if(argc == 2){
		file = argv[1];
		outputFile = argv[1];
		outputFile.append(".asm");
		file.append(".sp2022");

	} else if (argc == 1) {
		outputFile = "kb.asm";
		file = "tempFile.sp2022";
		tFile.open(file.c_str());

		while (getline(cin, input)) {
			tFile << input << "\n";
		}

		tFile.close();

	} else if(argc > 2){ 
		cout << "main.cpp: Too many arguments. 2 or less, please.\n";
		cout << "USAGE: 	   './compfs'	(hit enter, then keyboard input to userInputFile),\n";
		cout << "	or './compfs [fileName]'	(.sp2022 extension added automatically),\n";
		return 1;
	}
	inFile.open(file.c_str());
	outFile.open(outputFile.c_str());

	node *parseTree = parser(inFile);
	semCheck(parseTree, 0, outFile);

	cout << outputFile << "\n";

	inFile.close();
	outFile.close();

		
	return 0;
}
