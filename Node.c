//
// Created by tusii on 2/21/2022.
//

#include <stdlib.h>

#include "Node.h"

tree* makeNode0(char* label)
{
    tree* root = (tree*) malloc(sizeof(struct node));
    root->label = label;
    root->leftChild = NULL;
    root->rightSibling = NULL;
    return root;
}

tree* makeNode1(char* label, tree* t)
{
    tree* root = makeNode0(label);
    root->leftChild = t;
    return root;
}

tree* makeNode2(char* label, tree* t1, tree* t2)
{
    tree* root = makeNode0(label);
    root->leftChild = t1;
    t1->rightSibling = t2;
    return root;
}

tree* makeNode3(char* label, tree* t1, tree* t2, tree* t3)
{
    tree* root = makeNode1(label, t1);
    t1->rightSibling = t2;
    t2->rightSibling = t3;
    return root;
}

tree* makeNode4(char* label, tree* t1, tree* t2, tree* t3, tree* t4)
{
    tree* root = makeNode1(label, t1);
    t1->rightSibling = t2;
    t2->rightSibling = t3;
    t3->rightSibling = t4;
    return root;
}
tree* addChild(tree* t, char* label){
    if (t == NULL)
        return NULL;
    // Check if child list is not empty add the new label as a right sibling
    if (t->leftChild!=NULL){
        return addSibling(t->leftChild, label);
    }
    else{//if there's no left child, add it instead
        t->leftChild = makeNode0(label);
        return t->leftChild;
    }

}

// Adds a sibling to a list with starting with n
tree* addSibling(tree* t, char* label){
    if (t == NULL){
        return NULL;
    }
    while (t->rightSibling!=NULL){
        t = t->rightSibling;
    }
    t->rightSibling = makeNode0(label);
    return t->rightSibling;
}