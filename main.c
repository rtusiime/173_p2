#include <stdio.h>

#include "recursive_descent.h"
#include "table_driven_parser.h"

int main(int argc, char* argv[]) {
    while (!feof(stdin)) { // loop until end of file (ctrl-d)
        printf("Enter string to parse: \n");

        // the methods consume the strings, so make two copies
        char recursiveDescentWord[255];
        char tableDrivenWord[255];
        scanf("%s", recursiveDescentWord);
        strcpy(tableDrivenWord, recursiveDescentWord);

        printf("RECURSIVE DESCENT:\n");
        recursive_descent_parse_print(recursiveDescentWord);
        printf("\nTABLE DRIVEN:\n");
        table_driven_parse_print(tableDrivenWord);
    }
}