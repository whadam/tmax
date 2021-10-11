// Client side Socket DGRAM programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080
   
int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in server;
    char buf[256];
    char *hello[] = { "Hello server!\n", "Second msg\n", NULL };

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
 // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr)<=0) {  /* use local host 127.0.0.1  */
        printf("Invalid address/ Address not supported\n");
        return -1;
    }
    for(int i=0; hello[i] != NULL; i++) {
	if(sendto(sock, hello[i], strlen(hello[i]), 0,
		(struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("sendto err");
	    	break;
	}
    	printf("Msg %d sendto succeeded\n", i);
	sleep(1);
    }
    return 0;
}
