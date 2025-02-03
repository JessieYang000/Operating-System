#include <stdio.h>

/* Run the following commands to check the memory layout in C
1. gcc file_1.c -o file_1  
2. size file_1 
*/

int global_variable = 5;
int global_variable_in_bss; // a global uninitialized variable will be store in bss(b​lock started by symbol) 
int main() {
    static int static_variable_1; // a static uninitialized variable
    static int static_variable_2 = 12; // a static initialized variable
    return 0;
}