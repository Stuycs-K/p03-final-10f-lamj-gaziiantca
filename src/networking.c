#include "networking.h"
#include "types.h"
#include "engine_clock.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>

// Create a socket
int setupUDP_Server() {
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


int setupUDP_Client(char* IP, struct addrinfo** returnedResult, hdNetworkQueue* networkQueue) {
	struct addrinfo *results, *hints;
	hints = (struct addrinfo*) calloc(1, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_DGRAM;
	err(getaddrinfo(IP, PORT, hints, &results), "getaddrinfo()");


	int sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	err(sockfd, "Socket Error");

	free(hints);

	*returnedResult = results;
	if(networkQueue != NULL){
		networkQueue->servaddr = results->ai_addr;
		networkQueue->addr_len = results->ai_addrlen;
		networkQueue->sockfd = sockfd;
	}

	return sockfd;
}

hdNetworkQueue* initializeNetworkQueue(){
	hdNetworkQueue* out = (hdNetworkQueue*)(calloc(sizeof(hdNetworkQueue), 1));
	//printf("%p %lu\n", out->items, sizeof(*out));
	return out;
}

int sendMessage(int sockfd, void* data, u64 data_size, struct sockaddr* servaddr, u64 addr_len){
	/*char* debug = (char*)(malloc(data_size));
	strncpy(debug, data, data_size);
	debug[126] = 0;
	printf("Sending %s\n", debug);*/
	int bytes = sendto(sockfd, data, data_size, 0, servaddr, addr_len);
	if(bytes < 0) printf("Sussy error when sending a message through socket %d\n", sockfd);
	return bytes;
}

//Add a packet to the queue. Note: this WILL make a copy of the packet and it WILL free the original packet's pointer and you WILL NOT be able to reliably edit it. 
void QueueReliableNetworkMessage(hdNetworkQueue* queue, hdPacket* packet){
	u16 index = (queue->count++); 
	packet->pos = index;
	packet->isreal = 1;
	queue->items[index%256] = *packet;
	free(packet); //beamed by simple origin 
}

void receiveReliableAck(hdNetworkQueue* queue, u16 pos){
	queue->items[pos%256].isreal = false; //this will absolutely cause random errors that are not the user's fault :trollface:
}

int loopNetworkQueue(hdNetworkQueue* queue){
	u64 time = getTime(); //surely doing it like this won't cause anything stupid
	int bytes = 0;
	for(int i=0; i<256; i++){
		if(queue->items[i].isreal && time-queue->items[i].time_sent > 200){
			//printf("a\n");
			bytes += sendMessage(queue->sockfd, queue->items[i].data, queue->items[i].data_size, queue->servaddr, queue->addr_len);
			queue->items[i].time_sent = time;
		}
	}
	return bytes;
}

hdPacket* createReliablePacket(void* data, u64 data_size){
	hdPacket* out = (hdPacket*) (calloc(sizeof(hdPacket), 1));
	out->data = data;
	out->data_size = data_size;
	return out;
}

static void err(int i, char*message){
	if(i < 0){
		printf("Error: %s - %s\n",message, strerror(errno));
		exit(1);
	}
}
