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

int clientCount = 0;
struct client {

	int index;
	int sockID;
	struct sockaddr_in clientAddr;
	int len;

};
struct client Client[1024];
pthread_t thread[1024];

void * doNetworking(void * ClientDetail)
{
	struct client * clientDetail = (struct client*) ClientDetail;
	int index = clientDetail -> index;
	int clientSocket = clientDetail -> sockID;

	printf("Client %d connected.\n",index + 1);
	while(1) {
		int len;
		char data[1024];
		if((len = recv(clientSocket,data,1024,0)) <= 0)
			break;;
		data[len] = '\0';
		char output[1024];
		if(strcmp(data,"LIST") == 0){
			int l = 0;

			for(int i = 0 ; i < clientCount ; i ++){

				if(i != index)
					l += snprintf(output + l,1024,"Client %d is at socket %d.\n",i + 1,Client[i].sockID);
			}
			send(clientSocket,output,strlen(output),0);
			continue;
		}
		printf("Clnt%d: %s\n", index + 1, data);
	}
	return NULL;
}

int main(){
	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);


	if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) {
		perror("bind");
		exit(-1);
	}
	if(listen(serverSocket,1024) == -1) return -1;
	printf("Server started listenting on port 8080 ...........\n");

	while(1){
		Client[clientCount].sockID = accept(serverSocket, (struct sockaddr*) &Client[clientCount].clientAddr, &Client[clientCount].len);
		Client[clientCount].index = clientCount;
		pthread_create(&thread[clientCount], NULL, doNetworking, (void *) &Client[clientCount]);
		clientCount ++;
	}
	for(int i = 0 ; i < clientCount ; i ++)
		pthread_join(thread[i],NULL);
}
