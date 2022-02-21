//
// Created by tusii on 2/21/2022.
//

#ifndef UNTITLED1_NODE_H
#define UNTITLED1_NODE_H
typedef struct node {
    /*
     * node with leftChild rightSibling
     */
    char* label;
    struct node* leftChild;
    struct node* rightSibling;
} tree;

extern tree* makeNode0(char* label);
extern tree* makeNode1(char* label, tree* t);
extern tree* makeNode2(char* label, tree* t1, tree* t2);
extern tree* makeNode3(char* label, tree* t1, tree* t2, tree* t3);
extern tree* addSibling(tree* t, char* label);
extern tree* addChild(tree* t, char* label);
#endif //UNTITLED1_NODE_H
