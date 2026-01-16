#include "networking.h"
#include "types.h"
#include "engine_clock.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>

// Create a socket
int setupUDP_Server(hdNetwork* network) {
	//setup structs for getaddrinfo//use getaddrinfo
	struct addrinfo *results;
	struct addrinfo hints = {0};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM; //TCP socket
	hints.ai_flags = AI_PASSIVE; //only needed on server
	err(getaddrinfo(NULL, PORT, &hints, &results), "getaddrinfo()");

	//create the socket
	//store the socket descriptor here
	int clientd = socket(AF_INET, SOCK_DGRAM, 0);

	//this code should get around the address in use error
	int yes = 1;
	int sockOpt = setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	err(sockOpt, "Sockopt Error");

	//bind the socket to address and port
	err(bind(clientd, results->ai_addr, results->ai_addrlen), "Bind()");
	fcntl(clientd, F_SETFL, O_NONBLOCK);

	if(network != NULL){
		network->addr_len = results->ai_addrlen;
		network->sockfd = clientd;

		memcpy(&network->servaddr, results->ai_addr, results->ai_addrlen);
	}

	//free the structs used by getaddrinfo
	freeaddrinfo(results);

	return clientd;
}


int setupUDP_Client(char* IP, struct addrinfo** returnedResult, hdNetwork* networkQueue) {
	struct addrinfo *results;
	struct addrinfo hints = {0};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	err(getaddrinfo(IP, PORT, &hints, &results), "getaddrinfo()");


	int sockfd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	err(sockfd, "Socket Error");
	//fcntl(sockfd, F_SETFL, O_NONBLOCK);

	*returnedResult = results;
	if(networkQueue != NULL){
		networkQueue->addr_len = results->ai_addrlen;
		networkQueue->sockfd = sockfd;

		memcpy(&networkQueue->servaddr, results->ai_addr, results->ai_addrlen);
	}

	return sockfd;
}

hdNetwork* initializeNetworkQueue(){
	hdNetwork* out = (hdNetwork*)(calloc(sizeof(hdNetwork), 1));
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
void QueueReliableNetworkMessage(hdNetwork* queue, hdPacket* packet){
	u16 index = (queue->count++); 
	if(queue->count > 30000){
		queue->count %= 30000;
	}
	packet->pos = index;
	packet->isreal = 1;
	queue->items[index%256] = *packet;
	free(packet); //beamed by simple origin 
}

void handleAck(hdNetwork* queue, i16 pos){
	if(pos != -1){
		queue->items[pos%256].isreal = false; //this will absolutely cause random errors that are not the user's fault :trollface:
	}
}

int loopNetworkQueue(hdNetwork* queue){
	u64 time = getTime(); //surely doing it like this won't cause anything stupid
	int bytes = 0;
	for(int i=0; i<256; i++){
		if(queue->items[i].isreal && time-queue->items[i].time_sent > 10000){
			//printf("a\n");
			bytes += sendMessage(queue->sockfd, queue->items+i, sizeof(queue->items[i]), (struct sockaddr*) &queue->servaddr, queue->addr_len);
			queue->items[i].time_sent = time;
		}
	}
	return bytes;
}

hdPacket* createPacket(void* data, u64 data_size){ //will notably not be reliable unless put into the network queue
	hdPacket* out = (hdPacket*) (calloc(sizeof(hdPacket)+data_size, 1));
	memcpy(out->data, data, data_size); 
	//printf("creating packet with data %*.s\n", (int)data_size, (char*)data);
	out->data_size = data_size;
	out->pos = -1;
	return out;
}

void Server_receiveData(hdNetwork* network, hdPacket** ptrBuffer){ //will write a packet to the buffer. it will also free the old packet. EVERYTHING sent by the client MUST be in a packet. The packets are NOT optional. This is the reason
	int bytes; 
	hdPacket* buffer = *ptrBuffer;
	
	socklen_t bruh = network->addr_len; //this reeks
	bytes = recvfrom(network->sockfd, buffer, sizeof(hdPacket), MSG_PEEK, (struct sockaddr*) &network->servaddr, &bruh);
	if(bytes > 0){
		Server_getClient(network, bruh);
		u32 len = sizeof(hdPacket) + buffer->data_size;
		free(buffer);
		buffer = malloc(len);
		*ptrBuffer = buffer;
		bytes = recvfrom(network->sockfd, buffer, len, 0, (struct sockaddr*) &network->servaddr, &bruh); //genius code over here. haters cannot fathom.
		//printf("sending ack with pos=%d dat=%s\n", buffer->pos, (char*)buffer->data);
		
		//lowkey wasteful but I don't have the time to care anymore
		sendto(network->sockfd, buffer, sizeof(hdPacket) + buffer->data_size, 0, (struct sockaddr*) &network->servaddr, network->addr_len); 
	}
}

void Client_receiveData(hdNetwork* network, hdPacket** ptrBuffer){
	int bytes; 
	hdPacket* buffer = *ptrBuffer; 
	socklen_t IloveC = network->addr_len;
	bytes = recvfrom(network->sockfd, buffer, sizeof(hdPacket), MSG_PEEK, (struct sockaddr*) &network->servaddr, &IloveC);
	if(bytes > 0){
		u32 len = sizeof(hdPacket) + buffer->data_size;
		free(buffer); 
		buffer = malloc(len); 
		*ptrBuffer = buffer;
		bytes = recvfrom(network->sockfd, buffer, len, 0, (struct sockaddr*) &network->servaddr, &IloveC);
		//printf("%d\n", buffer->pos);
		handleAck(network, buffer->pos);
	}
}

void Client_sendData(hdNetwork* network, hdPacket* packet){
	sendMessage(network->sockfd, packet, packet->data_size + sizeof(hdPacket), (struct sockaddr*) &network->servaddr, network->addr_len);
}

void Server_getClient(hdNetwork* network, socklen_t socklen) { //if this works first try im officially god
	struct sockaddr_in* client = (struct sockaddr_in*) &network->servaddr;
	u64 time = getTime();
	for(int i=0; i<MAX_CLIENTS; i++){
		//ok like this SHOULD be done with a hashmap but I also want to go to sleep tonight so this WON'T be done with a hashmap, ok?
		hdClient* c = &network->clients[i];
		if(c->isreal){ 
			if(time - c->last_seen > 1000000){
				printf("Disconnecting inactive client\n");
				c->isreal = false;
			} else if(c->sockaddr.sin_addr.s_addr == client->sin_addr.s_addr){ 
				c->sockaddr = *client; c->addr_len = socklen; c->isreal = true; c->last_seen = getTime();
				return;
			}
		}
	}
	for(int i=0; i<MAX_CLIENTS; i++){
		hdClient* c = &network->clients[i];
		if(!c->isreal){
			c->sockaddr = *client; c->addr_len = socklen; c->isreal = true; c->last_seen = getTime();
			printf("yoo new client\n");
			return;
		}
	}
}

void Server_broadcastData(hdNetwork* network, hdPacket* packet){
	for(int i=0; i<MAX_CLIENTS; i++){
		//printf("%d\n", network->clients[i].isreal);
		if(network->clients[i].isreal){
			//printf("Sent stuff %*.s\n", packet->data_size, packet->data);
			sendto(network->sockfd, packet, sizeof(hdPacket)+packet->data_size, 0, (struct sockaddr*) &network->clients[i].sockaddr, network->clients[i].addr_len);
		}
	}
	free(packet);
}

static void err(int i, char*message){
	if(i < 0){
		printf("Error: %s - %s\n",message, strerror(errno));
		exit(1);
	}
}
