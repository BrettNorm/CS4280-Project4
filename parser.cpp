/*
	Holds functions for parsing the program for our particular grammar
*/


#include "parser.h"

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



token pToken;
token nextToken;
int line = 1;
int depth = 0;

/*		 Parser function 		*/
node* parser(ifstream& file){
	node* nd;

    while(file) {
        pToken = scanner(file, line);
		nd = program(file); 
    }

    return nd;
}


/* 		 Create new node 		*/
node* getNode(string n) {

	node *newNode = new node;

	newNode->c1 = NULL;
	newNode->c2 = NULL;
	newNode->c3 = NULL;
	newNode->c4 = NULL;
	newNode->c5 = NULL;
	
	newNode->line = depth;
	newNode->name = n;


	return newNode;
}

/* 		<program> -> <vars> main <block> 		*/
node* program(ifstream& inFile) {

	node *newNode = getNode("<program>");
	depth++;
	newNode->c1 = vars(inFile);
	
	if(pToken.ID_tk == Main_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		newNode->c2 = block(inFile);
		return newNode;

	} else {
		cout << "Parser.cpp: Wrong token... wanting Main Keyword but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
		exit(1);
	}
}


/* 		<block> -> { <vars> <stats> }		 */
node* block(ifstream& inFile) {

	node *newNode = getNode("<block>");

	if(pToken.ID_tk == Open_curly_tk) {
		depth++;
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		
		newNode->c1 = vars(inFile);
		newNode->c2 = stats(inFile);


		if(pToken.ID_tk == Close_curly_tk) {
			newNode->t2 = pToken;
			pToken = scanner(inFile, line);
			return newNode;

		} else {
			cout << pToken.token_Instance << "\n";
			cout << "Parser.cpp: Wrong token... wanting Close Curly Bracket but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
			exit(1);
		}
	} else {
		cout << "Parser.cpp: Wrong token... wanting Open Curly Bracket but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
		exit(1);
	}
}


/*		   <vars> -> empty | declare Identifier := whole ; <vars>		   */
node* vars(ifstream& inFile) {

	node *newNode = getNode("<vars>");

	if(pToken.ID_tk == Declare_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		
		if(pToken.ID_tk == IDENT_tk) {
			newNode->t2 = pToken;
			pToken = scanner(inFile, line);

			if(pToken.ID_tk == Colon_equals_tk) {
				newNode->t3 = pToken;
				pToken = scanner(inFile, line);
			
				if(pToken.ID_tk == Whole_tk) {
					newNode->t4 = pToken;
					pToken = scanner(inFile, line);
					
					if(pToken.ID_tk == Semicolon_tk) {
						depth++;
						newNode->t5 = pToken;
						pToken = scanner(inFile, line);
						newNode->c1 = vars(inFile);
						return newNode;
						
					} else {
						cout << "Parser.cpp: Wrong token... wanting Semicolon but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
						exit(1);
					}
				} else {
					cout << "Parser.cpp: Wrong token... wanting Whole Keyword but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
					exit(1);
				}
			} else {
				cout << "Parser.cpp: Wrong token... wanting Colon Equals but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
				exit(1);
			}
		} else {
			cout << "Parser.cpp: Wrong token... wanting Declare Keyword but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
			exit(1);
		}
	} else {
		return NULL;
	}
}


/*		  <expr> -> <N> - <expr> | <N>		  */
node* expr(ifstream& inFile) {

	node *newNode = getNode("<expr>");
	depth++;
	newNode->c1 = N(inFile);

	if(pToken.ID_tk == Minus_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		newNode->c2 = expr(inFile);
		return newNode;

	} else {
		return newNode;
	}
}

/*		 <N> -> <A> / <N> | <A> + <N> | <A> 		*/
node* N(ifstream& inFile) {

	node *newNode = getNode("<N>");
	depth++;
	newNode->c1 = A(inFile);

	if(pToken.ID_tk == For_slash_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		newNode->c2 = N(inFile);
		return newNode;

	} else if(pToken.ID_tk == Plus_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		newNode->c2 = N(inFile);
		return newNode;

	} else {
		
		return newNode;
	}
}

/*		 <A> -> <M>*<A> | <M> 		*/
node* A(ifstream& inFile) {
	
	node *newNode = getNode("<A>");
	depth++;
	newNode->c1 = M(inFile);

	if(pToken.ID_tk == Asterisk_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		newNode->c2 = A(inFile);
		return newNode;

	} else {
		return newNode;
	}
}

