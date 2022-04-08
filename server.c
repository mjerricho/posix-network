#include <stdio.h>
#include <unistd.h>     // read and write
#include <stdlib.h>
#include <string.h>     // memset
#include <sys/socket.h> 
#include <arpa/inet.h>  // htonl, htons
#include <pthread.h>

#define BUFFERSIZE 2048
static inline void error(char *str) { perror(str); exit(1);}
// Max 100 connections
static int connection_arr[100];
static size_t head = 0;

void *handler(void *arg) {
  int initialized_connection = *((int *) arg);
  fprintf(stderr, "Processing current connection...\n");

  int this_connection_makes_sense = 1;

  while (this_connection_makes_sense) {
      
    char buffer[BUFFERSIZE];
      
    int n = read(initialized_connection, buffer, BUFFERSIZE);
    if (n <= 0) {
      this_connection_makes_sense = 0; continue;
      fprintf(stderr, "server failed receiving data\n");	  
    } else {
      buffer[n] = 0;
    }   
    fprintf(stderr, "server received %d bytes: %s\n", n, buffer);

    char response [20];
    int code = 0;
    for (int i = 0 ; buffer[i] ; code += buffer[i++]);
      
    sprintf(response, "%d", code);

    n = write(initialized_connection, response, strlen(response));
    if (n < 0) {
      this_connection_makes_sense = 0; continue;
      fprintf(stderr, "server failed sending data\n");
    } 
    fprintf(stderr, "server sent %d bytes: %s\n", n, response);         
  }
}


int main(int argc, char **argv) {

  // Check arguments (port number)
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  // Define variables used later
  int port = atoi(argv[1]);
  
  struct sockaddr_in serveraddr;
  memset(&serveraddr, 0, sizeof(struct sockaddr_in));
  serveraddr.sin_family      = AF_INET;
  serveraddr.sin_port        = htons((unsigned short)port);
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
  // Initilize the socket
  int opened_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (opened_socket < 0) {
    error("Socket failed");
  }

  // Bind the socket with its port/address to listen to
  if (bind(opened_socket, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
    error("Binding failed");
  }

  // Starts to listen (two at a time)
  if (listen(opened_socket, 2) < 0) {
    error("Listen failed");
  }

  int KEEP_WORKING = 1;
  while (KEEP_WORKING) {
    fprintf(stderr, "Waiting for a new connection...");
    int initialized_connection = accept(opened_socket, NULL, NULL);
    
    if (initialized_connection < 0) {
      error("Accept failed");
    } else {      
      fprintf(stderr, "Done!\n");

      // create new thread
      fprintf(stderr, "Creating thread\n");
      pthread_t p;
      pthread_create(&p, NULL, handler, &initialized_connection);
    }
    connection_arr[head] = initialized_connection;
    head++;
  }
  fprintf(stderr, "Closing connection");
  for (size_t i = 0; i < head; ++i) {
    fprintf(stderr, "Closing connection: %d\n", connection_arr[i]);
    close(connection_arr[i]);
  }
}
