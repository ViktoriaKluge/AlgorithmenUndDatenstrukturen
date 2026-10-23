#ifndef TREE_H
#define TREE_H
#include <string>

using namespace std;

struct Node {
    int value;
    Node* left;
    Node* right;

    Node(int val): value(val), left(nullptr), right(nullptr) {}
};
void treecheck(string arg);
void searchTree(Node* root, string arg2);
bool checkSubtree(Node* maintree, Node* subtree);
void insert(Node*& node, int val);
bool searchValue(Node* node, int val);
void printWay(Node* node, int val);
void importTree (Node*& node, string filename);

#endif // TREE_H
