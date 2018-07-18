// #include <manifest.h>
// #include <bsdtypes.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
void error(char *msg)
{
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[])
{
  int s, namelen, news, client_address_size, portno;
  struct sockaddr_in client, server;
  char buf[2000];

  struct timespec ts,tp;    
  char buff1[1000], buff2[1000];

   /*
    * Create a datagram socket in the internet domain and use the
    * default protocol (UDP).
    */

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
   if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
       error("socket()");
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

  portno = atoi(argv[1]);

  server.sin_family      = AF_INET;  /* Server is in Internet Domain */
  server.sin_port        = htons(portno);         /* Use any available port      */
  server.sin_addr.s_addr = INADDR_ANY;/* Server's Internet Address   */

  if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
      error("bind()");
      exit(2);
  }

   /* Find out what port was really assigned and print it */
   namelen = sizeof(server);
   if (getsockname(s, (struct sockaddr *) &server, &namelen) < 0)
   {
       error("getsockname()");
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

   while(1){
     if(recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &client,
              &client_address_size) <0)
     {
         perror("recvfrom()");
         exit(4);
     }
    timespec_get(&ts, TIME_UTC);

    strftime(buff1, sizeof buff1, "%D %T", gmtime(&ts.tv_sec));
    sprintf(buff2, "\nServer Time Received: %s.%09ld UTC\n", buff1, ts.tv_nsec);

     /*
      * Print the message and the name of the client.
      * The domain should be the internet domain (AF_INET).
      * The port is received in network byte order, so we translate it to
      * host byte order before printing it.
      * The internet address is received as 32 bits in network byte order
      * so we use a utility that converts it to a string printed in
      * dotted decimal format for readability.
      */
      /*
    printf("\nReceived message %s from domain %s port %d internet\
        address %s\n",
        buf,
        (client.sin_family == AF_INET?"AF_INET":"UNKNOWN"),
        ntohs(client.sin_port),
        inet_ntoa(client.sin_addr));*/
        printf("\nReceived message %s \n",
        buf);
    bzero(buf, strlen(buf));
}

/*  strcpy(buf, "Hello");

  timespec_get(&ts, TIME_UTC);
  strftime(buff1, sizeof buff1, "%D %T", gmtime(&ts.tv_sec));

  sprintf(buff2 + strlen(buff2), "\nServer Time sent: %s.%09ld UTC\n", buff1, ts.tv_nsec);

    Send the message in buf to the serv_addr 
   if (sendto(s, buff2, (strlen(buff2)+1), 0,
                 (struct sockaddr *)&client, client_address_size) < 0)
   {
       error("sendto()");
       exit(2);
   }

   printf("\n\n%s", buff2);*/

   /*
    * Deallocate the socket.
    */
   close(s);
}
