#include "thread.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

// Enumerate function signatures
int main(int argc, char *argv[]);
void start(void *args);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cout << "Not enough inputs" << endl;
        return (0);
    }

    thread_libinit(start, argv);
}


void start_thread(void *args) {
    // Code goes here
}

void start(void *args) {
    thread_create(start_thread, args);
}
