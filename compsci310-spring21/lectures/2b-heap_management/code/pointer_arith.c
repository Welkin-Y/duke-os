#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// this code is slightly different than whats on the slides
int main(int argc, char** argv) {
    char* cb = (char*) malloc(14);
    strcpy(cb, "abcdefg");

    void* ptr = cb;
    //ptr = ptr + 1;
 
    printf("%s\n", ptr); 
 
    free(ptr);

}