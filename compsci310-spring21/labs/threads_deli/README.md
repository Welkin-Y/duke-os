# Deli simulation with user-level thread library

This project will give you experience writing simple multi-threaded programs
using monitors.  In addition to this lab, you are encouraged to review and practice
concurrency problems on the <a href="https://users.cs.duke.edu/~chase/systems/concurrency.html">concurrency page</a>.

You will see this lab referred to as "p1d" for historical reasons.
The moniker "p1" refers to a package of thread-related labs that
includes "p1t" (implementing threads) and "p1s" (implementing
synchronization).  They use the C++ language rather than C.

These labs are borrowed from Prof. Peter Chen at Michigan.  Please do
not publish your solution code.  We must answer to him for it.  Prior
abuses by Duke students have come to his attention, and it reflects
badly on us.

These labs are characterized by AG testing that is unusually stringent
and exacting.  For these labs the tests are "secret".  AG reveals
little about your specific errors.  The purpose is to place
responsibility for testing your code on you.  To further encourage you
to test your code, you are asked to create a test suite for the thread
library and submit it for grading.  The test suite is due with the
third thread lab p1s, but you should be working on it all along.  We
give you a small test case for deli, but you should also experiment
with larger test cases.

For p1d you write a simple concurrent program that schedules deli
orders. This concurrent program uses a thread library that we provide.
The grader considers a single source file: deli.cc.  It examines the
output from deli.cc under secret test cases.  You must follow the
instructions for your output byte-for-byte exactly to receive points
from the AG.

# Thread library interface


This section describes the interface to the thread library.
You write a multi-threaded program that uses this interface.  Later
your write your own implementation of this interface.

    int thread_libinit(thread_startfunc_t func, void *arg)

thread\_libinit initializes the thread library. A user program should call
thread\_libinit exactly once (before calling any other thread
functions). thread\_libinit creates and runs the first thread. This first thread
is initialized to call the function pointed to by func with the single argument
arg. Note that a successful call to thread\_libinit does not return to the
calling function. Instead, control transfers to func, and the function that
calls thread\_libinit will never execute again.

    int thread_create(thread_startfunc_t func, void *arg)

thread\_create is used to create a new thread. When the newly created
thread starts, it calls the function pointed to by func, passing the
single argument arg.

    int thread_yield(void)


thread\_yield causes the current thread to yield the CPU to the next
runnable thread. It has no effect if there are no other runnable threads.
thread\_yield is used to test the thread library. A normal concurrent
program should not depend on thread\_yield; nor should a normal concurrent
program produce incorrect answers if thread\_yield calls are inserted
arbitrarily.


    int thread_lock(unsigned int lock)
    int thread_unlock(unsigned int lock)
    int thread_wait(unsigned int lock, unsigned int cond)
    int thread_signal(unsigned int lock, unsigned int cond)
    int thread_broadcast(unsigned int lock, unsigned int cond)

thread\_lock, thread\_unlock, thread\_wait, thread\_signal, and thread\_broadcast
implement Mesa monitors in your thread library. 

A lock is identified by an unsigned integer (0 - 0xffffffff). Each lock
has a set of condition variables associated with it (numbered 0 -
0xffffffff), so a condition variable is identified uniquely by the tuple
(lock number, cond number). Programs can use arbitrary numbers for locks
and condition variables (i.e., they need not be numbered from 0 - n).
Note: it follows that if you use a condition number under two different locks, the
thread library understands it as two distinct condition variables.

Each of these functions returns -1 on failure. Each of these functions returns 0
on success, except for thread_libinit, which does not return at all on success.  *It is
wise to check your return values.*

A thread finishes when it returns from the function that was specified in thread_create.
When there are no runnable threads in the system (e.g. all threads have finished, or all threads are deadlocked), the thread library prints:

    Thread library exiting.

Bugs in your program may cause your thread library to exit sooner than expected.  If this happens, it is likely because one or more of your thread\_startfunc\_t in your program have returned prematurely, and/or all surviving threads are blocked, leaving nobody to wake them up (deadlock).


