#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int status;
    printf("about to run program %s.\n", argv[1]);
    execve(argv[1], argv, 0);
    perror("exec failed");
}
