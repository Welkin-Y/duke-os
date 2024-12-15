#include <stdlib.h>
#include <stdio.h>

typedef struct metadata {
       /* size_t is the return type of the sizeof operator. Since the size of
 	* an object depends on the architecture and its implementation, size_t 
	* is used to represent the maximum size of any object in the particular
 	* implementation. 
	* size contains the size of the data object or the amount of free
 	* bytes 
	*/
	size_t size;
	struct metadata* next;
	struct metadata* prev; //What's the use of prev pointer?
} metadata_t;

char freelist[4*1024];

int 
main()
{
  void *base = (void*)freelist;
  void *freespace = base + sizeof(metadata_t);
  int *firstword = (int *)freespace;

  *firstword = 0xdeadbeef;

  long *contents = (long*)freelist;
  printf("0x%8lx\n", contents[0]);
  printf("0x%8lx\n", contents[1]);
  printf("0x%8lx\n", contents[2]);
  printf("0x%8lx\n", contents[3]);

}
