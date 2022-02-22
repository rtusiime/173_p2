//
// Created by tusii on 2/21/2022.
//

#include "LinkedList.h"
#include "table_driven_parser.h"



//PRODUCTIONS
/*
 *1)⟨Expr ⟩ → ⟨Atomic⟩ ⟨ExprTail⟩
2)⟨ExprTail⟩ → U ⟨Expr ⟩
3)⟨ExprTail⟩ → ^ ⟨Expr ⟩
4)⟨ExprTail⟩ → ϵ
5)⟨Atomic⟩ → ( ⟨Expr ⟩ )
6)⟨Atomic⟩ → ⟨Set⟩
7)⟨Set⟩ → { ⟨SetTail⟩
8)⟨SetTail⟩ → }
9)⟨SetTail⟩ →  ⟨Elements⟩ }
10)⟨Elements⟩ → ⟨Element⟩ ⟨ElementsTail⟩
11)⟨ElementsTail⟩ → , ⟨Elements⟩
12)⟨ElementsTail⟩ →  | ϵ
13)⟨Element⟩ → ⟨Number ⟩
14)⟨Number ⟩ → ⟨Digit⟩ ⟨NumberTail⟩
15)⟨NumberTail⟩ → ⟨Number ⟩
16)⟨NumberTail⟩ →  ϵ
17)⟨Digit⟩ → 0
18)⟨Digit⟩ → 1
19)⟨Digit⟩ → 2
20)⟨Digit⟩ → 3
21)⟨Digit⟩ → 4
22)⟨Digit⟩ → 5
23)⟨Digit⟩ → 6
24)⟨Digit⟩ → 7
25)⟨Digit⟩ → 8
26)⟨Digit⟩ → 9
 */

//CATEGORIES
/*
 * 1)⟨Expr⟩
2)⟨ExprTail⟩
3)⟨Atomic⟩
4)⟨Set⟩
5)⟨SetTail⟩
6)⟨Elements⟩
7)⟨ElementsTail⟩
8)⟨Element⟩
9)⟨Number ⟩
10)⟨NumberTail⟩
11)⟨Digit⟩
 */

/** Global flag boolean variable to stop recursive search for leftmost non-terminal */
bool flag;

/*
 * basic match func
 */
bool matcher(char** input_str , char stack_element, int* len) {
    if (*len== 0) {
        return false;
    }
    if (*(*input_str) == stack_element) {
        *input_str = (*input_str)+1;
        *len = (*len)-1;
        return true;
    }
    return false;
}
///*
// * lookahead: returns true if the next character matches c
// */
//bool lookahead(char* input_str, char stack_element) {
//    return *input_str == stack_element;
//}

table construct_table(int categories){
    table parsing_table =  (int**) calloc(categories, sizeof(int*));
    for (int i = 0; i < categories; i++) {
        parsing_table[i] = (int*) calloc(128, sizeof(int));
    }
    return parsing_table;
}


table table_for_grammar(){
    table parsing_table = construct_table(11);
    set_production(parsing_table,0,'(',1);
    set_production(parsing_table,0,'{',1);

    set_production(parsing_table,1,'e',4);
    set_production(parsing_table,1,'U',2);
    set_production(parsing_table,1,'^',3);

    set_production(parsing_table,2,'{',6);
    set_production(parsing_table,2,'}',5);

    set_production(parsing_table,3,'{',7);

    set_production_str(parsing_table,4,"0123456789",9);
    set_production(parsing_table,4,'}',8);

    set_production_str(parsing_table,5,"0123456789",10);

    set_production_str(parsing_table,6,"0123456789eU^{,()",12);
    set_production(parsing_table,6,'}',11);

    set_production_str(parsing_table,7,"0123456789",13);

    set_production_str(parsing_table,8,"0123456789",14);

    set_production_str(parsing_table,9,"0123456789",15);
    set_production_str(parsing_table,9,"eU^{,()",16);

    set_production(parsing_table,10,'0',17);
    set_production(parsing_table,10,'1',18);
    set_production(parsing_table,10,'2',19);
    set_production(parsing_table,10,'3',20);
    set_production(parsing_table,10,'4',21);
    set_production(parsing_table,10,'5',22);
    set_production(parsing_table,10,'6',23);
    set_production(parsing_table,10,'7',24);
    set_production(parsing_table,10,'8',25);
    set_production(parsing_table,10,'9',26);

    return parsing_table;
}
/*
 * create a stack of strings
 * create production data structure/just use switch statement
 * push exp, then
 * while stack is not empty
 * pop item
 * is item syntactic category? check table and return production
 * push production elements onto stack
 * else, if it's a terminal, match and consume
 *
 * if stack is empty, look ahead==EOM, accept. if lookahead!=eom, reject.
 */