Here is the file "thread.h". DO NOT MODIFY OR RENAME IT. thread.h is to be
included by programs that use the thread library, and should also be included by
your library implementation.

    /*
    * thread.h -- public interface to thread library
    *
    * This file should be included in both the thread library and application
    * programs that use the thread library.
    */
    #ifndef _THREAD_H
    #define _THREAD_H
    
    #define STACK_SIZE 262144/* size of each thread's stack */
    
    typedef void (*thread_startfunc_t) (void *);
    
    extern int thread_libinit(thread_startfunc_t func, void *arg);
    extern int thread_create(thread_startfunc_t func, void *arg);
    extern int thread_yield(void);
    extern int thread_lock(unsigned int lock);
    extern int thread_unlock(unsigned int lock);
    extern int thread_wait(unsigned int lock, unsigned int cond);
    extern int thread_signal(unsigned int lock, unsigned int cond);
    extern int thread_broadcast(unsigned int lock, unsigned int cond);
    
    /*
    * start_preemptions() can be used in testing to configure the generation
    * of interrupts (which in turn lead to preemptions).
    *
    * The sync and async parameters allow several styles of preemptions:
    *
    * 1. async = true: generate asynchronous preemptions every 10 ms using
    * SIGALRM. These are non-deterministic.
    *
    * 2. sync = true: generate synchronous, pseudo-random preemptions before
    * interrupt_disable and after interrupt_enable. You can generate
    * different (but deterministic) preemption patterns by changing
    * random_seed.
    *
    * start_preemptions() should be called (at most once) in the application
    * function started by thread_libinit(). Make sure this is after the thread
    * system is done being initialized.
    *
    * If start_preemptions() is not called, no interrupts will be generated.
    *
    * The code for start_preemptions is in interrupt.cc, but the declaration
    * is in thread.h because it's part of the public thread interface.
    */
    extern void start_preemptions(bool async, bool sync, int random_seed);
    
    #endif /* _THREAD_H */

start\_preemptions() is part of the interrupt library we provide
(libinterrupt.a), but its declaration is included as part of the interface that
application programs include when using the thread library. Application programs
can call start\_preemptions() to configure whether (and how) timer interrupts are
generated (emulated) as the program runs.  These timer interrupts can
preempt a running thread and start the next ready thread by forcing the preempted thread to call
thread\_yield(). If you want to test a program in the presence of these
preemptions, have the application program call start\_preemptions() once in the
beginning of the function started by thread\_libinit().

# Concurrent Deli


In this part of the project, you write a concurrent program to simulate a
deli. We provide a working thread library (thread.o) for you to use while
testing your deli simulation. When you later write your thread library, the deli
program will also help you test your thread library (and vice versa).


The deli has a list of 1,000 sandwich types, and each sandwich is
assigned its own unique number between 0 and 999 on the
menu. Sandwiches that are numerically close share many of the same
ingredients. For example, a classic reuben (corned beef, swiss cheese,
sauerkraut, and Russian dressing on rye) might be sandwich number 2, a
"Dinty moore" (corned beef, lettuce, tomato, and Russian dressing on
rye) might be sandwich number 4, and a "BBQ Chicken" (pulled BBQ
chicken with white cheddar on a challah bun) might be sandwich number
977.

The deli contains one master sandwich maker and a variable number of cashiers,
who take orders from queued customers. The cashiers and sandwich maker
communicate via a fixed-size cork board. Cashiers post orders on the board so
that the sandwich maker knows (1) what to prepare, and (2) which cashier posted
the order. The board can hold a maximum number of order requests (max_orders).
Cashiers must wait if the board is full.

Your program should start by creating a specified number of cashier threads to
read in sandwich orders from a file and one thread for the sandwich maker.

There are no customer threads.  Instead, each cashier thread receives
and forwards a series of sandwich requests, which are specified in its
input file.

A cashier thread must wait until the sandwich maker has finished the
last request from the cashier before submitting its next order to
ensure that a cashier's orders are completed in FIFO order. This is to
prevent a customer from getting upset because someone behind them in
line received their sandwich first.

Also, because the sandwich maker's board has limited capacity,
cashiers may need to wait for an open spot to appear.

A cashier thread finishes after all the sandwiches in its input file
have been prepared (completed) by the sandwich maker.

While orders received by each cashier thread are prepared in FIFO order, the
sandwich maker does NOT prepare sandwiches from the board in FIFO
order. Instead, the sandwich maker's thread chooses the next sandwich based on
how similar it is to the one it just completed. This way the sandwich maker can
reduce latency between sandwiches by re-using as many materials as possible from
sandwich to sandwich. In other words, the next sandwich that the sandwich maker
should make is the one with the number on the board that is closest to the value
of the last sandwich it made. The maker is initialized with her last sandwich as -1.

Keep the cork board as full as possible to minimize average time between
sandwiches. That is, your sandwich-maker thread should handle a request only
when the cork board has the largest possible number of orders. This gives the
maker thread the largest number of sandwiches to choose from. Note that the
"largest number of orders" varies depending on how many cashier threads are
still active. When at least max\_orders cashier threads are alive, the largest
possible number of requests on the board is max\_orders. When fewer than
max\_orders cashier threads are alive, the largest number of orders on the board
is equal to the number of living cashier threads. You will probably want to
maintain the number of living cashier threads as shared state.

# Deli input

Your program is called with several command-line arguments. The first
argument specifies the maximum number of orders that the cork board can
hold. The rest of the arguments specify a list of input files (one input file
per cashier). I.e., the input file for cashier c is argv[c+2], where 0 <= c <
(number of cashiers). The number of cashier threads is given by the
number of input files specified.

The input file for each cashier contains that cashier's series of sandwich
orders. Each line of the input file specifies the requested sandwich (0 to
999). You may assume that input files are formatted correctly. Open each input
file read-only (use ifstream rather than fstream).

# Deli output

