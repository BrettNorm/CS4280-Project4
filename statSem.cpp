
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
static int j = 0;

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
			vCounter--;
			tempCounter--;
			oFile << "POP\n";
			semStack[i].token_Instance = "";
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
		// cout << semStack[i].semScope << "\n";
		// cout << scopeNum << "\n";
		if(semStack[i].semScope <= scopeNum && semStack[i].token_Instance == varString) { 
			return i;
			// return (counter - 1 - i); 
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

		int i = globalCounter;
		token globalCounterStack[globalCounter];
		while (i > 0) {
			globalCounterStack[i-1] = semStack[globalCounter-i];
			i--;
		}

		 semPop(scopeNum, oFile);
		 scopeNum--;

		oFile << "STOP\n";
		i = globalCounter;
		while(i > 0) {
			oFile << globalCounterStack[i-1].token_Instance << " 0\n";
			i--;
		}
		
		while(j > 0) {
			oFile << "T" << j-1 << " 0\n";
			j--;
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
				oFile << "LOAD " << semNode->t2.token_Instance << "\n";
				oFile << "STORE " << semNode->t2.token_Instance << "\n";
				globalCounter++;
				semCounter++;
			} else {
				semPush(semNode->t2);
				oFile << "PUSH\n";
				oFile << "LOAD " << semNode->t2.token_Instance << "\n";
				oFile << "STORE T" << tempCounter << "\n";
				semFound = semLocalOrParent(semNode->t2.token_Instance);
				oFile << "STACKW " << (vCounter - 1 - semFound) << "\n";
				semNode->t2.semScope = scopeNum;
				j++;
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
			j++;
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
			j++;

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
			j++;
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
			j++;
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
			int exists = semLocalOrParent(semNode->t1.token_Instance);
			if(exists == -1) {
				cout << "statSem.cpp <R> IDENT: " << semNode->t1.token_Instance << " on line " << semNode->t1.line <<" not declared in current scope\n";
				cout << "\nExiting program...\n";
				exit(1); 
			}
			if(semStack[exists].semScope == 0) {
				oFile << "LOAD " << semStack[exists].token_Instance << "\n";
			} else {
				oFile << "STACKR " << (vCounter - 1 - exists) << "\n";
			}


		} else if(semNode->t1.ID_tk == NUM_tk) {
			// int exists = semLocalOrParent(semNode->t1.token_Instance);
			// cout << semNode->t1.token_Instance << "\n";
			// if(exists == -1) {
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
			int exists = semLocalOrParent(semNode->t2.token_Instance);
			if(exists == -1) {
				// cout << "statSem.cpp: " << semNode->t2.token_Instance << " on line " << semNode->t2.line <<" not declared in current scope\n";
				// cout << "\nExiting program...\n";
                // exit(1);
                int tempVar = tempCounter;
                tempCounter++;
                j++;

				oFile << "PUSH\n";
				oFile << "READ T" << tempVar << "\n";
				oFile << "LOAD T" << tempVar << "\n";
				exists = semLocalOrParent(semNode->t2.token_Instance);
				oFile << "STACKW " << (vCounter - 1 - exists) << "\n";

			} else {
				if(semStack[exists].semScope == 0) {
					oFile << "READ " << semNode->t2.token_Instance << "\n";
				} else if(semStack[exists].semScope > 0) {
					oFile << "READ T" << (vCounter - 1 - exists) << "\n";
					oFile << "STACKW " << (vCounter - 1 - exists) << "\n";
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
				j++;
				
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
		j++;

		oFile << "STORE T" << tempVar << "\n";
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}

		if(RO == "=>") {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
        } else if(RO == "=<") {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
        } else if(RO == "==") {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
		} else if(RO[0] == '[') {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
		} else if(RO == "%") {
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
		int loopOne = ++loopCounter;
		oFile << "loop" << loopOne << ": ";
		string RO = semNode->c2->t1.token_Instance;
		if(semNode->c3 != NULL) { 
			semCheck(semNode->c3, semCounter, oFile); 
		}
		int tempVar = tempCounter;
		tempCounter++;
		j++;

		oFile << "STORE T" << tempVar << "\n";
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}

		if(RO == "=>") {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
		} else if(RO == "=<") {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
		} else if(RO == "==") {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
		} else if(RO[0] == '[') {
			oFile << "SUB T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
		} else if(RO == "%") {
			oFile << "MULT T" << tempVar << "\n";
			if(semNode->c2 != NULL) { 
				semCheck(semNode->c2, semCounter, oFile); 
			}
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


	/* <assign> */
	else if(semNode->name == "<assign>") {
		int exists = semLocalOrParent(semNode->t2.token_Instance);
		if(exists == -1) {
            cout << "statSem.cpp <assign> : " << semNode->t2.token_Instance << " on line " << semNode->t2.line << " not declared in current scope\n";
			cout << "\nExiting program...\n";
            exit(1);
        }
		
		if(semNode->c1 != NULL) { 
			semCheck(semNode->c1, semCounter, oFile); 
		}
		if(semStack[exists].semScope == 0) {
			oFile << "STORE " << semNode->t2.token_Instance << "\n";
		} else if(semStack[exists].semScope > 0) {
			oFile << "STORE T" << (vCounter - 1 - exists) << "\n";
			oFile << "STACKW " << (vCounter - 1 - exists) << "\n";
		}
	}


	/* <RO> */
	else if(semNode->name == "<RO>") {
		if(semNode->t1.ID_tk == Greater_equal_tk) {
				oFile << "BRZPOS loop" << (loopCounter + 1) << "\n";
		} else if(semNode->t1.ID_tk == Lesser_equal_tk) {
				oFile << "BRZNEG loop" << (loopCounter + 1) << "\n";
		} else if(semNode->t1.ID_tk == Equal_equal_tk) {
				oFile << "BRZERO loop" << (loopCounter + 1) << "\n";
		} else if(semNode->t1.ID_tk == Dot_tk) {
				oFile << "BRZERO loop" << (loopCounter + 1) << "\n";
		} else if(semNode->t1.ID_tk == Not_equal_tk) {
				oFile << "BRPOS loop" << (loopCounter + 1) << "\n";
				oFile << "BRNEG loop" << (loopCounter + 1) << "\n";
		}
    }


	/* <label> */
	else if(semNode->name == "<label>") {
		int exists = semLocalFind(semNode->t2.token_Instance);

		if(exists == -1 || exists > semCounter) {
			semPush(semNode->t2);
			semCounter++;
			oFile << semNode->t2.token_Instance << ": NOOP\n";

		} else if(exists < semCounter) {
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
		int exists = semLocalOrParent(semNode->t2.token_Instance);
		if(exists == -1) {
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
