#include <stdio.h>
#include "recursive_descent.h"
#include "table_driven_parser.h"

int main(int argc, char* argv[]) {
    printf("=== PART 1 ===\n");
//    recursive_descent_repl();
    printf("=== PART 2 ===\n");

    table_driven_parser_repl();
}