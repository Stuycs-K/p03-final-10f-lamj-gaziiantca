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
#include "types.h"

#ifndef NETWORKING_H
	
  #define NETWORKING_H
  #define PORT "24290"
	#define PORTNUM 24290
  #define BUFFER_SIZE 1024
  static void err(int i, char*message);

  int setupUDP_Server(hdNetwork* network);
  int setupUDP_Client(char* IP, struct addrinfo** returnedResult, hdNetwork* optionalNetworkQueue);
	//I love tab indents
	hdNetwork* initializeNetworkQueue();
	int sendMessage(int sockfd, void* data, u64 data_size, struct sockaddr* servaddr, u64 addr_len); //most useless abstraction of the decade award
	
	void QueueReliableNetworkMessage(hdNetwork* queue, hdPacket* packet);
	void handleAck(hdNetwork* queue, i16 pos);
	int loopNetworkQueue(hdNetwork* queue);
	hdPacket* createPacket(void* data, u64 data_size);
	void Server_receiveData(hdNetwork* network, hdPacket* buffer);
	void Client_receiveData(hdNetwork* network, hdPacket* buffer);
	
	void Server_getClient(hdNetwork* network, socklen_t socklen);
	void Server_broadcastData(hdNetwork* network, hdPacket* buffer);
	

#endif
