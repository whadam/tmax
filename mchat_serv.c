#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define MAXCLIENT 256	/* Max number of clients joinable */
#define DEFAULTPORT 8080

int num_client = 0;
struct client_info {
	int index;
	int sock_fd;
	struct sockaddr_in clientAddr;
	int len;
	char nickname[20];
};
struct client_info client[MAXCLIENT];
pthread_t thread[MAXCLIENT];

void * do_chat(void * clnt)
{
	struct client_info * clnt_info = (struct client_info *) clnt;
	int state;
	int index = clnt_info -> index;
	int clientsocket = clnt_info -> sock_fd;

	while(1) {
		int len;
		char buf[256], msg[300];

		if((len = recv(clientsocket,buf,sizeof(buf),0)) <= 0)
			break; /* This client is not available anymore */
		buf[len] = '\0';
		/* Broadcast received msg to other clients */
		sprintf(msg, "%s> %s", clnt_info->nickname, buf);
		for(int i = 0 ; i < num_client ; i++)
			if(i != index && client[i].sock_fd > 0)
			  send(client[i].sock_fd, msg, strlen(msg),0);
	}
	clnt_info -> sock_fd = -1;	/* disconnected client. Mark it */
	return NULL;
}

int main(argc, argv)
int argc;
char *argv[];
{
	int port = 0;
	int serverSocket;
	struct sockaddr_in serverAddr;

	if(argc > 1 )
		port = atoi(argv[1]);
	if(port < 1024 )
		port = DEFAULTPORT;
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);


	if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) {
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

		fd = accept(serverSocket, (struct sockaddr*) &client[num_client].clientAddr, &client[num_client].len);
		if(fd < 0) {
			perror("Accept failed");
			break;
		}
		if((len = recv(fd, buf, sizeof(buf),0)) <= 0)
			continue; /* This client is not available anymore */
		buf[len] = '\0';
		printf("client %s accepted by server.\n",buf);
		/* The first msg is client's nickname */
		strcpy(client[num_client].nickname, buf);
		client[num_client].sock_fd = fd;
		client[num_client].index = num_client;
		/* Let thread do the remaining job */
		pthread_create(&thread[num_client], NULL, do_chat, (void *) &client[num_client]);
		num_client ++;
	}
	for(int i = 0 ; i < num_client ; i ++)
		pthread_join(thread[i],NULL);
	return(0);
}
