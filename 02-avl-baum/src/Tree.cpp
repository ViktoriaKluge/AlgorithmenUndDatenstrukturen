#include <iostream>
#include <fstream>
#include <sstream>

#include "Tree.h"
#include "Utilities.h"

using namespace std;

void treecheck(string args){

    Node* root = new Node(-1);

    string arg1, arg2 = "";
    // Nimmt args und teilt diesen auf arg1 & arg2 auf.
    istringstream iss(args);
    iss >> arg1 >> arg2;

    if (arg2.empty()){
        importTree(root, arg1);
        if(root->value == -1) return;
        balance(root);
        cout << "min: " << minVal(root);
        cout << ", max: " << maxVal(root);
        cout << ", avg: " << avgVal(root) << endl;
    } else {
        importTree(root, arg1);
        if(root->value == -1) return;
        searchTree(root, arg2);
    }
};


void searchTree(Node* node, string arg2) {
    Node* s_tree = new Node(-1);
    importTree(s_tree, arg2);
    if(s_tree->value == -1) return;

    if ((s_tree->left == nullptr) && (s_tree->right == nullptr)) {

        if (searchValue(node,s_tree->value)) {
            printWay(node,s_tree->value);
        } else {
             cout << s_tree->value << " not found!" << endl;
        }
    } else {

        if (checkSubtree(node,s_tree)) {
            cout << "Subtree found!" << endl;
        } else {
            cout << "Subtree not found!" << endl;
        }
    }
};

/*
value aus Subtree wird mit Maintree verglichen -> wenn gefunden, werden beide Bäume weiter rekursiv traversiert.
*/
bool checkSubtree(Node* mainT, Node* searchT) {

    if (mainT== nullptr) return false;

    if (mainT->value == searchT->value) {
        bool checkLeft=true;
        bool checkRight=true;

        if (searchT->left) {
            checkLeft=checkSubtree(mainT->left,searchT->left);
        }

        if (searchT->right) {
            checkRight=checkSubtree(mainT->right,searchT->right);
        }

        return checkLeft && checkRight;

    }

    else if (searchT->value < mainT->value) return checkSubtree(mainT->left,searchT);
    else if (searchT->value > mainT->value) return checkSubtree(mainT->right,searchT);

    return false;

};

/*
Wir benötigen die Referenz auf einen Zeiger auf Node, da wir sonst nur die Kopie des Zeigers
innerhalb des Scopes verändern würden - nicht den Zeiger selbst. Also Node*&
Überprüft ob die aktuelle node ein nullptr ist -> falls ja, wird eine neue Node mit val erstellt.
Ansonsten wird der Wert überprüft, und die Funktion rekursiv aufgerufen mit node->left/node->right
*/

void insert(Node*& node, int val){
    if (node == nullptr) {
        node = new Node(val);
    } else if (val < node->value) {
        insert(node->left, val);
    } else if (val > node->value) {
        insert(node->right, val);
    }
};

/*
searchValue ruft sich rekursiv auf bis val (eventuell) gefunden wird.
Je nachdem ob node->value größer/kleiner als val ist, wird die Funktion mit node->right/node->left als Parameter aufgerufen..
*/
bool searchValue(Node* node,int val) {
    if (node == nullptr) return false; 
    if (node->value == val) {
        cout << val << " found ";
        return true;
    }
    else if (val < node->value) return searchValue(node->left, val);
    else return searchValue(node->right, val);
};

/*
printWay ruft sich rekursiv auf bis val gefunden wird und gibt den Weg dorthin zurück.
Je nachdem ob node->value größer/kleiner als val ist, wird die Funktion mit node->right/node->left als Parameter aufgerufen..
*/
void printWay(Node* node, int val) {
    if (node->value==val)
    {
        cout << node->value << endl;
    } else {
        cout << node->value << ", ";
        if (val < node->value) {
            printWay(node->left, val);
        } else if (val > node->value) {
            printWay(node->right, val);
        }
    }
};

void importTree(Node*& node, string filename) {
    ifstream file(filename);
    int val;
    if(!file) {
        cerr << "Datei " << filename << " konnte nicht geoeffnet werden!" << endl;
        return;
    }

    while (file >> val)
    {
        if (node->value == -1) {
            node->value = val;
        } else {
            insert(node, val);
        }
    }
};