/*		 <M> -> % <M> | <R>		 */
node* M(ifstream& inFile) {

	node *newNode = getNode("<M>");
	depth++;

	if(pToken.ID_tk == Modulus_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		newNode->c1 = M(inFile);
		return newNode;

	} else {
		newNode->c1 = R(inFile);
		return newNode;
	}
}

/* 		<R> -> ( <expr> ) | Identifier | Integer		 */
node* R(ifstream& inFile) {
	
	node *newNode = getNode("<R>");
	depth++;

	if(pToken.ID_tk == Open_par_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		newNode->c1 = expr(inFile);

		if(pToken.ID_tk == Close_par_tk) {
			newNode->t2 = pToken;
			pToken = scanner(inFile, line);
			return newNode;

		} else {
			cout << "Parser.cpp: Wrong token... wanting Close Parenthesis but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
			exit(1);
		}
	} else if(pToken.ID_tk == IDENT_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);

		return newNode;
	} else if(pToken.ID_tk == NUM_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);

		return newNode;
	} else {
		cout << "Parser.cpp: Wrong token... wanting Open Parenthesis but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
		exit(1);
	}
}

/* 		<stats> -> <stat> <mStat> 		*/
node* stats(ifstream& inFile) {
	
	node *newNode = getNode("<stats>");
	depth++;
	
	newNode->c1 = stat(inFile);
	newNode->c2 = mStat(inFile);
	return newNode;
}

/* 		   <mStat> -> empty | <stat> <mStat>		   */
node* mStat(ifstream& inFile) {
	if( pToken.ID_tk == If_tk || pToken.ID_tk == Repeat_tk || pToken.ID_tk == Listen_tk || pToken.ID_tk == Yell_tk || pToken.ID_tk == Until_tk ||
		pToken.ID_tk == Assign_tk  || pToken.ID_tk == Label_tk || pToken.ID_tk == Open_curly_tk || pToken.ID_tk == Portal_tk) {
		
		node *newNode = getNode("<mStat>");
		depth++;
		newNode->c1 = stat(inFile);
		newNode->c2 = mStat(inFile);
		return newNode;

	} else {
		return NULL;
	}
}
 
/* 		  <stat> -> <in>; | <out>; | <block> | <if>; | <loop1>; | <loop2>; | <assign>; | <goto>; | <label>;		 */
node* stat(ifstream& inFile) {
	
	node *newNode = getNode("<stat>");
	depth++;
	
	if(pToken.ID_tk == Listen_tk) {
		newNode->c1 = in(inFile);

		if(pToken.ID_tk == Semicolon_tk) {
			newNode->t1 = pToken;
			pToken = scanner(inFile, line);
			return newNode;

		} else {
			cout << "Parser.cpp: Wrong token after Listen... wanting Semicolon but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
			exit(1);
		}
	} else if(pToken.ID_tk == Yell_tk) {
		newNode->c1 = out(inFile);

        if(pToken.ID_tk == Semicolon_tk) {
            newNode->t1 = pToken;
            pToken = scanner(inFile, line);
            return newNode;

        } else {
            cout << "Parser.cpp: Wrong token after Yell... wanting Semicolon but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
            exit(1);
        }

	} else if(pToken.ID_tk == Open_curly_tk) {
		newNode->c1 = block(inFile);
		return newNode;

	} else if(pToken.ID_tk == If_tk) {
		newNode->c1 = if_F(inFile);

        if(pToken.ID_tk == Semicolon_tk) {
            newNode->t1 = pToken;
            pToken = scanner(inFile, line);
            return newNode;

        } else {
            cout << "Parser.cpp: Wrong token after If... wanting Semicolon but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
            exit(1);

        }
	} else if(pToken.ID_tk == Repeat_tk) {

		pToken = scanner(inFile, line);

		if (pToken.ID_tk == Open_square_tk) {
			newNode->c1 = loop1(inFile);

			if(pToken.ID_tk == Semicolon_tk) {
				newNode->t1 = pToken;
				
				pToken = scanner(inFile, line);
				return newNode;

			} else {
				cout << "Parser.cpp: Wrong token after Repeat in loop1... wanting Semicolon but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
				exit(1);
			}	

		} else {
			newNode->c1 = loop2(inFile);
			return newNode;
	
		}
	} else if(pToken.ID_tk == Assign_tk) {
		newNode->c1 = assign(inFile);
		
        if(pToken.ID_tk == Semicolon_tk) {
			newNode->t1 = pToken;
            pToken = scanner(inFile, line);
            return newNode;

        } else {
            cout << "Parser.cpp: Wrong token... wanting Semicolon but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
            exit(1);
        }
	} else if(pToken.ID_tk == Portal_tk) {
		newNode->c1 = goto_F(inFile);

        if(pToken.ID_tk == Semicolon_tk) {
            newNode->t1 = pToken;
            pToken = scanner(inFile, line);

            return newNode;
        } else {
            cout << "Parser.cpp: Wrong token... wanting Semicolon but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
            exit(1);
        }
	} else if(pToken.ID_tk == Label_tk) {
		newNode->c1 = label(inFile);
        if(pToken.ID_tk == Semicolon_tk) {
            newNode->t1 = pToken;
            pToken = scanner(inFile, line);
            return newNode;

        } else {
            cout << "Parser.cpp: Wrong token... wanting Semicolon but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
            exit(1);
        }
	} else {
		cout << "Parser.cpp: wanting Listen, Yell, Open Curly Bracket, If, Repeat, Assign, Label, or Portal Keyword but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
		exit(1);
	}
}

