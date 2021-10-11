#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define DEFAULTPORT 8080

void * recv_msg(void * sock)
{
	int sock_fd = *((int *) sock);

	/* receive message from server and display it */
	while(1) {
		char buf[256];

		// receive msg from server
		int cnt = recv(sock_fd, buf, sizeof(buf), 0);
		if( cnt <= 0 ) /* server may be down */
			break;
		buf[cnt] = '\0';
		/* Let's use terminal escape sequence to make the msg
		   stand out. */
		fprintf(stderr, "\x1b[33;40m%s\x1b[0m", buf);
	}
}

int main(int argc, char *argv[])
{
	int rv, port = 0;
	pthread_t thread;
	struct sockaddr_in svr_addr;
	int sock_fd;
	char *svr_ip = "127.0.0.1"; /* default: local loop */
	char *nickname;	/* nickname to use */

	if( argc < 2 ) {
		fprintf(stderr, "Usage: %s nickname [hostIP port#]\n", argv[0]);
		exit(-1);
	}
	nickname = argv[1];
	if(argc > 3) {
		svr_ip = argv[2];
		port = atoi(argv[3]);
	}
	if( port < 1024 )	/* Not available port number */
		port = DEFAULTPORT;
	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons(port);
	svr_addr.sin_addr.s_addr = inet_addr(svr_ip);
	if(svr_addr.sin_addr.s_addr == (in_addr_t) -1) {
		fprintf(stderr, "Bad addr %s. It should be like 192.168.56.101\n", svr_ip);
		exit(-1);
	}

	rv = connect(sock_fd, (struct sockaddr*) &svr_addr, sizeof(svr_addr));
	if(rv == -1) {
		perror("connect failed");
		return -1;
	}
	printf("Connection request OK..Enter 'quit' to end conversation\n");
	/* Let the server know my nickname */
	if( send(sock_fd, nickname, strlen(nickname), 0) <= 0) {
		perror("send failed");
		exit(-1);
	}

	/* Let thread do receiving msg from server */
	rv=pthread_create(&thread, NULL, recv_msg, (void *) &sock_fd );
	if( rv != 0 ) {
		perror("Thread creation failed");
		return(-1);
	}

	/* And below code gets msg from user and send it to the server */
	while(1) {
		char buf[256];

		printf(">");	/* prompt */
		if( fgets(buf, sizeof(buf), stdin) == NULL)
			break;	/* EOF or error case */
		if( strlen(buf) == 5 && strncmp(buf, "quit", 4) == 0)
			break;
		if( send(sock_fd, buf, strlen(buf), 0) <= 0) {
			perror("send failed");
			break;
		}
	}
	close( sock_fd );
	return(0);
}