tree* parser(table table, char* string){
    printf("boogers");
    LinkedList stack = new_LinkedList();
    LinkedList_add_at_front(stack,"Expr");
    printf("------------PRINTING STACK--------------\n");
    LinkedList_print(stack);
    printf("\n");
    int len = strlen(string);
    tree* parse_tree = makeNode0("Expr");
    while(!LinkedList_isEmpty(stack)) {
        char *symbol = LinkedList_pop(stack);
        int category_terminal_no = is_category_or_terminal(symbol);

        if (category_terminal_no < 0)return NULL;//if for some reason we get an invalid element
        else if (category_terminal_no <= 10) {//if it's category, check table and return production
            int production = get_production(table, category_terminal_no, *string);
            if (production == 0)return NULL;//if there's no production for the given category and symbol
            push_elements(parse_tree, stack, production);//push elements of production on stack
        } else {//if it's a terminal, match and consume
            //actually---------------> IMPORTANT, e is a special case, we won't consume, we will just continue
            if(*symbol=='e')continue;
            if (!matcher(&string, *symbol, &len)) { return NULL; }
        }
    }
    if(LinkedList_isEmpty(stack)&& len<=0)return parse_tree;
    else return NULL;
}

/*
 * pNODE seek(char let, pNODE n)
{
(1) c = n->leftmostChild;
(2) while (c != NULL)
(3) if (c->letter == let)
(4) break;
else
(5) c = c->rightSibling;
(6) return c;
}

 */

void add_to_tree(tree* parse_tree, char** string_arr, int children){
    if(parse_tree==NULL||flag==true)return;
    add_to_tree(parse_tree->leftChild, string_arr,children);
    if(is_category_or_terminal(parse_tree->label)<=10&&is_category_or_terminal(parse_tree->label)>=0&&parse_tree->leftChild==NULL){
        flag = true;
        for(int i=0; i<children;i++){
            addChild(parse_tree, string_arr[i]);
        }
    }
    add_to_tree(parse_tree->rightSibling,string_arr,children);

}

