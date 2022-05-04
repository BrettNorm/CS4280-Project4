
/* testScanner to...test scanner */
#include "scanner.h"

string tokenNames[] = {
	"EOF             ",
	"Error           ",
	"Identifier      ",
	"Integer         ",
	"Start Keyword   ",
	"Stop Keyword    ",
	"Repeat Keyword  ",
	"Until Keyword   ",
	"Whole Keyword   ",
	"Label Keyword   ",
	"Quit Keyword    ",
	"Listen Keyword  ",
	"Yell Keyword    ",
	"Main Keyword    ",
	"Portal Keyword  ",
	"If Keyword      ",
	"Then Keyword    ",
	"Pick Keyword    ",
	"Declare Keyword ",
	"Assign Keyword  ",
	"Proc Keyword    ",

	"Equals          ",
	"Greater/Equal To",
	"Less Or Equal To",
	"Equals Equals   ",
	"Not Equal To    ",
	"Colon           ",
	"Colon Equals    ",
	"Plus            ",
	"Minus           ",
	"Asterisk        ",
	"Forward Slash   ",
	"Modulus         ",
	"Period          ",
	"Open Parenthesis",
	"CloseParenthesis",
	"Comma           ",
	"Open Curly      ",
	"Close Curly     ",
	"Semicolon       ",
	"Open Bracket    ",
	"Close Bracket   "
};


void testScanner(ifstream& ifile){
	token testScanner_tk;
	int lineNumber = 1;
	int spaceNeeded = 0;

	while(ifile) {
		testScanner_tk = scanner(ifile, lineNumber);
		/* error checking */
		if(isalpha(testScanner_tk.token_Instance[0]) && tokenNames[testScanner_tk.ID_tk] == "Identifier") {
			if(toupper(testScanner_tk.token_Instance[0] == testScanner_tk.token_Instance[0])) {
				cout << "testScanner.cpp: Bad identifier " << testScanner_tk.token_Instance << ", line number" << testScanner_tk.line << "\n";
				cout << "You can't start identifiers with a capital letter\n";
				return;
			}
			
		} else if(tokenNames[testScanner_tk.ID_tk] == "Error") {
			return;
		} else {
				cout << "{tokenID: " << tokenNames[testScanner_tk.ID_tk] << "    tokenInstance: " 
			    << testScanner_tk.token_Instance;
				/* setting all instance output to 8 for cleaner output */
				if (testScanner_tk.token_Instance.length() < 8) {
					spaceNeeded = 8 - testScanner_tk.token_Instance.length();
					for (int i = 1; i < spaceNeeded; i++) {
						cout << " ";
					}
				}
				cout << "    Line #: " << testScanner_tk.line << "}\n";
		}
	}

	return;
}