/* 		    <in> -> listen Identifier		 */
node* in(ifstream& inFile) {
	
	node *newNode = getNode("<in>");
	depth++;
	
	if(pToken.ID_tk == Listen_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);

		if(pToken.ID_tk == IDENT_tk) {
			newNode->t2 = pToken;
			pToken = scanner(inFile, line);
			return newNode;

		} else {
			cout << "Parser.cpp: Wrong token... wanting Identifier but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
			exit(1);
		}
	} else {
		cout << "Parser.cpp: Wrong token... wanting Listen Keyword but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
		exit(1);
	}
}

/* 	          <out> -> yell <expr> 	    	*/
node* out(ifstream& inFile) {
	
    node *newNode = getNode("<out>");
	depth++;

    if(pToken.ID_tk == Yell_tk) {
		newNode->t1 = pToken;
       	pToken = scanner(inFile, line);
		newNode->c1 = expr(inFile);
		return newNode;
		
    } else {
        cout << "Parser.cpp: Wrong token... wanting Yell Keyword but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
        exit(1);
    }
}

/*  		<if> -> if [ <expr> <RO> <expr> ] then <stat>   		*/
node* if_F(ifstream& inFile) {
	if(pToken.ID_tk == If_tk) {
		
		node *newNode = getNode("<if>");
		depth++;
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		
		if(pToken.ID_tk == Open_square_tk) {
			newNode->t2 = pToken;
			pToken = scanner(inFile, line);
			newNode->c1 = expr(inFile);
	        newNode->c2 = RO(inFile);
        	newNode->c3 = expr(inFile);

			if(pToken.ID_tk == Close_square_tk) {
				newNode->t3 = pToken;
				pToken = scanner(inFile, line);
				
				if(pToken.ID_tk == Then_tk) {
					newNode->t4 = pToken;
					pToken = scanner(inFile, line);
					newNode->c4 = stat(inFile);
					return newNode;

				} else {
					cout << "Parser.cpp: Wrong token... wanting Then Keyword but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
                    exit(1);
				}	
			} else {
				cout << "Parser.cpp: Wrong token... wanting Close Bracket but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
                exit(1);
			}
		} else {
			cout << "Parser.cpp: Wrong token... wanting Open Bracket but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
			exit(1);
		}
	} else {
		cout << "Parser.cpp: Wrong token... wanting If Keyword but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
		exit(1); 
	}
}

/*		    <loop1> -> repeat [ <expr> <RO> <expr> ] <stat>	        */
node* loop1(ifstream& inFile) {

	node *newNode = getNode("<loop1>");
	depth++;
	newNode->t1.ID_tk = Repeat_tk;
	newNode->t1.token_Instance = "Repeat";
	newNode->t1.line = line;

	if(pToken.ID_tk == Open_square_tk) {
		newNode->t2 = pToken;
		pToken = scanner(inFile, line);
		newNode->c1 = expr(inFile);
		newNode->c2 = RO(inFile);
		newNode->c3 = expr(inFile);

		if(pToken.ID_tk == Close_square_tk) {
			newNode->t3 = pToken;
			pToken = scanner(inFile, line);
			newNode->c4 = stat(inFile);

			return newNode;
			

		} else {
			cout << "Parser.cpp: Wrong token in loop1... wanting Close Bracket but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
			exit(1);
		}
	
	} else {
		cout << "Parser.cpp: Wrong token in loop1 ... wanting Open Bracket but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
		exit(1);
	}
}

