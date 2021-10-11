#include <in.h>
#include <socket.h>
#include <netdb.h>
#include <stdio.h>

main()
{
   int s, namelen, client_address_size;
   struct sockaddr_in client, server;
   char buf[32];

   /*
    * Create a datagram socket in the internet domain and use the
    * default protocol (UDP).
    */
   if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       tcperror("socket()");
       exit(1);
   }

   /*
    * Bind my name to this socket so that clients on the network can
    * send me messages. (This allows the operating system to demultiplex
    * messages and get them to the correct server)
    *
    * Set up the server name. The internet address is specified as the
    * wildcard INADDR_ANY so that the server can get messages from any
    * of the physical internet connections on this host. (Otherwise we
    * would limit the server to messages from only one network
    * interface.)
    */
   server.sin_family      = AF_INET;  /* Server is in Internet Domain */
   server.sin_port        = 0;         /* Use any available port      */
   server.sin_addr.s_addr = INADDR_ANY;/* Server's Internet Address   */
k
   if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
       tcperror("bind()");
       exit(2);
   }

   /* Find out what port was really assigned and print it */
   namelen = sizeof(server);
   if (getsockname(s, (struct sockaddr *) &server, &namelen) < 0)
   {
       tcperror("getsockname()");
       exit(3);
   }

   printf("Port assigned is %d\n", ntohs(server.sin_port));

   /*
    * Receive a message on socket s in buf  of maximum size 32
    * from a client. Because the last two paramters
    * are not null, the name of the client will be placed into the
    * client data structure and the size of the client address will
    * be placed into client_address_size.
    */
   client_address_size = sizeof(client);

   if(recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &client,
            &client_address_size) <0)
   {
       tcperror("recvfrom()");
       exit(4);
   }
   /*
    * Print the message and the name of the client.
    * The domain should be the internet domain (AF_INET).
    * The port is received in network byte order, so we translate it to
    * host byte order before printing it.
    * The internet address is received as 32 bits in network byte order
    * so we use a utility that converts it to a string printed in
    * dotted decimal format for readability.
    */
   printf("Received message %s from domain %s port %d internet\
 address %s\n",
       buf,
       (client.sin_family == AF_INET?"AF_INET":"UNKNOWN"),
       ntohs(client.sin_port),
       inet_ntoa(client.sin_addr));

   /*
    * Deallocate the socket.
    */
   close(s);
}