After issuing a request, a cashier thread should call (note the space characters
in the strings):

    cout << "POSTED: cashier " << cashier << " sandwich " << sandwich << endl;

An order is available to be made (i.e., has been posted to the board) when the
cashier thread prints this line.

After making a sandwich, the maker thread should make the following call (note
the space characters in the strings):

    cout << "READY: cashier " << cashier << " sandwich " << sandwich << endl;

An order is considered to be complete and off the board when the sandwich-maker
thread prints this line.

Your program should not generate any other output. Note that the console is
shared between the different threads. Hence the couts in your program must be
protected by a monitor lock to prevent interleaving output from multiple
threads.

## Sample input/output

Here is an example set of input files (sw.in0 - sw.in4). These sample input
files will be in the course repo.

    sw.in0 sw.in1 sw.in2 sw.in3 sw.in4
    ------ ------ ------ ------ ------
    53     914    827    302    631
    785    350    567    230    11

Here is one of several possible correct outputs from running the deli simulation with the following command:


    ./deli 3 sw.in0 sw.in1 sw.in2 sw.in3 sw.in4

(The final line of the output is produced by the thread library, not the deli simulation.)

    POSTED: cashier 0 sandwich 53
    POSTED: cashier 1 sandwich 914
    POSTED: cashier 2 sandwich 827
    READY: cashier 0 sandwich 53
    POSTED: cashier 3 sandwich 302
    READY: cashier 3 sandwich 302
    POSTED: cashier 4 sandwich 631
    READY: cashier 4 sandwich 631
    POSTED: cashier 0 sandwich 785
    READY: cashier 0 sandwich 785
    POSTED: cashier 3 sandwich 230
    READY: cashier 2 sandwich 827
    POSTED: cashier 4 sandwich 11
    READY: cashier 1 sandwich 914
    POSTED: cashier 2 sandwich 567
    READY: cashier 2 sandwich 567
    POSTED: cashier 1 sandwich 350
    READY: cashier 1 sandwich 350
    READY: cashier 3 sandwich 230
    READY: cashier 4 sandwich 11
    Thread library exiting.

## Tips

We provide a working thread library (thread.o) for you to use while testing
your deli. You should first get your deli working without preemption, then test
it with preemption enabled.  The section above explains how to configure
preemptions in your testing.

## Example program

Here is a short program that uses the thread library, along with the output generated by the program. Make sure you understand how the CPU is switching between two threads (both in function loop). "i" is on the stack and so is private to each thread. "g" is a global variable and so is shared among the two threads.

    #include <stdlib.h>
    #include <iostream>
    #include "thread.h"
    #include <assert.h>
    
    using namespace std;
    
    int g=0;
    
    void loop(void *a) {
      char *id;
      int i;
    
      id = (char *) a;
      cout <<"loop called with id " << (char *) id << endl;
    
      for (i=0; i<5; i++, g++) {
        cout << id << ":\t" << i << "\t" << g << endl;
        if (thread_yield()) {
          cout << "thread_yield failed\n";
          exit(1);
        }
      }
    }
    
    void parent(void *a) {
      int arg;
      arg = (long int) a;
    
      cout << "parent called with arg " << arg << endl;
      if (thread_create((thread_startfunc_t) loop, (void *) "child thread")) {
        cout << "thread_create failed\n";
        exit(1);
      }
    
      loop( (void *) "parent thread");
    }
    
    int main() {
      if (thread_libinit( (thread_startfunc_t) parent, (void *) 100)) {
        cout << "thread_libinit failed\n";
        exit(1);
      }
    }

Output:

    parent called with arg 100
    loop called with id parent thread
    	parent thread:00
    loop called with id child thread
    	child thread:00
    	parent thread:11
    	child thread:12
    	parent thread:23
    	child thread:24
    	parent thread:35
    	child thread:36
    	parent thread:47
    	child thread:48
    Thread library exiting.


# Project logistics

Write your thread library and deli simulation in C++ on Linux. The public
functions in thread.h are declared "extern", but all other functions and global
variables in your thread library should be declared "static" to prevent naming
conflicts with programs that link with your thread library.

Compile application source deli.cc into executable deli with the
solution thread library thread.o as follows:

    g++ -o deli thread.o deli.cc libinterrupt.a -ldl -no-pie

Use g++ (/usr/bin/g++) to compile your programs. You may use any functions
included in the standard C++ library, including (and especially) the STL. You
should not use any libraries other than the standard C++ library.

Your  deli simulation must be in a single file called "deli.cc".

Your git repo includes copies of thread.h, thread.o, interrupt.h, and libinterrupt.a.  Do not modify them.

Because your programs are auto-graded, you must be careful to follow the
exact rules in the project description:


1) (deli simulation)  Print only the two items specified above.

2) (deli simulation) Your program should expect several command-line
arguments, with the first being max_orders and the others
specifying the list of input files for the requester threads.

3) Do not modify source code included in this handout (thread.h,
interrupt.h).