/* 		   <loop2> -> repeat <stat> until [ <expr> <R0> <expr> ]		   */
node* loop2(ifstream& inFile) {
		
	node* newNode = getNode("<loop2>");
	depth++;
	newNode->t1.ID_tk = Repeat_tk;
	newNode->t1.token_Instance = "Repeat";
	newNode->t1.line = line;
	newNode->c1 = stat(inFile);

	if(pToken.ID_tk == Until_tk){
		newNode->t2 = pToken;
		pToken = scanner(inFile, line);

		if(pToken.ID_tk == Open_square_tk) {
			newNode->t3 = pToken;
			pToken = scanner(inFile, line);
			newNode->c2 = expr(inFile);
			newNode->c3 = RO(inFile);
			newNode->c4 = expr(inFile);

			if(pToken.ID_tk == Close_square_tk) {
				newNode->t4 = pToken;
				pToken = scanner(inFile, line);
				return newNode;

			} else {
				cout << "Parser.cpp: Wrong token in loop2... wanting Close Bracket but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
				exit(1);
			}
		} else { 
			cout << "Parser.cpp: Wrong token in loop2... wanting Open Bracket but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
		exit(1);
		}		
	} else {
		cout << "Parser.cpp: Wrong token in loop2... wanting Until Keyword but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
		exit(1);
	}

	return newNode;
}

/* 		<assign> -> assign identifier = <expr> 		*/
node* assign(ifstream& inFile) {

	if(pToken.ID_tk == Assign_tk) {
		
		node *newNode = getNode("<assign>");
		depth++;
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);

		if(pToken.ID_tk == IDENT_tk) {
			newNode->t2 = pToken;
			pToken = scanner(inFile, line);

			if(pToken.ID_tk == Equals_tk) {
				newNode->t3 = pToken;
				pToken = scanner(inFile, line);
				newNode->c1 = expr(inFile);
				return newNode;

			} else {
				cout << "Parser.cpp: Wrong token in assign... wanting Equals but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
                exit(1);
			}
		} else {
			cout << "Parser.cpp: Wrong token in assign... wanting Identifier but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
            exit(1);	
		}
	} else {
		cout << "Parser.cpp: Wrong token in assign... wanting Assign Keyword but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
        exit(1);
	}
}

/* 		<RO> -> >= | <= | == | ... (three tokens here) | !=		 */
node* RO(ifstream& inFile) { 
	
	node *newNode = getNode("<RO>");
	depth++;

	if(pToken.ID_tk == Greater_equal_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		return newNode;

	} else if(pToken.ID_tk == Lesser_equal_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		return newNode;

	} else if(pToken.ID_tk == Equal_equal_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		return newNode;

	} else if(pToken.ID_tk == Dot_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);

		if(pToken.ID_tk == Dot_tk) {
			newNode->t2 = pToken;
			pToken = scanner(inFile, line);

			if(pToken.ID_tk == Dot_tk) {
				newNode->t3 = pToken;
				pToken = scanner(inFile, line);
				return newNode;

			} else {
				cout << "Parser.cpp: Wrong token in R0... on line " << depth << "... wanting '.'\n";
        	    exit(1);
			}
		} else {
			cout << "Parser.cpp: Wrong token in R0... on line " << depth << "... wanting '.'\n";
			exit(1); 
		}

	} else if(pToken.ID_tk == Not_equal_tk) {
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);
		return newNode;

	} else {
		cout << "Parser.cpp: Wrong token in R0... wanting <= , >= , == , ... , or != but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
		exit(1);
	}
}

/* 		  <label> -> label Identifier		 */
node* label(ifstream& inFile) {

	if(pToken.ID_tk == Label_tk) {
		
		node *newNode = getNode("<label>");
		depth++;
		newNode->t1 = pToken;
		pToken = scanner(inFile, line);

		if(pToken.ID_tk == IDENT_tk) {
			newNode->t2 = pToken;
			pToken = scanner(inFile, line);
			return newNode;

		} else {
			cout << "Parser.cpp: Wrong token in Label... wanting Identifier but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
            exit(1);
		}
	} else {
		cout << "Parser.cpp: Wrong token in Label... wanting Label Keyword but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
        exit(1);
	}
}

/*		 <goto> -> portal Identifier 		*/
node* goto_F(ifstream& inFile) {
	if(pToken.ID_tk == Portal_tk) {
        
        node *newNode = getNode("<goto>");
		depth++;
		newNode->t1 = pToken;
        pToken = scanner(inFile, line);

        if(pToken.ID_tk == IDENT_tk) {
            newNode->t2 = pToken;
            pToken = scanner(inFile, line);
            return newNode;

        } else {
            cout << "Parser.cpp: Wrong token in goto... wanting Identifier but received " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
            exit(1);
		}	
	} else {
            cout << "Parser.cpp: Wrong token in goto... wanting Portal Keyword but recieved " << tokenNames[pToken.ID_tk] << " on line " << depth << "\n";
            exit(1);
        }
}