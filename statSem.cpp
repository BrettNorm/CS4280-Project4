
#include "statSem.h"

using namespace std;

static int counter = 0;
static int scopeNum = 0;
static int checkLine = 0;

token semStack[100];

/* push to stack */
void semPush(token stackToken) {

	if(counter > 99) {
		cout << "statSem.cpp: Stack full\n";
		cout << "\nExiting program...\n";
		exit(1);

	} else {
		for(int i = scopeNum ; i < counter ; i++) {
			if(semStack[i].token_Instance == stackToken.token_Instance) {
				cout << "statSem.cpp: " << stackToken.token_Instance << " on line " << stackToken.line << " already declared in current scope on line "<< semStack[i].line  << "\n";
				cout << "\nExiting program...\n";
				exit(1);
			}
		}
		semStack[counter].semScope = scopeNum;
		semStack[counter] = stackToken;
		counter++;
	}
}

/* pop off of stack */
void semPop(int pScope) {
	for(int i = 0 ; i < counter ; i++) {
		if(semStack[i].semScope == pScope) {
			counter--;
			semStack[i].token_Instance = "";
		}
	}
}

/* check for variable already declared in local scope */
int semLocalFind(string semString) {
	for(int i = 0 ; i <= 99 ; i++) {
		if(semStack[i].token_Instance == semString && semStack[i].semScope == scopeNum) { 
			return (counter - 1 - i); 
		} 
	}
	return -1;
}

/* check for variable already declared in local OR parent scope */
int semLocalOrParent(string varString) {
	for(int i = 0 ; i < counter ; i++) {
		if(semStack[i].semScope <= scopeNum && semStack[i].token_Instance == varString) { 
			return (counter - 1 - i); 
		}
	}
	return -1;
}

