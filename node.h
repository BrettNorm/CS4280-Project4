/*
	Holds node stucture for tree
*/

#ifndef NODE_H
#define NODE_H

#include "token.h"

using namespace std;

struct node {
	string name;
	int line;
	
	node *c1, *c2, *c3, *c4, *c5;
    token t1, t2, t3, t4, t5;
};

#endif