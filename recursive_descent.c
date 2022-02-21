//
// Created by tusii on 2/21/2022.
//

#include "recursive_descent.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Node.h"
#include "pretty_print.h"

/** Global holders for input and input length */
char* input;
int input_length;

/*
 * basic match func
 */
bool match(char sym) {
    if (input_length == 0) {
        return false;
    }
    if (*input == sym) {
        input++;
        input_length--;
        return true;
    }
    return false;
}
/*
 * lookahead: returns true if the next character matches c
 */
bool lookahead(char c) {
    return *input == c;
}

tree* Expr();
tree* ExprTail();
tree* Atomic();
tree* Set();
tree* SetTail();
tree* Elements();
tree* ElementsTail();
tree* Element();
tree* Number();
tree* NumberTail();
tree* Digit();

tree* Expr() {
    tree* atomic = Atomic();
    if (atomic == NULL) return NULL;
    tree* exprTail = ExprTail();
    if (exprTail == NULL) return NULL;
    return makeNode2("Expr", atomic, exprTail);
}

tree* ExprTail() {
    if (lookahead('U')) {
        match('U'); // consume U
        tree* expr = Expr();
        if (expr == NULL) return NULL;
        return makeNode2("ExprTail", makeNode0("U"), expr);
    } else if (lookahead('^')) {
        match('^'); // consume ^
        tree* expr = Expr();
        if (expr == NULL) return NULL;
        return makeNode2("ExprTail", makeNode0("^"), expr);
    } else {
        return makeNode1("ExprTail", makeNode0("ϵ"));
    }
}

tree* Atomic() {
    if (!lookahead('(')) { // Set
        tree* set = Set();
        if (set == NULL) return NULL;
        return makeNode1("Atomic", set);
    } else {
        if (!match('(')) return NULL;
        tree* expr = Expr();
        if (expr == NULL) return NULL;
        if (!match(')')) return NULL;
        return makeNode3("Atomic", makeNode0("("), expr, makeNode0(")"));
    }
}

tree* Set() {
    if (!match('{')) return NULL;
    tree* setTail = SetTail();
    if (setTail == NULL) return NULL;
    return makeNode2("Set", makeNode0("{"), setTail);
}

tree* SetTail() {
    if (!lookahead('}')) { // <Elements> }
        tree* elements = Elements();
        if (elements == NULL) return NULL;
        if (!match('}')) return NULL;
        return makeNode2("SetTail", elements, makeNode0("}"));
    } else {
        if (!match('}')) return NULL;
        return makeNode1("SetTail", makeNode0("}"));
    }
}

tree* Elements() {
    tree* element = Element();
    if (element == NULL) return NULL;
    tree* elementsTail = ElementsTail();
    if (elementsTail == NULL) return NULL;
    return makeNode2("Elements", element, elementsTail);
}

tree* ElementsTail() {
    if (!lookahead(',')) {
        return makeNode1("ElementsTail", makeNode0("ϵ"));
    } else {
        if (!match(',')) return NULL;
        tree* elements = Elements();
        if (elements == NULL) return NULL;
        return makeNode2("ElementsTail", makeNode0(","), elements);
    }
}

tree* Element() {
    tree* number = Number();
    if (number == NULL) return NULL;
    return makeNode1("Element", number);
}

tree* Number() {
    tree* digit = Digit();
    if (digit == NULL) return NULL;
    tree* numberTail = NumberTail();
    if (numberTail == NULL) return NULL;
    return makeNode2("Number", digit, numberTail);
}

tree* NumberTail() {
    tree* number = Number();
    if (number == NULL) {
        return makeNode1("NumberTail", makeNode0("ε"));
    }
    return makeNode1("NumberTail", number);
}

tree* Digit() {
    // for each digit, lookahead for that digit. If it matches, call match to consume that digit, then return digit tree
    if (lookahead('1')) { match('1'); return makeNode1("Digit", makeNode0("1")); }
    if (lookahead('2')) { match('2'); return makeNode1("Digit", makeNode0("2")); }
    if (lookahead('3')) { match('3'); return makeNode1("Digit", makeNode0("3")); }
    if (lookahead('4')) { match('4'); return makeNode1("Digit", makeNode0("4")); }
    if (lookahead('5')) { match('5'); return makeNode1("Digit", makeNode0("5")); }
    if (lookahead('6')) { match('6'); return makeNode1("Digit", makeNode0("6")); }
    if (lookahead('7')) { match('7'); return makeNode1("Digit", makeNode0("7")); }
    if (lookahead('8')) { match('8'); return makeNode1("Digit", makeNode0("8")); }
    if (lookahead('9')) { match('9'); return makeNode1("Digit", makeNode0("9")); }
    return NULL;
}

void recursive_descent_repl() {
    while (!feof(stdin)) { // loop until end of file (ctrl-d)
        printf("Enter string to parse: \n");
        char word[255];
        scanf("%s", word);
        input = word;
        input_length = strlen(input);
        tree* tree = Expr();
        if (tree == NULL) {
            printf("Could not parse input.\n");
        } else {
            print_tree(tree);
        }
    }
}
