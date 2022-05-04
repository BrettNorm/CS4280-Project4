
#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>

using namespace std;

enum tokenID {
	
	/* EOF, error, id, int tokens*/
	EOF_tk, ERROR_tk, IDENT_tk, NUM_tk, 
	
	/* individual keyword token */
	Start_tk, Stop_tk, Repeat_tk, Until_tk, Whole_tk, Label_tk, 
	Quit_tk, Listen_tk, Yell_tk, Main_tk, Portal_tk, If_tk, 
	Then_tk, Pick_tk, Declare_tk, Assign_tk, Proc_tk,

	/* operator tokens */
	Equals_tk, Greater_equal_tk, Lesser_equal_tk, Equal_equal_tk, Not_equal_tk, Colon_tk,
	Colon_equals_tk, Plus_tk, Minus_tk, Asterisk_tk, For_slash_tk, 
	Modulus_tk, Dot_tk, Open_par_tk, Close_par_tk, Comma_tk,
	Open_curly_tk, Close_curly_tk, Semicolon_tk, Open_square_tk, 
	Close_square_tk

};

struct token {
	tokenID ID_tk;
	string token_Instance;
	int line;
	int semScope;
};




#endif