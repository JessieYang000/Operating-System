#include <stdio.h>

/* 
 * created in a read-only area
 */
char* str = "Hello";

int main() {
    printf("%s\n",str);
    
    str[0] = 'Y';
    
    printf("%s\n",str);
    return 0;
}
