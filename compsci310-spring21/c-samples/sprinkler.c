#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

/*
  This program creates/truncates the specified file.  Then, every second it writes a 'c' at
  a random offset in the file, forever.
 */

int 
main(int argc, char* argv[])
{    
    if (argc < 2) {
      fprintf(stderr, "Usage: %s <file>\n", argv[0]);
      exit(1);
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)  {
      perror("open failed");
      exit(1);
    }

    char c = 'c';
    while(1) {
        off_t offset = (off_t)rand();
        off_t seekoff = lseek(fd, offset, SEEK_SET);
        if (seekoff != offset) {
                perror("lseek failed");
                exit(1);
        }
	if (write(fd, &c, 1) < 1) {
	  perror("write failed");
	  exit(1);
	}
	sleep(1);
    }
}

/*
Watch the size grow with bash line:
while true; do du zot; sleep 1; done
*/
