
#include <iostream>

#include "Utilities.h"

using namespace std;

/*
Traversiert den Baum mittels Rekursion komplett durch und zählt die Aufrufe.
*/
float countNode(Node* root){
    if ((!(root->left))&&(!(root->right))){
        return 1;
    } else {

        float countN=1;
        if (root->left) {
            countN+=countNode(root->left);
        }

        if (root->right){
            countN+=countNode(root->right);
        }

        return countN;
    }
};

/*
Rekursive Funktion mit Übergabe von node->left bis node->left == nullptr
Letzte node->value vor nullptr = minVal
*/
int minVal(Node* root){
    if (root->left==nullptr){
        return root->value;
    } else {
        return minVal(root->left);
    }
};

/*
Rekursive Funktion mit Übergabe von node->right bis node->right == nullptr.
Letzte node->value vor nullptr = maxVal
*/
int maxVal(Node* root){
    if (root->right==nullptr){
        return root->value;
    } else {
        return maxVal(root->right);
    }
};

float avgVal(Node* root){
    return (sumVal(root)/countNode(root));
};

/*
Traversiert den Baum mittels Rekursion komplett durch und summiert node->value.
*/
float sumVal(Node* root){
     if ((!(root->left))&&(!(root->right))){
        return root->value;
    } else {

        float sumV=root->value;
        if (root->left) {
            sumV+=sumVal(root->left);
        }
        if (root->right){
            sumV+=sumVal(root->right);
        }

        return sumV;
    }
};

void balance(Node* root){
    bool check=true;

    calcBalance(root,check);

    if (check) {
        cout << "AVL: yes" << endl;
    } else {
        cout << "AVL: no" << endl;
    }
};
/*
Berechnet die Balance pro Knoten, indem die Höhe des linken/rechten Teilbaumes 
rekursiv überprüft wird und gibt ggf. AVL-Violation aus.
*/

int calcBalance(Node* root, bool& check){
    int bal=0;
    int hr=0;
    int hl=0;

    if (root->right) hr=1+calcBalance(root->right,check);
    if (root->left) hl=1+calcBalance(root->left,check);

    bal=hr-hl;

    cout << "bal(" << root->value << ") = " << bal;

    if ((bal>1) || (bal<-1))
    {
        cout << " (AVL violation!)";
        check=false;
    }
    cout << endl;
    return hr>hl ? hr : hl;
};
