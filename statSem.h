#ifndef STATSEM_H
#define STATSEM_H

#include "node.h"
#include "token.h"

void semCheck(node *, int, ofstream&);
int semLocalFind(string);
int semLocalOrParent(string);
void semPush(token);
void semPop(int, ofstream&);

#endif