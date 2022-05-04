
#include "statSem.h"

using namespace std;

static int vCounter = 0;
static int scopeNum = 0;
static int checkLine = 0;
static int tempCounter = 0;
static bool isGlobal = true;
static bool isLoop = false;
static int globalCounter = 0;
static int loopCounter = 0;
static int localCounter = 0;
static int pushCounter = 0;

token semStack[100];

/* push to stack */
void semPush(token stackToken) {
	
	if(vCounter > 99) {
		cout << "statSem.cpp: Stack full\n";
		cout << "\nExiting program...\n";
		exit(1);

	} else {
		for(int i = scopeNum ; i < vCounter ; i++) {
			if(semStack[i].token_Instance == stackToken.token_Instance) {
				cout << "statSem.cpp: " << stackToken.token_Instance << " on line " << stackToken.line << " already declared in current scope on line "<< semStack[i].line  << "\n";
				cout << "\nExiting program...\n";
				exit(1);
			}
		}

		semStack[vCounter] = stackToken;
		semStack[vCounter].semScope = scopeNum;
		stackToken.semScope = scopeNum;
		vCounter++;
	}
}

/* pop off of stack */
void semPop(int pScope, ofstream& oFile) {
	for(int i = 0 ; i < vCounter ; i++) {
		if(semStack[i].semScope == pScope) {
			oFile << "POP\n";
			semStack[i].token_Instance = "";
			tempCounter--;
		}
	}
}

/* check for variable already declared in local scope */
int semLocalFind(string semString) {
	for(int i = 0 ; i <= 99 ; i++) {
		if(semStack[i].token_Instance == semString && semStack[i].semScope == scopeNum) { 
			return (vCounter - 1 - i); 
		} 
	}
	return -1;
}

/* check for variable already declared in local OR parent scope */
int semLocalOrParent(string varString) {
	for(int i = 0 ; i < vCounter ; i++) {
		if(semStack[i].semScope <= scopeNum && semStack[i].token_Instance == varString) { 
			return i;
		}
	}
	return -1;
}

