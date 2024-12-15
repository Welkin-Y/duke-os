#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    char* cb = (char*)malloc(14);
    strcpy(cb, "cool string");
    free(cb);
    /*
    * Dangling reference!
    */
    printf("%s\n", cb);

    /*
    * Uninitialized heap block!
    */
    char* cb2 = (char*)malloc(14);
    printf("%s\n", cb2);  
    
}