
/*** IBMCOPYR ********************************************************/
/*                                                                   */
/* Component Name: UDPC                                              */
/*                                                                   */
/*                                                                   */
/* Copyright:    Licensed Materials - Property of IBM                */
/*                                                                   */
/*               "Restricted Materials of IBM"                       */
/*                                                                   */
/*               5647-A01                                            */
/*                                                                   */
/*               (C) Copyright IBM Corp. 1977, 1998                  */
/*                                                                   */
/*               US Government Users Restricted Rights -             */
/*               Use, duplication or disclosure restricted by        */
/*               GSA ADP Schedule Contract with IBM Corp.            */
/*                                                                   */
/* Status:       CSV2R6                                              */
/*                                                                   */
/*  SMP/E Distribution Name: EZAEC020                                */
/*                                                                   */
/*** IBMCOPYR ********************************************************/

static char ibmcopyr[] =
   "UPDC     - Licensed Materials - Property of IBM. "
   "This module is \"Restricted Materials of IBM\" "
   "5647-A01 (C) Copyright IBM Corp. 1992, 1996. "
   "See IBM Copyright Instructions.";

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
#include <sys/time.h>
#include <sys/resource.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[])
// int argc;
// char **argv;
{

  int which = PRIO_PROCESS;
  id_t pid;
  int s, servlen;
  unsigned short port;
  struct sockaddr_in serv_addr;
  char buf[2000];
  struct hostent *server;
   pid = getpid();

  if(getpriority(which, pid) < 0){
      perror("getpriority Error");
      exit(EXIT_FAILURE);
  }
  printf("%d\n", pid);
 

  struct timeval ts;
  char buff1[1000], buff2[1000];

   /* argv[1] is internet address of serv_addr argv[2] is port of serv_addr.
    * Convert the port from ascii to integer and then from host byte
    * order to network byte order.
    */
  if(argc != 3)
  {
    printf("Usage: %s <host address> <port> \n",argv[0]);
    exit(1);
  }
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }

  port = htons(atoi(argv[2]));


  /* Create a datagram socket in the internet domain and use the
   * default protocol (UDP).
   */
  if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
      error("socket()");
      exit(1);
  }
  

   /* Set up the serv_addr name */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  bcopy((char *)server->h_addr, 
    (char *)&serv_addr.sin_addr.s_addr,
    server->h_length);
   serv_addr.sin_family      = AF_INET;            /* Internet Domain    */
   serv_addr.sin_port        = port;               /* serv_addr Port        */
   // serv_addr.sin_addr.s_addr = inet_addr(argv[1]); /* serv_addr's Address   */

  servlen = sizeof(serv_addr);


  strcpy(buf, "Hello");
  int n;
  int j;
  j = 0;
  int count;
  count = 0;
// if(setpriority(which, pid, -20) < 0){
  //    perror("setPriority Error");
    //  exit(EXIT_FAILURE);
//    }
while(j < 144){
  printf("%d \n", j);
  n = 0;
  while(n < 10000){
  gettimeofday(&ts,NULL);

  strftime(buff1, sizeof buff1, "%D %T", gmtime(&ts.tv_sec));
  sprintf(buff2, "\nClient Time Sent: %s.%09ld UTC\n", buff1, ts.tv_usec);
  sprintf(buf, "%014u", count);

//  strcat(buf, buff2);

  usleep(400);
   /* Send the message in buf to the serv_addr */
   if (sendto(s, buf, (strlen(buf)+1), 0,
                 (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
   {
       error("sendto()");
       exit(2);
   }

  printf("\n%s \t \t %d", buff2, n);
  bzero(buf, strlen(buf));
  n++;
  count++;
 }
 j++;
 sleep(600);
}
/*
   if(recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &serv_addr, &servlen)<0){
    error("recvfrom()");
    exit(4);
   }


  gettimeofday(&ts, NULL);
  strftime(buff1, sizeof buff1, "%D %T", gmtime(&ts.tv_sec));

  sprintf(buff2 + strlen(buff2), "\nClient Time Recieved: %s.%09ld UTC\n", buff1, ts.tv_usec);


   printf("\nRecieved message: \n%s \n\nfrom domain %s port %d internet\
    address %s\n", 
    buf, (serv_addr.sin_family ==AF_INET?"AF_INET":"UNKNOWN"), 
    ntohs(serv_addr.sin_port), inet_ntoa(serv_addr.sin_addr));
    //Deallocate the socket 

   printf("\n%s\n", buff2);*/
   close(s);
}
