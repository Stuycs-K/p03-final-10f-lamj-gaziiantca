#include "networking.h"

int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }

  struct addrinfo *hints, *server_info;
  int sockfd;

  hints = (struct addrinfo*) calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;      // IPv4
  hints->ai_socktype = SOCK_DGRAM; // UDP

  if (getaddrinfo(IP, PORT, hints, &server_info) != 0) {
    perror("getaddrinfo");
    return 1;
  }

  sockfd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
  if (sockfd < 0) {
    perror("socket");
    return 1;
  }

  char *message = "I always come back...";
  printf("Sending message to %s:%s...\n", IP, PORT);
  
  int bytes_sent = sendto(sockfd, message, strlen(message), 0, server_info->ai_addr, server_info->ai_addrlen);

  if (bytes_sent < 0) {
    perror("sendto");
  } else {
    printf("Successfully sent %d bytes.\n", bytes_sent);
  }

  freeaddrinfo(server_info);
  close(sockfd);

  return 0;
}