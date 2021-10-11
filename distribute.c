#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
	int i, from, to, load = 1024 * 1024;
	int parent, status;

	parent = getpid();
	for(i = from = 0; i < 6; i++, from += load) {
		to = from + load;
		if( fork() == 0 )
			break;
	}
	if( getpid() == parent )	/* reap children */
		while( wait( &status) != -1)
			;
	else
		printf("Child %d: My job is from offset %d to %d\n", i, from, to - 1);
	return(0);
}
