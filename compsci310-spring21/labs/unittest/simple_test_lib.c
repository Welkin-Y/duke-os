#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "simple_test_lib.h"

#define MAX_TESTS 10

char* (*test_funcs[MAX_TESTS])(); // array of function pointers that store
                           // all of the tests we want to run
int num_tests = 0;

void add_test(char* (*test_func)()) {
    if(num_tests == MAX_TESTS) {
        printf("exceeded max possible tests");
        exit(1);
    }
    test_funcs[num_tests] = test_func;
    num_tests++;
}

void* run_test(void *test_to_run_void) {
    setup();
    char* (*test_func)() = test_to_run_void;
    return test_func();
}

void run_all_tests() {

    pthread_t tests[100];
    int count_passed = 0;
    int count_failed = 0;
    int count_crashed = 0; // these types get added later in the lab
    int count_timed_out = 0;
    
    
    for(int i = 0; i < num_tests; i++) {
        if(pthread_create(&tests[i], NULL, &run_test, test_funcs[i])) {

            printf("Error creating thread\n");
            exit(2);

        }
    }
    for(int i = 0; i < num_tests; i++) {
        char* result = NULL;
        if(pthread_join(tests[i],(void**) &result)) {
            printf("Error joining thread\n");
            exit(2);
        }
        if(result == TEST_PASSED) {
            printf("Test Passed\n");
            count_passed++;
        } else {
            printf("Test Failed: %s\n",result);
            count_failed++;
        }
    }
    printf("%s report: passed %d failed %d crashed %d timed out %d\n",
           TESTER_NAME, count_passed, count_failed, count_crashed, count_timed_out);
        
}

