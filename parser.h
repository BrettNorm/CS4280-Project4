/*
    Header file for parser, contains functions prototypes.
*/

#ifndef PARSER_H
#define PARSER_H

#include "node.h"
#include "scanner.h"

using namespace std;

node* getNode(string);
node* program(ifstream&);
node* block(ifstream&);
node* vars(ifstream&);
node* expr(ifstream&);

node* N(ifstream&);
node* A(ifstream&);
node* M(ifstream&);
node* R(ifstream&);

node* stats(ifstream&);
node* mStat(ifstream&);
node* stat(ifstream&);
node* in(ifstream&);

node* out(ifstream&);
node* if_F(ifstream&);
node* loop1(ifstream&);
node* loop2(ifstream&);
node* assign(ifstream&);

node* RO(ifstream&);
node* label(ifstream&);
node* goto_F(ifstream&);
node* parser(ifstream&);

#endif
