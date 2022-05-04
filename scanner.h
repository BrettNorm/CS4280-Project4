
#ifndef SCANNER_H
#define SCANNER_H



#include "token.h"

int grabColumn(char);
token find(int, string);
token scanner(ifstream&, int);
void testScanner(ifstream&);

struct findCharacter {
	int key;
	char letter;
};

struct findToken {
	int key;
	tokenID enum_tk;	
};


#endif
