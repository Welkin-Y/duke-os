#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// this code is slightly different than whats on the slides
int main(int argc, char** argv) {
    uint8_t nums[8] = {0, 0, 0, 0, 0, 0, 0, 1};
 
    printf("as 64 bit integer: %lu\n", ((u_int64_t*) nums)[0]); 
    printf("as 2 32 bit integers: %x %x\n", ((u_int32_t*) nums)[0], ((u_int32_t*) nums)[1]); 

}