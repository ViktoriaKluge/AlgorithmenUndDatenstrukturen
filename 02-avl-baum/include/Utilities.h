#ifndef UTILITIES_H
#define UTILITIES_H

#include "Tree.h"

float countNode(Node* root);

int minVal(Node* root);
int maxVal(Node* root);
float avgVal(Node* root);
float sumVal(Node* root);

void balance(Node* root);
int calcBalance(Node* root, bool& check);

#endif // UTILITIES_H
