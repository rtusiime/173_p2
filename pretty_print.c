//
// Created by tusii on 2/21/2022.
//

#include "pretty_print.h"
#include <stdio.h>
#include "Node.h"

/*
 * Internal _pprint func that prints formatting dependent on level
 */
void _pprint(tree* node, int level) {
    if (node == NULL) return;
    // space formatting dependent on level
    for (int i = 0; i<level; i++) {
        printf(" ");
    }
    printf("%s \n", node->label);
    _pprint(node->leftChild, level+1); // child so +1 to level
    _pprint(node->rightSibling, level); // sibling so same level
}

/*
 * Top level print_tree that calls _pprint
 */
void print_tree(tree* root) {
    _pprint(root, 0); // calls _pprint on root node at level 0
}
