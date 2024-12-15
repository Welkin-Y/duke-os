#include <unistd.h> // execlp()
#include <stdio.h>  // perror()
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE

int main(void) {
    /* execvp 
       
       This form always takes 2 arguments, but the 2nd argument is a
       NULL terminated array of arguments.  Because building the array
       is an extra step, this form is usually annoying unless you need
       to be able to vary how many arguments you pass programmatically
       (i.e. you don't know how many args there will be when you write
       the exec command). */
    
   
    char *const command2[] = {"ls","-l","-h", NULL};
    execvp("ls",command2);

    
    /* 
       NO MATTER WHICH FORM YOU USE

       1. You must always pass the executable name twice.  The first
       time is what to execute, the second time is what you're going
       to tell the executing program its first parameter is (first
       parameter is always the name of the executable).

       2. NULL must always be the last parameter.  Otherwise your exec
       variant can't know where the arguments stop.
    
    */
    
    perror("Return from exec() not expected");
    // ^^ this should never print because exec *replaces* the process that runs it
    exit(EXIT_FAILURE);
}

