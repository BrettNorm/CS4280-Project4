#ifndef STATSEM_H
#define STATSEM_H

#include "node.h"
#include "token.h"

void semCheck(node *, int);
int semLocalFind(string);
int semLocalOrParent(string);
void semPush(token);
void semPop(int);

#endif