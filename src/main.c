#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include <ncurses.h>
#include <math.h>
#include "hashmap.h"

#include "engine_clock.h"
#include "player.h"
#include "ascii_image.h"
#include "event_signals.h"
#include "types.h"
#include "vector2.h"
#include "screen.h"
#include "networking.h"



void testRawImageReadingAndWriting(char* path){
	hdRawImage* img = loadRawImage(path);
	for(int y=0; y<img->size_y; y++){
		for(int x=0; x<img->size_x; x++){
			hdPixel* p = &img->arr[y * img->size_x + x];
			printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p->r, p->g, p->b, p->c); 
		}
		printf("\n");
	}
	writeRawImage(img, "temp.img"); 
	hdRawImage* img2 = readRawImage("temp.img");
	for(int y=0; y<img->size_y; y++){
		for(int x=0; x<img->size_x; x++){
			hdPixel* p1 = &img2->arr[y * img->size_x + x];
			hdPixel* p2 = &img->arr[y * img->size_x + x];
			printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p1->r, p1->g, p1->b, p1->c); 
			printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p2->r, p2->g, p2->b, p2->c); 
			if(p1->r != p2->r){
				printf("sus (%d,%d) %d vs %d\n", x, y, p1->r, p2->r);
				return;
			}
		}
		printf("\n");
	}
	remove("temp.img");
	printf("it did not break :thumbsup:\n");
}

void testRawImageCompression(char* path){
	hdRawImage* img = loadRawImage(path);
	printf("compressing image\n");
	u64 time; 
	time = getTime();
	hdCompressedImage* cimg = compressRawImage(img, NULL);
	printf("Compressing image took %lu ms\n", (getTime() - time) / 1000);
	printf("uncompressing image\n");
	time = getTime();
	hdRawImage* img2 = uncompressImage(cimg);
	printf("uncompressing image took %lu ms\n", (getTime() - time) / 1000);

	printf("comparing the images\n");
	for(int y=0; y<img2->size_y; y++){
		for(int x=0; x<img2->size_x; x++){
			hdPixel* p1 = &img2->arr[y * img->size_x + x];
			hdPixel* p2 = &img->arr[y * img->size_x + x];
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p1->r, p1->g, p1->b, p1->c); 
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p2->r, p2->g, p2->b, p2->c); 
			if(p1->r != p2->r){
				printf("sus (%d,%d) %d vs %d\n", x, y, p1->r, p2->r);
				return;
			}
		}
		//printf("\n");
	}
	printf("Writing the image\n");
	writeCompressedImage(cimg, "temp.compimg");
	printf("Reading the image\n");

	time = getTime();
	hdCompressedImage* cimg2 = readCompressedImage("temp.compimg", NULL);
	printf("Reading took %lu ms\n", (getTime()-time) / 1000);
	
	printf("Uncompressing the read image\n");
	hdRawImage* img3 = uncompressImage(cimg2);
	printf("Comparing the image after reading with the original image\n");
	for(int y=0; y<img3->size_y; y++){
		for(int x=0; x<img3->size_x; x++){
			hdPixel* p1 = &img3->arr[y * img->size_x + x];
			hdPixel* p2 = &img->arr[y * img->size_x + x];
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p1->r, p1->g, p1->b, p1->c); 
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p2->r, p2->g, p2->b, p2->c); 
			if(p1->r != p2->r){
				printf("sus (%d,%d) %d vs %d\n", x, y, p1->r, p2->r);
				return;
			}
		}
		//printf("\n");
	}
	printf("Now reading the image while re-using the palette from the old image\n");

	time = getTime();
	hdCompressedImage* cimg3 = readCompressedImage("temp.compimg", cimg->palette);
	printf("Reading took %lu ms\n", (getTime() - time) / 1000);

	printf("Comparing uncompressed with original\n"); 
	hdRawImage* img4 = uncompressImage(cimg3);
	for(int y=0; y<img4->size_y; y++){
		for(int x=0; x<img4->size_x; x++){
			hdPixel* p1 = &img4->arr[y * img->size_x + x];
			hdPixel* p2 = &img->arr[y * img->size_x + x];
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p1->r, p1->g, p1->b, p1->c); 
			//printf("\033[48;2;%hhu;%hhu;%hhum%c\033[0m", p2->r, p2->g, p2->b, p2->c); 
			if(p1->r != p2->r){
				printf("sus (%d,%d) %d vs %d\n", x, y, p1->r, p2->r);
				return;
			}
		}
		//printf("\n");
	}

	nukeRawImage(img); 
	nukeRawImage(img2);
	nukeRawImage(img3);
	nukeRawImage(img4);
	
	nukeCompressedImage(cimg); //this will run nukePalette(cimg->palette)
	nukeCompressedImage(cimg2); 
	cleanupCompressedImage(cimg3); //Since cimg3 was created by reusing the palette from cimg, if I were to run nuke I would free the palette twice, which is undefined behaviour. Therefore I use cleanupCompressedImage which does NOT free the palette.

}