/* main driver for auditing semantics */
void semCheck(node *semNode, int semCounter, ofstream& oFile) {

	if(semNode == NULL) { 
		return; 
	}
	

	/* <program> */
	if(semNode->name == "<program>") {
		int semNum = 0;
			
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semNum, oFile); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semNum, oFile); 
		}
		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semNum, oFile); 
		}
        if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semNum, oFile); 
		}
		if(semNode->c5 != NULL) { 
			semCheck(semNode->c5, semNum, oFile); 
		}

		int programCounter = globalCounter;
		token globalCounterStack[globalCounter];
		while (programCounter > 0) {
			globalCounterStack[programCounter-1] = semStack[globalCounter-programCounter];
			programCounter--;
		}

		for (int i = 0; i < pushCounter; i++) {
			semPop(scopeNum, oFile);
			scopeNum--;
		}

		oFile << "STOP\n";
		programCounter = globalCounter;
		while(programCounter > 0) {
			oFile << globalCounterStack[programCounter-1].token_Instance << " 0\n";
			programCounter--;
		}
		while(localCounter > 0) {
			oFile << "T" << localCounter-1 << " 0\n";
			localCounter--;
		}

		return;
	} 
	

	/* <block> */
	else if(semNode->name == "<block>") {
		int semNum = 0;
		scopeNum++;
		isGlobal = false;

		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semNum, oFile);
		}
        if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semNum, oFile); 
		}
		
		semPop(scopeNum, oFile);
		scopeNum--;	
	}


	/* <vars> */
	else if(semNode->name == "<vars>") {
		int semFound = 0;
		semFound = semLocalFind(semNode->t2.token_Instance);
		
		if(semFound > semCounter || semFound == -1) {
			if (isGlobal == true) {
				semPush(semNode->t2);
				oFile << "PUSH\n";
				pushCounter++;
				oFile << "LOAD " << semNode->t2.token_Instance << "\n";
				oFile << "STORE " << semNode->t2.token_Instance << "\n";
				globalCounter++;
				semCounter++;
			} else {
				semPush(semNode->t2);
				oFile << "PUSH\n";
				pushCounter++;
				oFile << "LOAD " << semNode->t2.token_Instance << "\n";
				oFile << "STORE T" << tempCounter << "\n";
				semFound = semLocalOrParent(semNode->t2.token_Instance);
				oFile << "STACKW " << (vCounter - 1 - semFound) << "\n";
				semNode->t2.semScope = scopeNum;
				localCounter++;
				tempCounter++; 
				semCounter++;
			}
			
		} else if(semFound < semCounter) {
			cout << "statSem.cpp: " << semNode->t2.token_Instance << " already declared in current scope on line " << semNode->t2.line << "\n";
			cout << "\nExiting program...\n";
            exit(1);
		}

		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter, oFile); 
		}
        if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter, oFile); 
		}
		if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semCounter, oFile); 
		}
		if(semNode->c5 != NULL) { 
			semCheck(semNode->c5, semCounter, oFile); 
		}
	}


	/* <expr> */
	else if(semNode->name == "<expr>") {
		if(semNode->t1.ID_tk == Minus_tk) {
			int exprVar = tempCounter;
			tempCounter++;
			localCounter++;
			if(semNode->c1 != NULL) {
				semCheck(semNode->c1, semCounter, oFile); 
			}
			oFile << "STORE T" << exprVar << "\n";
            if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
			oFile << "SUB T" << exprVar << "\n";

		} else {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
		} 
	}


	/* <N> */
	else if(semNode->name == "<N>") {
		if(semNode->t1.ID_tk == For_slash_tk) {
			int tempVar = tempCounter;
			tempCounter++;
			localCounter++;

			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
			oFile << "STORE T" << tempVar << "\n";
            if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
			oFile << "DIV T" << tempVar << "\n";
		} else if(semNode->t1.ID_tk == Plus_tk) {
			int tempVar = tempCounter;
			tempCounter++;
			localCounter++;
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
			oFile << "STORE T" << tempVar << "\n";
            if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
            oFile << "ADD T" << tempVar << "\n";
		} else {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile);
			}
		}
	}


	/* <A> */
	else if(semNode->name == "<A>") {
		if(semNode->t1.ID_tk == Asterisk_tk) {
			int tempVar = tempCounter;
			tempCounter++;
			localCounter++;
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
			oFile << "STORE T" << tempVar << "\n";
            if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
			oFile << "MULT T" << tempVar << "\n";
		} else {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
		}
	}


	/*<M>*/
	else if(semNode->name == "<M>") {
		if(semNode->t1.ID_tk == Modulus_tk) {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
			oFile << "MULT -1\n";
		} else {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
		}
	}


	/* <R> */
	else if(semNode->name == "<R>") {
		if(semNode->t1.ID_tk == Open_par_tk) {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
		} else if(semNode->t1.ID_tk == IDENT_tk) {
			int semFound = semLocalOrParent(semNode->t1.token_Instance);
			if(semFound == -1) {
				cout << "statSem.cpp <R> IDENT: " << semNode->t1.token_Instance << " on line " << semNode->t1.line <<" not declared in current scope\n";
				cout << "\nExiting program...\n";
				exit(1); 
			}
			if(semStack[semFound].semScope == 0) {
				oFile << "LOAD " << semStack[semFound].token_Instance << "\n";
			} else {
				oFile << "STACKR " << (vCounter - 1 - semFound) << "\n";
			}


		} else if(semNode->t1.ID_tk == NUM_tk) {
			// int semFound = semLocalOrParent(semNode->t1.token_Instance);
			// cout << semNode->t1.token_Instance << "\n";
			// if(semFound == -1) {
			// 	cout << "statSem.cpp <R> NUM: " << semNode->t1.token_Instance << " on line " << semNode->t1.line <<" not declared in current scope\n";
			// 	cout << "\nExiting program...\n";
			// 	exit(1); 
			// }
			oFile << "LOAD " << semNode->t1.token_Instance << "\n";
		} else {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
		}
	}


	/* <stats> */
	else if(semNode->name == "<stats>") {
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter, oFile); 
		}
	}


	/* <mStat> */
	else if(semNode->name == "<mStat>") {
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter, oFile); 
		}
	}


	/* <stat> */
	else if(semNode->name == "<stat>") {
		if(semNode->c1->name == "<in>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}

		} else if(semNode->c1->name == "<out>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
        } else if(semNode->c1->name == "<block>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}

        } else if(semNode->c1->name == "<if>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}

        } else if(semNode->c1->name == "<loop1>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}

        } else if(semNode->c1->name == "<loop2>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}

        } else if(semNode->c1->name == "<assign>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}

        } else if(semNode->c1->name == "<goto>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
        } else if(semNode->c1->name == "<label>") {
			if(semNode->c1 != NULL) { 
				semCheck(semNode->c1, semCounter, oFile); 
			}
        }
	}


	/* <in> */
	else if(semNode->name == "<in>") {
		if(semNode->t1.ID_tk == Listen_tk) {
			int semFound = semLocalOrParent(semNode->t2.token_Instance);
			if(semFound == -1) {
				// cout << "statSem.cpp: " << semNode->t2.token_Instance << " on line " << semNode->t2.line <<" not declared in current scope\n";
				// cout << "\nExiting program...\n";
                // exit(1);
                int tempVar = tempCounter;
                tempCounter++;
                localCounter++;

				oFile << "PUSH\n";
				pushCounter++;
				oFile << "READ T" << tempVar << "\n";
				oFile << "LOAD T" << tempVar << "\n";
				semFound = semLocalOrParent(semNode->t2.token_Instance);
				oFile << "STACKW " << (vCounter - 1 - semFound) << "\n";

			} else {
				if(semStack[semFound].semScope == 0) {
					oFile << "READ " << semNode->t2.token_Instance << "\n";
				} else if(semStack[semFound].semScope > 0) {
					oFile << "READ T" << (vCounter - 1 - semFound) << "\n";
					oFile << "STACKW " << (vCounter - 1 - semFound) << "\n";
				}
			}
		}
	}


	/* <out> */
	else if(semNode->name == "<out>") {
		if(semNode->t1.ID_tk == Yell_tk) {
			if(semNode->c1 != NULL) { 
				 semCheck(semNode->c1, semCounter, oFile); 
				int tempVar = tempCounter;
				tempCounter++;
				localCounter++;
				
				oFile << "STORE T" << tempVar << "\n";
				oFile << "WRITE T" << tempVar << "\n";
			}
		}
	}


	/* <if> */
	else if(semNode->name == "<if>") {
		int loopVar = ++loopCounter;
		oFile << "loop" << loopVar << ": ";
		string RO = semNode->c2->t1.token_Instance;
		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter, oFile); 
		}
		int tempVar = tempCounter;
		tempCounter++;
		localCounter++;

		oFile << "STORE T" << tempVar << "\n";
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}

		if(RO == ">=") {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
        } else if(RO == "<=") {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
        } else if(RO == "==") {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
		} else if(RO[0] == '.') {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
		} else if(RO == "!=") {
			oFile << "MULT T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
		}

		if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semCounter, oFile); 
		}

		oFile << "loop" << (loopVar + 1) << ": NOOP\n";
		loopCounter++;
	}


	/* <loop1> */
	else if(semNode->name == "<loop1>") {
		int tempVar = tempCounter;
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile);
			oFile << "STORE T" << tempVar << "\n"; 
		}
		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter, oFile); 
			oFile << "SUB T" << tempVar << "\n";
		}

		if(semNode->c2 != NULL) {
				loopCounter++; 
				semCheck(semNode->c2, semCounter, oFile); 
				loopCounter--;
		}
		int loopOne = ++loopCounter;
		oFile << "loop1: ";
		string RO = semNode->c2->t1.token_Instance;
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}
		tempCounter++;
		localCounter++;
		oFile << "STORE T" << tempVar << "\n";

		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter, oFile); 
			oFile << "SUB T" << tempVar << "\n";
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter, oFile); 
		}
		if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semCounter, oFile); 
		}
		oFile << "BR loop" << loopOne << "\n";
		oFile << "loop" << (loopOne + 1) << ": NOOP\n";
		loopCounter++;
	}


	/* <loop2> */
	else if(semNode->name == "<loop2>") {
		int loopOne = loopCounter++;
		oFile << "loop1: ";			
		string RO = semNode->c3->t1.token_Instance;
		int tempVar = tempCounter;
		oFile << "STORE T" << tempVar << "\n";
		tempCounter++;
		localCounter++;
		
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}
		if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semCounter, oFile); 
		}

		oFile << "SUB T" << tempVar << "\n";

		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter, oFile); 
		}
		if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
		}
		oFile << "BR loop" << (loopOne + 1) << "\n";
		oFile << "loop" << (loopOne + 2) << ": NOOP\n";
		loopCounter++;
	}


	/* <assign> */
	else if(semNode->name == "<assign>") {
		int semFound = semLocalOrParent(semNode->t2.token_Instance);
		if(semFound == -1) {
            cout << "statSem.cpp <assign> : " << semNode->t2.token_Instance << " on line " << semNode->t2.line << " not declared in current scope\n";
			cout << "\nExiting program...\n";
            exit(1);
        }
		
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}
		if(semStack[semFound].semScope == 0) {
			oFile << "STORE " << semNode->t2.token_Instance << "\n";
		} else if(semStack[semFound].semScope > 0) {
			oFile << "STORE T" << (vCounter - 1 - semFound) << "\n";
			oFile << "STACKW " << (vCounter - 1 - semFound) << "\n";
		}
	}


	/* <RO> */
	else if(semNode->name == "<RO>") {
		if(semNode->t1.ID_tk == Greater_equal_tk) {
				oFile << "BRNEG loop" << (loopCounter + 1) << "\n";
		} else if(semNode->t1.ID_tk == Lesser_equal_tk) {
				oFile << "BRPOS loop" << (loopCounter + 1) << "\n";
		} else if(semNode->t1.ID_tk == Equal_equal_tk) {
				oFile << "BRZERO loop" << (loopCounter + 1) << "\n";
		} else if(semNode->t1.ID_tk == Dot_tk) {
				oFile << "BRZPOS loop" << (loopCounter + 1) << "\n";
		} else if(semNode->t1.ID_tk == Not_equal_tk) {
				oFile << "BRPOS loop" << (loopCounter + 1) << "\n";
				oFile << "BRNEG loop" << (loopCounter + 1) << "\n";
		}
    }


	/* <label> */
	else if(semNode->name == "<label>") {
		int semFound = semLocalFind(semNode->t2.token_Instance);

		if(semFound == -1 || semFound > semCounter) {
			semPush(semNode->t2);
			semCounter++;
			oFile << semNode->t2.token_Instance << ": NOOP\n";

		} else if(semFound < semCounter) {
			cout << "statSem.cpp: " << semNode->t2.token_Instance << " already declared in current scope on line " << semNode->t2.line << "\n";
			cout << "\nExiting program...\n";
			exit(1);
		}

		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}
	}


	/* <goto> */
	else if(semNode->name == "<goto>") {
		int semFound = semLocalOrParent(semNode->t2.token_Instance);
		if(semFound == -1) {
			cout << "statSem.cpp <goto> : " << semNode->t2.token_Instance << " on line " << semNode->t2.line << " not declared in current scope\n";
			cout << "\nExiting program...\n";
			exit(1);
		}
		oFile << "BR " << semNode->t2.token_Instance << "\n";
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}
	}


	else {
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}
		if(semNode->c2 != NULL) { 
			semCheck(semNode->c2, semCounter, oFile); 
		}
		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter, oFile); 
		}
		if(semNode->c4 != NULL) { 
			semCheck(semNode->c4, semCounter, oFile); 
		}
		if(semNode->c5 != NULL) { 
			semCheck(semNode->c5, semCounter, oFile); 
		}
	}
}
