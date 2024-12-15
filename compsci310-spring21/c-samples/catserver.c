#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/wait.h>

char* port_arg;
int sock = 0;

void forkme(int s, char* prog, char* argv[]);

int main(int argc, char* argv[]) {
	int port;
	char* prog = "/bin/cat";

	if (argc < 2 || (port = atoi(argv[1])) < 1 || port > 65535) {
	  printf("Usage: %s port\n", argv[0]);
		exit(1);
	}
	if (argc > 2) {
	  prog = argv[2];
	}

	argc--; argv++;
	argc--; argv++;

	port_arg = (char*) malloc(6*sizeof (char));
	memset(port_arg, '\0', 6);
	strncpy(port_arg, argv[1], 5);

	struct sockaddr_in socket_addr;
	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&socket_addr, 0, sizeof socket_addr);
	socket_addr.sin_family = PF_INET;
	socket_addr.sin_port = htons(port);
	socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr *) &socket_addr, sizeof socket_addr) < 0) {
		perror("bind failed");
		exit(1);
	}
	listen(sock, 10);

	while (1) {
		int acceptsock = accept(sock, NULL, NULL);
		forkme(acceptsock, prog, argv);
		/*		shutdown(acceptsock, SHUT_RDWR);*/
		close(acceptsock);
	}

	return 0;
}

void
forkme(int s, char* prog, char* argv[])
{
  int status;
  int rc;
  rc = fork();
  if (rc < 0) {
    perror("fork failed: ");
    exit(1);
  } else if (rc == 0) {
    printf("I am a child: %d.\n", getpid());
    dup2(s, 0);
    dup2(s, 1);
    dup2(s, 2);
    execve(prog, argv, 0);
    /* NOTREACHED */
    perror("execve failed");
    exit(1);
  } else {
    if (waitpid(rc, &status, 0) == -1)
      perror("waitpid failed");
    else
      printf("Child %d exited with status %d.\n", rc,
	   WEXITSTATUS(status));
  }
}