/* main driver for auditing semantics */
void semCheck(node *semNode, int semCounter) {


	if(semNode == NULL) { 
		return; 
	}
	

	/* <program> */
	if(semNode->name == "<program>") {
		int semNum = 0;
			
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semNum); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semNum); 
		}
		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semNum); 
		}
        if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semNum); 
		}
		if(semNode->c5 != NULL) { 
			semCheck(semNode->c5, semCounter); 
		}

	} 
	

	/* <block> */
	else if(semNode->name == "<block>") {
		int semNum = 0;
		scopeNum++;

		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semNum);
		}
        if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semNum); 
		}
		
		semPop(scopeNum);
		scopeNum--;	
	}


	/* <vars> */
	else if(semNode->name == "<vars>") {
		int semFound = 0;
		semFound = semLocalFind(semNode->t2.token_Instance);
		
		if(semFound > semCounter || semFound == -1) {
			semPush(semNode->t2);
			semCounter++;
		} else if(semFound < semCounter) {
			cout << "statSem.cpp: " << semNode->t2.token_Instance << " already declared in current scope on line " << semNode->t2.line << "\n";
			cout << "\nExiting program...\n";
            exit(1);
		}

		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter); 
		}
        if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter); 
		}
		if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semCounter); 
		}
		if(semNode->c5 != NULL) { 
			semCheck(semNode->c5, semCounter); 
		}
	}


	/* <expr> */
	else if(semNode->name == "<expr>") {
		if(semNode->t1.ID_tk == Minus_tk) {
			if(semNode->c1 != NULL) {
				semCheck(semNode->c1, semCounter); 
			}
            if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter); 
			}

		} else {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
		} 
	}


	/* <N> */
	else if(semNode->name == "<N>") {
		if(semNode->t1.ID_tk == For_slash_tk) {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
            if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter); 
			}
		} else if(semNode->t1.ID_tk == Plus_tk) {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
            if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter); 
			}
		} else {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter);
			}
		}
	}


	/* <A> */
	else if(semNode->name == "<A>") {
		if(semNode->t1.ID_tk == Asterisk_tk) {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
            if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter); 
			}
		} else {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
		}
	}


	/*<M>*/
	else if(semNode->name == "<M>") {
		if(semNode->t1.ID_tk == Modulus_tk) {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
		} else {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
		}
	}


	/* <R> */
	else if(semNode->name == "<R>") {
		if(semNode->t1.ID_tk == Open_par_tk) {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
		} else if(semNode->t1.ID_tk == IDENT_tk) {
			int exists = semLocalOrParent(semNode->t1.token_Instance);
			if(exists == -1) {
				cout << "statSem.cpp: " << semNode->t1.token_Instance << " on line " << semNode->t1.line <<" not declared in current scope\n";
				cout << "\nExiting program...\n";
				exit(1); 
			}
		} else if(semNode->t1.ID_tk == NUM_tk) {
			int exists = semLocalOrParent(semNode->t1.token_Instance);
			if(exists == -1) {
				cout << "statSem.cpp: " << semNode->t1.token_Instance << " on line " << semNode->t1.line <<" not declared in current scope\n";
				cout << "\nExiting program...\n";
				exit(1); 
			}
		} else {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
		}
	}


	/* <stats> */
	else if(semNode->name == "<stats>") {
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter); 
		}
	}


	/* <mStat> */
	else if(semNode->name == "<mStat>") {
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter); 
		}
	}


	/* <stat> */
	else if(semNode->name == "<stat>") {
		if(semNode->c1->name == "<in>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}

		} else if(semNode->c1->name == "<out>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
        } else if(semNode->c1->name == "<block>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}

        } else if(semNode->c1->name == "<if>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}

        } else if(semNode->c1->name == "<loop1>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}

        } else if(semNode->c1->name == "<loop2>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}

        } else if(semNode->c1->name == "<assign>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}

        } else if(semNode->c1->name == "<goto>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
        } else if(semNode->c1->name == "<label>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter); 
			}
        }
	}


	/* <in> */
	else if(semNode->name == "<in>") {
		if(semNode->t1.ID_tk == Listen_tk) {
			int exists = semLocalOrParent(semNode->t2.token_Instance);
			if(exists == -1) {
				cout << "statSem.cpp: " << semNode->t2.token_Instance << " on line " << semNode->t2.line <<" not declared in current scope\n";
				cout << "\nExiting program...\n";
                exit(1);
			}
		}
	}


	/* <out> */
	else if(semNode->name == "<out>") {
		if(semNode->t1.ID_tk == Yell_tk) {
			if(semNode->c1 != NULL) { 
				 semCheck(semNode->c1, semCounter); 
			}
		}
	}


	/* <if> */
	else if(semNode->name == "<if>") {
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter); 
		}
		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter); 
		}
		if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semCounter); 
		}
		if(semNode->c5 != NULL) { 
			semCheck(semNode->c5, semCounter); 
		}
	}


	/* <loop1> */
	else if(semNode->name == "<loop1>") {
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter); 
		}
		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter); 
		}
		if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semCounter); 
		}
		if(semNode->c5 != NULL) { 
			semCheck(semNode->c5, semCounter); 
		}
	}


	/* <loop2> */
	else if(semNode->name == "<loop2>") {
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter); 
		}
		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter); 
		}
		if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semCounter); 
		}
		if(semNode->c5 != NULL) { 
			semCheck(semNode->c5, semCounter); 
		}
	}


	/* <assign> */
	else if(semNode->name == "<assign>") {
		int exists = semLocalOrParent(semNode->t2.token_Instance);
		if(exists == -1) {
            cout << "statSem.cpp: " << semNode->t2.token_Instance << " on line " << semNode->t2.line << " not declared in current scope\n";
			cout << "\nExiting program...\n";
            exit(1);
        }
		
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter); 
		}
	}


	/* <label> */
	else if(semNode->name == "<label>") {
		int exists = semLocalFind(semNode->t2.token_Instance);

		if(exists == -1 || exists > semCounter) {
			semPush(semNode->t2);
			semCounter++;

		} else if(exists < semCounter) {
			cout << "statSem.cpp: " << semNode->t2.token_Instance << " already declared in current scope on line " << semNode->t2.line << "\n";
			cout << "\nExiting program...\n";
			exit(1);
		}

		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter); 
		}
	}


	/* <goto> */
	else if(semNode->name == "<goto>") {
		int exists = semLocalOrParent(semNode->t2.token_Instance);
		if(exists == -1) {
			cout << "statSem.cpp: " << semNode->t2.token_Instance << " on line " << semNode->t2.line << " not declared in current scope\n";
			cout << "\nExiting program...\n";
			exit(1);
		}

		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter); 
		}
	}


	else {
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter); 
		}
		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter); 
		}
		if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semCounter); 
		}
		if(semNode->c5 != NULL) { 
			semCheck(semNode->c5, semCounter); 
		}
	}
}
