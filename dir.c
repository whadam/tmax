#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
int main(int argc, char **argv)
{
	DIR *dir_ptr = NULL;
	struct dirent *file = NULL;
	char home[256];

	getcwd(home, sizeof(home));
	if((dir_ptr = opendir(home)) == NULL) { 
		fprintf(stderr, "%s open failed\n", home);
		exit(-1); 
	}
	while((file = readdir(dir_ptr)) != NULL)
		printf("%s\n", file->d_name);
	closedir(dir_ptr);
	return 0;
}
