#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#define DEFAULTPORT 8080
#define PICTURE "flag.jpg"

int main(argc, argv)
int argc;
char *argv[];
{
	int port = 0;
	int serverSocket;
	struct sockaddr_in addr, caddr;
	char *p;

	if(argc == 2)
		port = atoi(argv[1]);
	if(argc < 2 || port < 1024 )
		port = DEFAULTPORT;
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(serverSocket,(struct sockaddr *) &addr , sizeof(addr)) == -1) {
		perror("bind");
		exit(-1);
	}
	if(listen(serverSocket, 5) == -1) {
		perror("Listen failed");
		return -1;
	}
	printf("Server started listenting on port %d...\n", port);

	while(1){
		int	fd, len;
		char buf[256];
		FILE *stream;
		struct stat sbuf;

		len = sizeof(caddr);
		fd = accept(serverSocket, (struct sockaddr *)&caddr, &len);
		if(fd < 0) {
			perror("Accept failed");
			break;
		}
		if (stat(PICTURE, &sbuf) < 0) {
		   perror("stat err");
		   break;
		}
		/* Just send Korean flag image */
		if ((stream = fdopen(fd, "r+")) == NULL) {
		    perror("ERROR on fdopen");
		    continue;
		}
		fprintf(stream, "HTTP/1.1 200 OK\n");
		fprintf(stream, "Server: Tiny Web Server\n");
		fprintf(stream, "Content-length: %d\n", (int)sbuf.st_size);
		fprintf(stream, "Content-type: %s\n", "image/jpg");
		fprintf(stream, "\r\n");
		fflush(stream);
		/* Use mmap to return arbitrary-sized response body */
		fd = open(PICTURE, O_RDONLY);
		p = mmap(0, sbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		fwrite(p, 1, sbuf.st_size, stream);
		munmap(p, sbuf.st_size);
		while( 1 ) {
			if((len = recv(fd, buf, sizeof(buf),0)) <= 0) {
				fprintf(stderr, "Conn closed\n");
				break;
			}
			buf[len] = '\0';
			printf("%s",buf);
		}
		printf("Loop for the next call..\n");
	}
	return(0);
}