void testHashing(){
	//printf("A\n");
	hdPixel* p1 = (hdPixel*)(calloc(sizeof(hdPixel), 1)); 
	hdPixel* p2 = (hdPixel*)(calloc(sizeof(hdPixel), 1)); 
	hdPixel* p3 = (hdPixel*)(calloc(sizeof(hdPixel), 1)); 
	p1->c = 'a'; p1->r = 100; p1->g = 100; p1->b = 100; 
	p2->c = 'a'; p2->r = 100; p2->g = 100; p2->b = 100; 
	p3->c = 'a'; p3->r = 100; p3->g = 99; p3->b = 100; 
	//printf("A\n");
	struct hashmap *map = hashmap_new(sizeof(hdHashEntry), 0, 0, 0, pixel_hash, pixel_compare, NULL, NULL);
	//printf("A\n");
	hashmap_set(map, &(hdHashEntry){ .pixel = p1, .pos = 1});
	hdHashEntry* result;
	result = (hdHashEntry*) hashmap_get(map, &(hdHashEntry) {.pixel = p1 });
	printf("Position of p1 when it is the only element with a value of 1. This should print 1: %d\n", result->pos);
	if(result->pos != 1){
		return;
	}
	result = (hdHashEntry*) hashmap_get(map, &(hdHashEntry) {.pixel = p2 });
	printf("p2, an identical pixel to p1's position. This, too, should print 1: %d\n", result->pos);
	
	if(result->pos != 1){
		return;
	}
	result = (hdHashEntry*) hashmap_get(map, &(hdHashEntry) {.pixel = p3 });
	printf("p3==0, a different pixel that's not in the map yet. This, as well, should print 1: %d\n", result==0);
	if(result != 0){
		return;
	}
	
	
	

}


void exitGame(int n){
	curs_set(1);
	endwin();
	exit(n);
}

int get_wasd_input() {
  int ch = getch();
  if (
    ch == 'w' || ch == 'W' || 
    ch == 'a' || ch == 'A' || 
    ch == 's' || ch == 'S' || 
    ch == 'd' || ch == 'D') {
      return ch;
  }

  return 0;
}

typedef struct { 
  int* var; 
  Vector2 lastPos;
  Connection* self;
} MovedCtx;

void IsPlayerPastXValOf700(void* context, void* args) { // Check if player x pos is >= 10
  MovedCtx* con = (MovedCtx*) context;
  Vector2 pos = *((Vector2*) args);

  Vector2 delta = Vector2_sub(pos, con->lastPos);
  con->lastPos = pos;

  mvprintw(12, 0, "^^^ Proof it's still connected. Position changed by (%.3lf, %.3lf)", delta.x, delta.y);

  if (pos.x >= 700) {
    *con->var = 1;
    Connection_Disconnect(con->self);
  }
}

void testClient(char* ip){
	//printf("vro\n");
	struct addrinfo *results; 

	hdNetwork* queue = initializeNetworkQueue();
	int sockfd = setupUDP_Client(ip, &results, queue);

	char* message = (char*) (malloc(100)); 
	strcpy(message, "sus");
	hdPacket* out = createPacket(message, strlen(message));
	//QueueReliableNetworkMessage(queue, out);
	printf("Letting the server know we exist\n");
	//loopNetworkQueue(queue);
	Client_sendData(queue, out);
	int bytes;
	hdPacket* in = (hdPacket*) (calloc(sizeof(hdPacket), 1));
	int i=0;
	while(1){
		//sprintf(message, "%d", i++);
		//bytes = loopNetworkQueue(queue);
		printf("Waiting...\n");
		Client_receiveData(queue, &in);
		printf("Received back from the server: %s, pos id: %d\n", (char*)in->data, in->pos);
		//out = createPacket(message, strlen(message));
		
		//QueueReliableNetworkMessage(queue, out); //this will free() the original out. it queues a copy of out. 
		usleep(10000);
	}
	//printf("%d\n", bytes);
	//bytes = sendto(sockfd, message, strlen(message), 0, results->ai_addr, results->ai_addrlen);
	
}

void testServer(char* ip){
	hdNetwork* network = initializeNetworkQueue();
	int server_fd = setupUDP_Server(network); 
	char* data = (char*)(malloc(100));
	int i = 0;
	//sprintf(data, "%d", i);
	sprintf(data, "%d", i++);
	hdPacket* out = createPacket(data, strlen(data));
	hdPacket* buffer = (hdPacket*)(calloc(sizeof(hdPacket), 1));
	while(1){
		//bytes = recvfrom(server_fd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*) &client_addr, &addr_len);
		//buffer[bytes] = 0;
		//printf("Received %s\n", buffer);
		Server_receiveData(network, &buffer);
		//Server_broadcastData(network, buffer);
		//sendto(server_fd, buffer, strlen(buffer), 0, (struct sockaddr*) &client_addr, addr_len);
		sprintf(data, "%d", i++);
		//printf("data=%*.s\n", (int)strlen(data), data);
		//printf("data=%s %lu %d\n", data, strlen(data), (int) strlen(data));
		out = createPacket(data, strlen(data));
			
		//printf("Sending %s\n", (char*)out->data);
		//out->data_size = strlen((char*) out->data);
		Server_broadcastData(network, out);
		//QueueReliableNetworkMessage(network, out);
		//loopNetworkQueue(network);
		//out = createPacket(data, strlen(data));
		usleep(1000000);
	}

}

