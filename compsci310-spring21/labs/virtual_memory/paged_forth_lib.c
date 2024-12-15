#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include "forth/forth_embed.h"
#include "paged_forth.h"

void* stackheap;
int max_pages;

void initialize_forth(struct forth_data *forth, int max_pages_input) {

    max_pages = max_pages_input;
    
    // the return stack is a forth-specific data structure if we
    // wanted to, we could give it an expanding memory segment like we
    // do for the stack/heap but I opted to keep things simple
    //
    // note this static is really important
    static char returnstack[1024];

    // because this might be called multiple times, we unmap the
    // region
    int result = munmap((void*) STACKHEAP_MEM_START, getpagesize()*NUM_PAGES);
    if(result == -1) {
        perror("error unmapping");
        exit(1);
    }

    
    int stackheap_size = getpagesize() * NUM_PAGES;

    stackheap = mmap((void*) STACKHEAP_MEM_START, stackheap_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);

    
    // printf("stack at %p\n", stackheap);

    initialize_forth_data(forth,
                          returnstack + sizeof(returnstack), //beginning of returnstack
                          stackheap, //begining of heap
                          stackheap + stackheap_size);

    
}
