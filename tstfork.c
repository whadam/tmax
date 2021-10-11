#include <stdio.h>
#include <unistd.h>
int main()
{
	int pid;

	printf("Before fork. My process ID = %d\n", getpid());
	pid = fork();
	if( pid == 0 )
		printf("I'm child. My PID = %d, Parent PID=%d\n",
			getpid(), getppid());
	else {
		printf("I'm parent. PID of My child = %d\n", pid); 
		sleep(1);
	}
	return(0);
}
