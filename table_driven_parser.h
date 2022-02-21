//
// Created by tusii on 2/21/2022.
//

#ifndef UNTITLED1_TABLE_DRIVEN_PARSER_H
#define UNTITLED1_TABLE_DRIVEN_PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Node.h"
#include "pretty_print.h"
#include "LinkedList.h"
//parsing table
typedef int** table;

extern bool matcher(char* input_str , char stack_element, int* len);

/**
 * creates parsing table
 */
extern table construct_table(int categories);

/**
 * given parsing table and input string, carry out table-driven parse
 */
extern tree* parser(table table, char* string);

/**
 * Helper function to check if a symbol is a syntactic cartegory or terminal and returning respective
 * index
 * if -1, it's neither
 */
extern int is_category_or_terminal(char* string);

/**
 * Helper function to return the index of a syntactic category
 */
extern int category_number(char* category);

/**
 * frees parsing table
 */
extern void free_table(table parsing_table, int categories);

/**
 * given parsing table, syntactic category and lookahead symbol,
 * this function tells you what production to use
 */
extern int get_production(table parsing_table, int category, char sym);

/**
 * given production, push elements of production into stack in reverse order
 */
extern void push_elements(tree* parse_tree, LinkedList stack, int production);

/**
 * For the given parsing table, set the production from syntactic category
 * and lookahead symbol
 */
extern void set_production(table parsing_table, int category, char sym, int production);

/**
 * For the given parsing table, set the production from syntactic category
 * and lookahead symbol
 */
extern void set_production_str(table parsing_table, int category, char *str, int production);

/**
 * REPL for table_driven_parser
 */
extern   void  table_driven_parser_repl();

/**
 * adding elements of the production to the tree as children
 * of the leftmost child that IS NOT a terminal
 */
extern void add_to_tree(tree* parse_tree, char** string_arr, int children);

/**
 * add sibling to a given node
 */
extern tree* addSibling(tree* t, char* label);

/**
 * add child node to a given tree
 */
extern tree* addChild(tree* t, char* label);

#endif //UNTITLED1_TABLE_DRIVEN_PARSER_H
