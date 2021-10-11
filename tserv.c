#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define DEFAULTPORT 8080

int main(argc, argv)
int argc;
char *argv[];
{
	int port = 0;
	int serverSocket;
	struct sockaddr_in addr, caddr;

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

		len = sizeof(caddr);
		fd = accept(serverSocket, (struct sockaddr *)&caddr, &len);
		if(fd < 0) {
			perror("Accept failed");
			break;
		}
		while( 1 ) {
			if((len = recv(fd, buf, sizeof(buf),0)) <= 0) {
				fprintf(stderr, "Conn closed\n");
				break; /* This client is not available anymore */
			}
			buf[len] = '\0';
			printf("%s",buf);
		}
		printf("Loop for the next call..\n");
	}
	return(0);
}

