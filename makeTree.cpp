/*
    Tree printed using preorder traversal
*/


#include "makeTree.h"

using namespace std;

string tokens[] = {
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


static int level = 0;
static int spaceNeeded = 0;

void treeMaker(node *parseTree){
    if (parseTree != NULL){
        token token = parseTree->t1;
        for(int i = 0 ; i < parseTree->line ; i++){ 
            cout << " "; 
        }
        cout << parseTree->name;
        cout << "\n";
        
        if (token.token_Instance != "") {

            for(int i = 0 ; i < parseTree->line ; i++){ 
                cout << " "; 
            }    
            cout << "    {TokenID: " << tokens[token.ID_tk] << "    tokenInstance: " << token.token_Instance;
            if (token.token_Instance.length() < 8) {
                spaceNeeded = 8 - token.token_Instance.length();
                for (int i = 1; i < spaceNeeded; i++) {
                    cout << " ";
                }
            }
            cout << "    Line #: " << token.line << "}\n";
            token = parseTree->t2;


            if(tokens[token.ID_tk] != "EOF             ") {
                for(int i = 0 ; i < parseTree->line ; i++){ 
                    cout << " "; 
                }  
		
                cout << "    {TokenID: " << tokens[token.ID_tk] << "    tokenInstance: " << token.token_Instance;
                if (token.token_Instance.length() < 8) {
                    spaceNeeded = 8 - token.token_Instance.length();
                    for (int i = 1; i < spaceNeeded; i++) {
                        cout << " ";
                    }
                    }
                cout << "    Line #: " << token.line << "}\n";
                token = parseTree->t3; 


                if (tokens[token.ID_tk] != "EOF             ") {
                    for(int i = 0 ; i < parseTree->line ; i++){ 
                        cout << " "; 
                    }  

                    cout << "    {TokenID: " << tokens[token.ID_tk] << "    tokenInstance: " << token.token_Instance;
                    if (token.token_Instance.length() < 8) {
                        spaceNeeded = 8 - token.token_Instance.length();
                        for (int i = 1; i < spaceNeeded; i++) {
                            cout << " ";
                        }
                    }
                    cout << "    Line #: " << token.line << "}\n";
                    token = parseTree->t4;


                    if (tokens[token.ID_tk] != "EOF             ") {

                        for(int i = 0 ; i < parseTree->line ; i++){ 
                            cout << " "; 
                        } 
                        cout << "    {TokenID: " << tokens[token.ID_tk] << "    tokenInstance: " << token.token_Instance;
                        if (token.token_Instance.length() < 8) {
                            spaceNeeded = 8 - token.token_Instance.length();
                            for (int i = 1; i < spaceNeeded; i++) {
                                cout << " ";
                            }
                        }
                        cout << "    Line #: " << token.line << "}\n";
                        token = parseTree->t5;


                        if (tokens[token.ID_tk] != "EOF             ") {

                            for(int i = 0 ; i < parseTree->line ; i++){ 
                                cout << " "; 
                            } 
                            cout << "    {TokenID: " << tokens[token.ID_tk] << "    tokenInstance: " << token.token_Instance;
                            if (token.token_Instance.length() < 8) {
                                spaceNeeded = 8 - token.token_Instance.length();
                                for (int i = 1; i < spaceNeeded; i++) {
                                    cout << " ";
                                }
                            }
                            cout << "    Line #: " << token.line << "}\n";
                        }
                    }
                }		
            }
        }
       
        cout << "\n";
            
        /*     Children     */
        treeMaker(parseTree->c1);

        treeMaker(parseTree->c2);

        treeMaker(parseTree->c3);
  
        treeMaker(parseTree->c4);
 
        treeMaker(parseTree->c5);
 
    }
}