# Testing the Thread Library

Very soon we are going to ask you to build the thread library that you
used in the concurrent deli.  But before you start that, you'll need a
robust suite of test cases.  As testing is a significant part of
system work, we'd like you to build these tests yourself.  Then you'll
use this test suite to test your thread library.

To make sure your thread library is good, we've developed a suite of
buggy thread library implementations.  Your goal will be detect those
buggy implementations are buggy - it you do that for all of them,
you'll get 100% on this part and you can move on to implementing the
library itself.

# What is a test case?

Each test case for the thread library is a short C++ program that uses
functions in the thread library (e.g., the example program
above). These programs take no inputs: no arguments and no input
files. Test cases should exit(0) when run with a correct thread
library (normally this happens when your test case's last runnable
thread ends or blocks). 

If your test case finds that the thread library has some bug, it
should report it in its output on stdout (cout, printf). It does not
matter exactly what the test case outputs: you win the points iff it
generates different output on a buggy thread library than it does on a
correct thread library. Your test cases may also generate output on
stderr, but the auto-grader ignores stderr. Output files are not
permitted: output only to stdout and (optionally) stderr.

Your test cases should NOT call start\_preemption(), because we are not
evaluating how thoroughly your test suite exercises the
interrupt\_enable() and interrupt\_disable() calls.

Your test suite may contain up to 20 test cases. Each test case may
generate at most 10 KB of output and must take less than 60 seconds to
run. These limits are much larger than needed for full credit.

I've developed a sample test case and included it in the repository so
you can see what I mean.

# Running your test case

You can test your tests cases with the correct thread library like
this:

    g++ -o test0 thread.o test0.cc libinterrupt.a -ldl -no-pie

Only AG350 has our buggy implementations - so you'll have to test
against those there.

# Can I use my deli as a test case?

Yes, but if you submit your deli simulation as a test
case, remember to specify all inputs (number of requesters, buffers,
and the list of requests) statically in the program. This shouldn't be
too inconvenient because the list of requests should be short to make
a good test case (i.e. one that you can trace through what should
happen).

You should also not expect your deli to detect all or even most buggy
implementations.  A good test suite often contains a few big programs
like deli, but mostly is targeted tests designed to rigorously look
and individual functions and error cases.


# Error handling

One thing any good test suite should exercise is error handling - that
is that the system responds to bad calling correctly.

Your thread library must detect when a user program misuses thread
functions (e.g., calling another thread function before
thread_libinit, calling thread_libinit more than once, misusing
monitors, a thread that tries to acquire a lock it already has or
release a lock it doesn't have, etc.). For error conditions the thread
function should detect the error and return -1 to the user program (it
should not print any error messages). User programs can then detect
the error and retry or exit.

There are certain behaviors that are arguably errors or not. Here is a
list of questionable behaviors that should NOT be considered errors:
signaling without holding the lock (this is explicitly NOT an error in
Mesa monitors); deadlock (however, trying to acquire a lock by a
thread that already has the lock IS an error); a thread that exits
while still holding a lock (the thread should keep the lock). Ask on
piazza if you're unsure whether you should consider a certain behavior
an error.

# Scheduling order

Our thread library has specific rules about scheduling that are not
necessarily true of every thread library with similar functions.  Here
are the additional rules.

All scheduling queues should be FIFO. This includes the ready queue,
the queue of threads waiting for a monitor lock, and the queue of
threads waiting for a signal. Locks should be acquired by threads in
the order in which the locks are requested (by thread_lock() or in
thread_wait()).

When a thread calls thread_create, the caller does not yield the
CPU. The newly created thread is put on the ready queue but is not
executed right away.

When a thread calls thread_unlock, the caller does not yield the
CPU. The woken thread is put on the ready queue but is not executed
right away.

When a thread calls thread_signal or thread_broadcast, the caller does
not yield the CPU. The woken thread is put on the ready queue but is
not executed right away. The woken thread requests the lock when it
next runs.

# What's next?

This lab by itself is a little on the light side.  After your finish
this and get credit, we suggest you continue on to the next lab which
may take more than a week to do properly.  But having a nice suite of
test cases should help you!
