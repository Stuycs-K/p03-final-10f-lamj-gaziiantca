#include "networking.h"

int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }

  struct addrinfo *results;
  int sockfd = setupUDP(IP, &results);

  char* message = "I always come back...";
  printf("Sending message to %s:%s...\n", IP, PORT);

  int bytes_sent = sendto(sockfd, message, strlen(message), 0, results->ai_addr, results->ai_addrlen);

  err(bytes_sent, "sendto()");
  printf("Successfully sent %d bytes.\n", bytes_sent);

  freeaddrinfo(results);
  close(sockfd);

  return 0;
}
