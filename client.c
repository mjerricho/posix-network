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
  int s = socket ( AF_INET, SOCK_STREAM, 0);  

  // Connect the socket with its port/address to listen to
  if (connect (s, (struct sockaddr *) & serveraddr, sizeof (serveraddr)) < 0) {
    fail("connect failed");
  }

  while(1) {
    // Prepare the data to be sent
    char buffer[BUFFERSIZE] = "GET\n";
    fprintf(stderr, "Input: ");
    fprintf(stderr, "\n");
    fscanf(stdin, "%s", buffer);
    if (buffer[0] == 'q' && strlen(buffer) == 1) break;
    
    fprintf(stderr, "Will send %s\n", buffer);

    struct timeval begin, end;
    gettimeofday(&begin, 0);

    // Send the data (read and write)
    int sent = write(s, buffer, strlen(buffer));
    fprintf(stderr, "Sent %d\n", sent);
  
    // Receive the first chunk of data
    int received = read(s, buffer, BUFFERSIZE-1);
    buffer[received] = 0;

    gettimeofday(&end, 0);
    double elapsed = 1000*(end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec)*1e-3;
    fprintf(stderr, "Received, %d\n", received);
    fprintf(stderr, "Inside the buffer: %s\n", buffer);
    fprintf(stderr, "Elapsed time was %f msec\n", elapsed);
  }
  
  // Close the connection
  close(s);
}
