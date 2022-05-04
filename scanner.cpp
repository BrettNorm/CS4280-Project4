/*
	primary file for scanner
*/

#include "scanner.h"
using namespace std;


/* keywords */
token keyworkTokens[] = {
	{Start_tk, "start", 0}, {Stop_tk, "stop", 0}, {Repeat_tk, "repeat", 0}, {Until_tk, "until", 0}, {Whole_tk, "whole", 0}, 
	{Label_tk, "label", 0}, {Quit_tk, "quit", 0}, {Listen_tk, "listen", 0}, {Yell_tk, "yell", 0}, {Main_tk, "main", 0}, 
	{Portal_tk, "portal", 0}, {If_tk, "if", 0}, {Then_tk, "then", 0}, {Pick_tk, "pick", 0}, {Declare_tk, "declare", 0}, 
	{Assign_tk, "assign", 0}, {Proc_tk, "proc", 0}
};

/* tokens & final states */
findToken tokenKeys[] = {
	{-1, EOF_tk},  {-2, ERROR_tk}, {1000, IDENT_tk}, {1001, NUM_tk}, {1002, Equals_tk}, {1003, Greater_equal_tk}, 
	{1004, Lesser_equal_tk}, {1005, Colon_tk}, {1006, Not_equal_tk}, {1007, Equal_equal_tk}, {1008, Asterisk_tk}, 
	{1009, Modulus_tk}, {1010, For_slash_tk}, {1011, Plus_tk}, {1012, Dot_tk}, {1013, Open_par_tk}, 
	{1014, Close_par_tk}, {1015, Comma_tk}, {1016, Open_curly_tk}, {1017, Close_curly_tk}, {1018, Semicolon_tk,}, 
	{1019, Open_square_tk}, {1020, Close_square_tk}, {1021, Open_square_tk}, {1022, Close_square_tk}, 
	{1023, Open_curly_tk}, {1024, Colon_equals_tk}, {1025, Minus_tk}
};

/* characters and their columns */
findCharacter characterColumns[] = {
	{3, '='}, {4, ':'}, {5, '+'}, {6, '-'}, {7, '*'}, {8, '/'},
	{9, '%'}, {10, '.'}, {11, '('}, {12, ')'}, {13, ','},{14, '{'},
	{15, '}'}, {16, ';'}, {17, '['}, {18, ']'}
};


int fsaTable[26][20] = {
	/* WS     L     D     =     :     +     -     *     /     %     .     (     )     ,     {     }     ;     [     ]    EOF */
	{   0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,   16,   17,   18,   -1}, /* state 0 */
	{1000,    1,    1, 1000, 1000, 1000, 1000, 1012, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000}, /* state 1 */
	{1001, 1001,    2, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001}, /* state 2 */
	{1002, 1002, 1002,   22, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002}, /* state 3 */
	{1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005}, /* state 4 */
	{1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011}, /* state 5 */ 
	{1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025, 1025}, /* state 6 */
	{1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008}, /* state 7 */
	{1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010}, /* state 8 */
	{1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009}, /* state 9 */
	{1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012}, /* state 10 */
	{1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013}, /* state 11 */
	{1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014}, /* state 12 */
	{1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015}, /* state 13 */
	{1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016}, /* state 14 */
	{1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017}, /* state 15 */
	{1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018}, /* state 16 */
	{1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019}, /* state 17 */
	{1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020}, /* state 18 */
	{1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021}, /* state 19 */
	{1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022}, /* state 20 */
	{1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023}, /* state 21 */
	{1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007}, /* state 22 */
	{1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004}, /* state 23 */
	{1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006}, /* state 24 */
	{1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003}, /* state 25 */
};

