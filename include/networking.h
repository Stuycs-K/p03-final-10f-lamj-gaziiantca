#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>


#ifndef NETWORKING_H
  #define NETWORKING_H
  #define PORT "24290"
  #define BUFFER_SIZE 1024
  void err(int i, char*message);

  int setupUDP();
  int setupUDP_Client(char* IP, struct addrinfo** returnedResult);
#endif
