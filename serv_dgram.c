// Server program using socket DGRAM
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8080
int main(int argc, char const *argv[])
{
    struct sockaddr_in server, client;
    int server_fd, len;
    int addrlen = sizeof(server);
    char buf[256];
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if( getsockname(server_fd, (struct sockaddr *)&server, &len) < 0) {
	perror("getsockname");
	exit(-1);
    }
    addrlen = sizeof(client);
    for( ; ; ) {
	    if((len = recvfrom(server_fd, buf, sizeof(buf), 0, 
		(struct sockaddr *)&client, &addrlen)) < 0) {
		perror("recvfrom");
		exit(-1);
	    }
	    buf[len] = '\0';
	    printf("Msg %s from port %d IP %s\n", buf, ntohs(client.sin_port),
		inet_ntoa(client.sin_addr));
    }
    close(server_fd);
    return 0;
}
