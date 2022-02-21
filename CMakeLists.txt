cmake_minimum_required(VERSION 3.17)
project(untitled1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_executable(untitled1 main.c
        LinkedList.h LinkedList.c
        IntHashSet.h IntHashSet.c
        Node.h Node.c
        pretty_print.h pretty_print.c
        recursive_descent.h recursive_descent.c
        table_driven_parser.h table_driven_parser.c)

