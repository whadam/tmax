#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(){

	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(connect(clientSocket, (struct sockaddr *) &serverAddr , sizeof(serverAddr)) == 0){

		printf("Connection established .........\n");

		char data[1024];
		int read = recv(clientSocket,data,1024,0);
		data[read] = '\0';
		printf("%s\n",data);

		char input[1024];
		scanf("%s",input);
		send(clientSocket,input,1024,0);

		read = recv(clientSocket,data,1024,0);
		printf("%s\n",data);

	}

}