token scanner(ifstream& inFile, int line) {

	string String_tk;
	token Scanner_tk;
	int stateColumn = 0, state = 0, nextState = 0;
	static int lineNum = line;
	char currentChar, nextChar;
	

	while( state >= 0 && state <= 999 ) {
		inFile.get(currentChar);

		/* ignoring comments */
		if(currentChar == '#') {
			int line = lineNum;
			inFile.get(currentChar);
			inFile.get(currentChar);
			
			inFile.get(nextChar);
			while(currentChar != '#' && nextChar != '#') {
				currentChar = nextChar;
				inFile.get(nextChar);
	
				/* if comment started but not closed */
				if(inFile.eof()) {
					cout << "scanner.cpp: comment started at line " << line << " but never closed\n";
					Scanner_tk.ID_tk = ERROR_tk;
					Scanner_tk.line = line;
					Scanner_tk.token_Instance = "Error";

					return Scanner_tk;
				}

				if(nextChar == '\n') {
					lineNum++;
				}
			}
			inFile.get(currentChar);
			inFile.get(currentChar);
		}
		
		/* end of file */
		if(inFile.eof()) {
			Scanner_tk.ID_tk = EOF_tk;
			Scanner_tk.line = lineNum;
			Scanner_tk.token_Instance = "EOF";
			
			return Scanner_tk;
		}	

		stateColumn = grabColumn(currentChar);
		nextState = fsaTable[state][stateColumn];	
		
		/* for double character operators */
		if(currentChar == '!') {
				inFile.get(nextChar);
				if (nextChar == '=') {
					Scanner_tk.ID_tk = Not_equal_tk;
					Scanner_tk.line = lineNum;
					Scanner_tk.token_Instance = "!=";
				}

				return Scanner_tk;
		} else if(currentChar == '<') {
				inFile.get(nextChar);
				if (nextChar == '=') {
					Scanner_tk.ID_tk= Lesser_equal_tk;
					Scanner_tk.line = lineNum;
					Scanner_tk.token_Instance = "<=";
				}

				return Scanner_tk;
		} else if(currentChar == '>') {
				inFile.get(nextChar);
				if (nextChar == '=') {
					Scanner_tk.ID_tk = Greater_equal_tk;
					Scanner_tk.line = lineNum;
					Scanner_tk.token_Instance = ">=";

				}

				return Scanner_tk;
		} else if(currentChar == ':') {
				inFile.get(nextChar);
				if (nextChar == '=') {
					Scanner_tk.ID_tk = Colon_equals_tk;
					Scanner_tk.line = lineNum;
					Scanner_tk.token_Instance = ":=";
				}
				return Scanner_tk;
		} else if(stateColumn == -2) {
			cout << "scanner.cpp: at line " << lineNum << " invalid character: " << currentChar << "\n";	
			Scanner_tk.token_Instance = currentChar;
			Scanner_tk.ID_tk = ERROR_tk;
			Scanner_tk.line = lineNum;

			return Scanner_tk;
		}
	
		if(nextState >= 1000 || nextState == -1) {
			/* final state */
			if(nextState >= 1000) {
				Scanner_tk = find(nextState, String_tk);
				Scanner_tk.line = lineNum;
				inFile.unget();


				return Scanner_tk;
			}

		} else if (nextState == -1) {
			/* end of file */
			if(nextState == -1) {
				Scanner_tk.ID_tk = EOF_tk;
				Scanner_tk.token_Instance = "End of file";
				Scanner_tk.line = lineNum;
				return Scanner_tk;
			}

		} else {
			/* keep going if not eof or final */
			state = nextState;
			char character = currentChar;
			
			/* if > 8 characters */
			if(String_tk.length() > 8) {
				cout << "scanner.cpp: line number: " << lineNum << "\n";
				cout << "More than 8 characters: " << String_tk << "\n";
				Scanner_tk.token_Instance = "ERROR";
				Scanner_tk.ID_tk = ERROR_tk;
				Scanner_tk.line = lineNum;

				return Scanner_tk;
			}

			if(!isspace(character)) {
				String_tk += character;
			}	

			if(currentChar == '\n') {
				lineNum++;
			}	
		}
	}
		
	/* if all else fails, we're catching what we assume is scanner.cpp at this point */
	Scanner_tk.token_Instance = "scanner.cpp: Scanner Failure\n";
	Scanner_tk.ID_tk = ERROR_tk;
	Scanner_tk.line = lineNum;
	
	return Scanner_tk;	
}


token find(int key, string instance) {
	token Return_tk;
	
	for(int i = 0 ; i < (sizeof(tokenKeys)/sizeof(tokenKeys[0])) ; i++) {
		if(tokenKeys[i].key == key) {
			Return_tk.token_Instance = instance;
			Return_tk.ID_tk = tokenKeys[i].enum_tk;
		}
	}

	if(key == 1000) {
		for(int j = 0 ; j < (sizeof(keyworkTokens)/sizeof(keyworkTokens[0])); j++) {
			if(keyworkTokens[j].token_Instance == instance) {
					Return_tk.token_Instance = keyworkTokens[j].token_Instance;
					Return_tk.ID_tk = keyworkTokens[j].ID_tk;

			}		
		}	
	}

	return Return_tk;
}


/* fsaTable column grabber */
int grabColumn(char nextChar) {
	
	if(isspace(nextChar)) {
		return 0;
	}
	else if(isalpha(nextChar) || nextChar == '_') {
		return 1;
	}
	else if(isdigit(nextChar)) {
		return 2;
	}
	else if(nextChar == EOF) {
		return 22;
	}
	else if(nextChar != EOF || !isalpha(nextChar) || !isdigit(nextChar) || !isspace(nextChar)) {
		for(int i = 0 ; i < (sizeof(characterColumns)/sizeof(characterColumns[0])) ; i++) {
			if(characterColumns[i].letter == nextChar) {
				return characterColumns[i].key;
			}
		}
	}
	return -2;
}







