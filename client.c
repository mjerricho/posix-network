#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // read and write
#include <string.h>     // memset
#include <sys/socket.h> 
#include <sys/time.h> // gettimeofday
#include <arpa/inet.h>  // htonl, htons
#include <netdb.h> // gethostbyname

#define BUFFERSIZE 2048
static inline void fail(char *str) { fprintf(stderr, str); exit(1);}

int main(int argc, char **argv) {

  // Check arguments
  if (argc != 3) {
    fprintf(stderr, "usage: %s <server> <port>\n", argv[0]);
    exit(1);
  }
  
  // Collect and Process arguments
  struct hostent *he = gethostbyname(argv[1]);
  if (!he) {fail("The gethostbyname command failed\n");}
  unsigned short port = atoi(argv[2]);


  struct sockaddr_in serveraddr;
  memset(&serveraddr, 0,     sizeof(struct sockaddr_in));
  serveraddr.sin_family      = AF_INET;
  serveraddr.sin_port        = htons(port);
  serveraddr.sin_addr        = *((struct in_addr *)he->h_addr);
  
  // Initilize the socket 's'
  // TODO - NOT IMPLEMENTED

   // Connect the socket with its port/address to listen to
   // TODO - NOT IMPLEMENTED


  // Prepare the data to be sent
  char buffer[BUFFERSIZE] = "GET\n";
  fprintf(stderr, "Will send %s\n", buffer);
  

  // Send the data
  // TODO - NOT IMPLEMENTED
  
  // Receive the first chunk of data
  // TODO - NOT IMPLEMENTED



  
  // Close the connection
  // TODO - NOT IMPLEMENTED
}