void testMulti(char* ip){
	hdNetwork* networkqueue = initializeNetworkQueue();
}

typedef struct {
  hdScreen* screen;
  Player* player;
  hdSprite* playerSprite;
  int* var;
} HeartbeatCtx;

typedef struct {
  double dt;
  double elapsed;
} HeartbeatEvent;

void centerCameraOnPlayer(hdScreen* screen, Player* player) {
  int screen_x, screen_y;
  getmaxyx(stdscr, screen_y, screen_x);
  screen->camera->pos_x = round(player->pos.x - (double) screen_x / 4);
  screen->camera->pos_y = round(player->pos.y + (double) screen_y / 2);
}

void centerSpriteOnPlayer(hdSprite* sprite, Player* player) {
  sprite->pos_x = round(player->pos.x - (double) sprite->image->size_x / 2);
  sprite->pos_y = round(-1 * player->pos.y - (double) sprite->image->size_y / 4);
}

void onHeartBeat(void* context, void* args) {
  HeartbeatCtx* con = (HeartbeatCtx*) context;
  // HeartbeatEvent* event = (HeartbeatEvent*) args;

  int input;

  hdScreen* screen = con->screen;
  Player* player = con->player;
  hdSprite* playerSprite = con->playerSprite;

  int* var = con->var;

  double dt = EngineClock_waitForNextFrame();
  double elapsed = EngineClock_getTimeElapsed();

  draw(screen); // Let this draw a frame behind noobs

  input = getch(); 
  if(input == '['){
    screen->camera->theta += M_PI / 16;
  }

  Player_handleInput(player, input);
  Player_updateMovement(player, dt * 5);

  centerCameraOnPlayer(screen, player);
  centerSpriteOnPlayer(playerSprite, player);

  mvprintw(10, 0, "Pos: (%.2lf, %.2lf)", player->pos.x, player->pos.y);
  if (*var == 1) {
    mvprintw(11, 0, "Player x position has passed 700!! (Event now disconnected)");
  } else {
    mvprintw(11, 0, "Player x position not yet passed 700. (Event still connected)");
  }
  if (elapsed > 5) {
    mvprintw(13, 0, "Over 5 seconds have passed!");
  }
  refresh();
}

Signal* Game_init() {
	EngineClock_init(); 
	Player* newPlayer = (Player*) calloc(1, sizeof(Player)); 
	Player_init(newPlayer, "Jesse");

  int* var = (int*) calloc(1, sizeof(int));

  MovedCtx* MovedContext = calloc(1, sizeof(MovedCtx));
  MovedContext->var = var;
  MovedContext->self = Signal_Connect(newPlayer->moved, &IsPlayerPastXValOf700, MovedContext);

	hdScreen* screen = initScreen();
	hdSprite* bg = initSprite(loadRawImage("assets/TheSkeld.txt"), NULL);
	hdSprite* cmap = initSprite(loadRawImage("assets/TheSkeldMask.txt"), NULL);
	hdSprite* amog = initSprite(loadRawImage("assets/smallsus.txt"), NULL);

  addCollisionMap(screen, cmap);
  Player_enableCollision(newPlayer, screen);

	addSprite(screen, bg);
	addSprite(screen, amog);

  Signal* Heartbeat = Signal_new();

  HeartbeatCtx* HeartbeatContext = (HeartbeatCtx*) malloc(sizeof(HeartbeatCtx));
  HeartbeatContext->screen = screen;
  HeartbeatContext->player = newPlayer;
  HeartbeatContext->playerSprite = amog;
  HeartbeatContext->var = var;

  Signal_Connect(Heartbeat, &onHeartBeat, HeartbeatContext);
  return Heartbeat;
}

void StartLocalGame() {
  Signal* GameHeartbeat = Game_init();

	while (1) {
    Signal_Fire(GameHeartbeat, NULL);
    // Connect extra signals to heartbeat
  }
}

int main(int argc, char* argv[]){
	//testRawImageReadingAndWriting("assets/sus.txt");
	//testRawImageCompression("assets/TheSkeld.txt");
	//testHashing();
	//testScreen("assets/smallsus.txt", "assets/TheSkeld.txt");

  //testEngineClock();
	//testScreen("assets/TheSkeld.txt", "assets/sus.txt");

	// if(argc == 1){
	// 	testClient("127.0.0.1");
	// }else{
	// 	testServer("127.0.0.1");
	// }
	//testMulti("127.0.0.1");

  StartLocalGame();
}
