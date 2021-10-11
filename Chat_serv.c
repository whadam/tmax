#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

// Max number of connections allowed = 1024 .....

struct connection{

	struct sockaddr_in clientAddr;
	int len;

};

struct clientDetail{

	int index;
	int sockID;

};

int clientNum = 0;

int val1, val2, op;

struct connection client[1024];
struct clientDetail ClientDetail[1024];
pthread_t thread[1024];

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void * doNetworking(void* clientDet){

	struct clientDetail* clientSock = (struct clientDetail *) clientDet;
	int index = clientSock -> index;
	int clientSocket = clientSock -> sockID;

	/*char name[64];
	inet_ntop(AF_INET, &(client[index].clientAddr.sin_addr), name, (client[index].len));*/

	pthread_cond_wait(&cond,&mtx); // while(clientNum != 3);

	printf("Client %d connected to server.\n",index + 1);

	char data[1024];

	if(index == 0)
		snprintf(data,1024,"Ready\nEnter first number: ");
	if(index == 1)
		snprintf(data,1024,"Ready\nEnter second number: ");
	if(index == 2)
		snprintf(data,1024,"Ready\nEnter operator: ");

	send(clientSocket,data,1024,0);

	pthread_mutex_unlock(&mtx);
	pthread_cond_broadcast(&cond);

	char input[1024];
	int read = recv(clientSocket,input,1024,0);
	input[read] = '\0';
	printf("%s\n",input);

	if(index == 0)
		val1 = atoi(input);
	if(index == 1)
		val2 = atoi(input);
	if(index == 2)
		op = input[0];

	return NULL;

}

int main(){

	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(serverSocket == -1) return 0;

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(serverSocket, (struct sockaddr*) &serverAddr , sizeof(serverAddr)) == -1) return 0;

	if(listen(serverSocket,1024) == 0){

		printf("Server listening on port 8080 ...........\n");

		while(1){

			unsigned int len;

			int clientSocket = accept(serverSocket, (struct sockaddr*) &(client[clientNum].clientAddr) , &(client[clientNum].len));

			ClientDetail[clientNum].index = clientNum;
			ClientDetail[clientNum].sockID = clientSocket;

			pthread_create(&thread[clientNum], NULL, doNetworking , (void *) &(ClientDetail[clientNum]));

			clientNum ++;

			if(clientNum == 3)
				break;

		}

		pthread_cond_broadcast(&cond);

		for(int i = 0 ; i < 3 ; i ++){
			pthread_join(thread[i],NULL);
		}

		int result;

		if(op == '+')
			result = val1 + val2;
		if(op == '-')
			result = val1 - val2;
		if(op == '*')
			result = val1 * val2;
		if(op == '/')
			result = val1 / val2;

		char output[1024];

		snprintf(output,1024,"%d",result);

		for(int i = 0 ; i < clientNum ; i ++){

			send(ClientDetail[i].sockID,output,1024,0);

		}

	}

}