void push_elements(tree* parse_tree, LinkedList stack, int production){

    switch (production) {//push elements of returned production in reverse order
        case 1:
            LinkedList_add_at_front(stack,"ExprTail");
            LinkedList_add_at_front(stack,"Atomic");
            printf("------------PRINTING STACK--------------\n");
            LinkedList_print(stack);
            printf("\n");
            char *p_one[2] = {"Atomic","ExprTail",};
            add_to_tree(parse_tree,p_one,2);
            flag=false;
            break;
        case 2:
            //add_to_tree(tree, char* string);
            LinkedList_add_at_front(stack,"Expr");
            LinkedList_add_at_front(stack,"U");
            char *p_two[2] = {"U","Expr",};
            add_to_tree(parse_tree,p_two,2);
            flag=false;
            break;
        case 3:
            LinkedList_add_at_front(stack,"Expr");
            LinkedList_add_at_front(stack,"^");
            char *p_three[2] = {"^","Expr",};
            add_to_tree(parse_tree,p_three,2);
            flag=false;
            break;
        case 4:
            LinkedList_add_at_front(stack,"e");
            char *p_four[1] = {"e"};
            add_to_tree(parse_tree,p_four,1);
            flag=false;
            break;
        case 5:
            LinkedList_add_at_front(stack,")");
            LinkedList_add_at_front(stack,"Expr");
            LinkedList_add_at_front(stack,"(");
            char *p_five[3] = {"(","Expr",")"};
            add_to_tree(parse_tree,p_five,3);
            flag=false;
            break;
        case 6:
            LinkedList_add_at_front(stack,"Set");
            char *p_six[1] = {"Set"};
            add_to_tree(parse_tree,p_six,1);
            flag=false;
            break;
        case 7:
            LinkedList_add_at_front(stack,"SetTail");
            LinkedList_add_at_front(stack,"{");
            char *p_seven[2] = {"{","SetTail",};
            add_to_tree(parse_tree,p_seven,2);
            flag=false;
            break;
        case 8:
            LinkedList_add_at_front(stack,"}");
            char *p_eight[1] = {"}"};
            add_to_tree(parse_tree,p_eight,1);
            flag=false;

            break;
        case 9:
            LinkedList_add_at_front(stack,"}");
            LinkedList_add_at_front(stack,"Elements");
            char *p_nine[2] = {"Elements","}"};
            add_to_tree(parse_tree,p_nine,2);
            flag=false;
            break;
        case 10:
            LinkedList_add_at_front(stack,"ElementsTail");
            LinkedList_add_at_front(stack,"Element");
            char *p_ten[2] = {"Element","ElementsTail"};
            add_to_tree(parse_tree,p_ten,2);
            flag=false;
            break;
        case 11:
            LinkedList_add_at_front(stack,"Elements");
            LinkedList_add_at_front(stack,",");
            char *p_eleven[2] = {",","Elements"};
            add_to_tree(parse_tree,p_eleven,2);
            flag=false;
            break;
        case 12:
            LinkedList_add_at_front(stack,"e");
            char *p_twelve[1] = {"e"};
            add_to_tree(parse_tree,p_twelve,1);
            flag=false;
            break;
        case 13:
            LinkedList_add_at_front(stack,"Number");
            char *p_13[1] = {"Number"};
            add_to_tree(parse_tree,p_13,1);
            flag=false;
            break;
        case 14:
            LinkedList_add_at_front(stack,"NumberTail");
            LinkedList_add_at_front(stack,"Digit");
            char *p_fourteen[2] = {"Digit","NumberTail"};
            add_to_tree(parse_tree,p_fourteen,2);
            flag=false;
            break;
        case 15:
            LinkedList_add_at_front(stack,"Number");
            char *p_fifteen[1] = {"Number"};
            add_to_tree(parse_tree,p_fifteen,1);
            flag=false;
            break;
        case 16:
            LinkedList_add_at_front(stack,"e");
            char *p_sixteen[1] = {"e"};
            add_to_tree(parse_tree,p_sixteen,1);
            flag=false;
            break;
        case 17:
            LinkedList_add_at_front(stack,"0");
            char *p_seventeen[1] = {"0"};
            add_to_tree(parse_tree,p_seventeen,1);
            flag=false;
            break;
        case 18:
            LinkedList_add_at_front(stack,"1");
            char *p_eighteen[1] = {"1"};
            add_to_tree(parse_tree,p_eighteen,1);
            flag = false;
            break;
        case 19:
            LinkedList_add_at_front(stack,"2");
            char *p_nineteen[1] = {"2"};
            add_to_tree(parse_tree,p_nineteen,1);
            flag=false;
            break;
        case 20:
            LinkedList_add_at_front(stack,"3");
            char *p_twenty[1] = {"3"};
            add_to_tree(parse_tree,p_twenty,1);
            flag = false;
            break;
        case 21:
            LinkedList_add_at_front(stack,"4");
            char *p_21[1] = {"4"};
            add_to_tree(parse_tree,p_21,1);
            flag=false;
            break;
        case 22:
            LinkedList_add_at_front(stack,"5");
            char *p_22[1] = {"5"};
            add_to_tree(parse_tree,p_22,1);
            flag=false;
            break;
        case 23:
            LinkedList_add_at_front(stack,"6");
            char *p_23[1] = {"6"};
            add_to_tree(parse_tree,p_23,1);
            flag=false;
            break;
        case 24:
            LinkedList_add_at_front(stack,"7");
            char *p_24[1] = {"7"};
            add_to_tree(parse_tree,p_24,1);
            flag=false;
            break;
        case 25:
            LinkedList_add_at_front(stack,"8");
            char *p_25[1] = {"8"};
            add_to_tree(parse_tree,p_25,1);
            flag=false;
            break;
        case 26:
            LinkedList_add_at_front(stack,"9");
            char *p_26[1] = {"9"};
            add_to_tree(parse_tree,p_26,1);
            flag=false;
            break;
    }
    printf("-----------------------------\n");
    printf("production is %d \n", production);
    printf("---------------PRINTING TREE--------------\n");
    print_tree(parse_tree);
    printf("\n");
    printf("------------PRINTING STACK--------------\n");
    LinkedList_print(stack);
    printf("\n");
}

