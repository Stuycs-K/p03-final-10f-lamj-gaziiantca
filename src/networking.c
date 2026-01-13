#include "networking.h"
#include <sys/socket.h>

// Create a socket
int setupUDP() {
  //setup structs for getaddrinfo//use getaddrinfo
  struct addrinfo *results, *hints;

  hints = (struct addrinfo*) calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_DGRAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  err(getaddrinfo(NULL, PORT, hints, &results), "getaddrinfo()");

  //create the socket
  //store the socket descriptor here
  int clientd = socket(AF_INET, SOCK_DGRAM, 0);

  //this code should get around the address in use error
  int yes = 1;
  int sockOpt = setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt, "Sockopt Error");

  //bind the socket to address and port
  err(bind(clientd, results->ai_addr, results->ai_addrlen), "Bind()");

  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);

  return clientd;
}

int setupUDP_Client(char* IP, struct addrinfo** returnedResult) {
  struct addrinfo *results, *hints;
  hints = (struct addrinfo*) calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_DGRAM;
  err(getaddrinfo(IP, PORT, hints, &results), "getaddrinfo()");


  int sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  err(sockfd, "Socket Error");

  free(hints);

  *returnedResult = results;

  return sockfd;
}

void err(int i, char*message){
  if(i < 0){
	  printf("Error: %s - %s\n",message, strerror(errno));
  	exit(1);
  }
}
