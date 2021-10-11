#include <stdio.h>
#include <unistd.h>
int main()
{
        int fd[2];
        int pid, status;

        pipe(fd);
        if((pid= fork()) == 0) {
                dup2( fd[0], 0);
                close(fd[1]);
                execlp("tee", "tee", NULL);
        }
        if((pid= fork()) == 0) {
                close(fd[0]);
                dup2( fd[1], 1);
                execlp("ls", "ls", "-l", NULL);
        }
        return(0);
}