int is_category_or_terminal(char* string){
    if(strcmp(string,"Expr")==0)return 0;
    if(strcmp(string,"ExprTail")==0)return 1;
    if(strcmp(string,"Atomic")==0)return 2;
    if(strcmp(string,"Set")==0)return 3;
    if(strcmp(string,"SetTail")==0)return 4;
    if(strcmp(string,"Elements")==0)return 5;
    if(strcmp(string,"ElementsTail")==0)return 6;
    if(strcmp(string,"Element")==0)return 7;
    if(strcmp(string,"Number")==0)return 8;
    if(strcmp(string,"NumberTail")==0)return 9;
    if(strcmp(string,"Digit")==0)return 10;
    if(strcmp(string,"0")==0)return  (int)'0';
    if(strcmp(string,"1")==0)return  (int)'1';
    if(strcmp(string,"2")==0)return  (int)'2';
    if(strcmp(string,"3")==0)return  (int)'3';
    if(strcmp(string,"4")==0)return  (int)'4';
    if(strcmp(string,"5")==0)return  (int)'5';
    if(strcmp(string,"6")==0)return  (int)'6';
    if(strcmp(string,"7")==0)return  (int)'7';
    if(strcmp(string,"8")==0)return  (int)'8';
    if(strcmp(string,"9")==0)return  (int)'9';
    if(strcmp(string,"e")==0)return  (int)'e';
    if(strcmp(string,"U")==0)return  (int)'U';
    if(strcmp(string,"^")==0)return  (int)'^';
    if(strcmp(string,"{")==0)return  (int)'{';
    if(strcmp(string,"}")==0)return  (int)'}';
    if(strcmp(string,",")==0)return  (int)',';
    if(strcmp(string,"(")==0)return  (int)'(';
    if(strcmp(string,")")==0)return  (int)')';
    else return -1;
}

void free_table(table parsing_table, int categories){
    for (int i = 0; i < categories; i++) {
        free(parsing_table[i]);
    }
    free(parsing_table);
}

int get_production(table parsing_table, int category, char sym) {
//    int category_no = category_number(category);
    return parsing_table[category][(int) sym];
}


void set_production(table parsing_table, int category, char sym, int production) {
    int isym = (int) sym;
    parsing_table[category][isym] = production;
}

void set_production_str(table parsing_table, int category, char *str, int production) {
    for (int i = 0; i < strlen(str); i++) {
        set_production(parsing_table, category, str[i], production);
    }
}


void table_driven_parser_repl() {
    while (!feof(stdin)) { // loop until end of file (ctrl-d)
        printf("Enter string to parse: \n");
        char word[255];
        scanf("%s", word);
        table parse_table =table_for_grammar();
        tree* tree = parser(parse_table,word);//Expr()
        if (tree == NULL) {
            printf("Could not parse input.\n");
        } else {
            print_tree(tree);
        }
    }
}
