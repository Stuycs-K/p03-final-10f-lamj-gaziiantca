[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QfAIDTGi)
# ASCII Game Engine

### Group: Ctrl + Alt + L

- Aleksandr Gaziiantc
- Jesse Lam

### Project Description: Create games in terminal (ASCIImongus coverup)

This a game engine with foundational tools and low-level control.

Use Signals to have listening objects or functions activate only on demand.

Use ASCII screen and image functions to display textures and images in the terminal.
  (We used ascii-image-converter by TheZoraiz)

Use the UDP networking library for Server and Client communication.


We have a (perhapseth useful) explanation video:
  - [YouTube](https://youtu.be/oVhpzuQS6QM),
  - [Google Drive](https://drive.google.com/file/d/1RGw3FeTWNVIox9mwPq2uTTM96xRaM53r/view?usp=drive_link)

### Libraries:
  - ncurses
    - Debian/Ubuntu:
      ```
      $ sudo apt update
      $ sudo apt install libncurses5-dev libncursesw5-dev
      ```
    - Arch:
      ```
      $ sudo pacman -S ncurses
      ```
  - hashmap.c (https://github.com/tidwall/hashmap.c)
    - Is a part of the repo 

### Instructions:

To compile:
```
  [p03-final-10f-lamj-gaziiantca]$ make compile
```

Run one of these commands in terminal to run the game:
```
  [p03-final-10f-lamj-gaziiantca]$ make
  [p03-final-10f-lamj-gaziiantca]$ make run
```

To start creating games, edit main() in [p03-final-10f-lamj-gaziiantca/src/main.c].

Edit or connect functions to the game's Heartbeat event in order to create your game.

How you can use Signals:
```
// Define variables that your function can look at as "context"
typedef struct { 
  int* var; 
  Vector2 lastPos;
  Connection* self;
} MovedCtx;

// Similarly to context, define arguments containing data about the specific instance of an event that activates this callback
typedef struct {
  Vector2 lastPos;
  Vector2 newPos;
} PlayerMovedEvent;

// This is your Callback
void IsPlayerPastXValOf700(void* context, void* args) { // Check if player x pos is >= 700
  MovedCtx* con = (MovedCtx*) context;
  PlayerMovedEvent* event = (PlayerMovedEvent*) args;

  Vector2 pos = event->newPos;

  if (pos.x >= 700) {
    *con->var = 1;
    Connection_Disconnect(con->self);
    // Remove this event and edit the boolean after the player fulfills the conditions
  }
}

int main() {
  MovedCtx* MovedContext = calloc(1, sizeof(MovedCtx));
  MovedContext->var = (int*) calloc(1, sizeof(int)); // Storing a boolean

  // Connect your callback using a function pointer and the context
  // The variable newPlayer->moved is the Signal that will activate all connected functions when used with Signal_Fire()
  // This returns a connection object that can be used in Connection_Disconnect() to delete the connection
  Connection* newConnection = Signal_Connect(newPlayer->moved, &IsPlayerPastXValOf700, MovedContext);

  // Storing the connection itself in the context to use Connection_Disconnect() on later
  MovedContext->self = newConnection
}

In Player_UpdateMovement():
  ...
  // Add data for each specific movement of a player to the event
  PlayerMovedEvent event = {oldPos, self->pos};
  // Fire the Signal (Player.moved) with the Event data to activate connected function every time this player moves
  Signal_Fire(self->moved, &event);
  ...

```

Example of how you can create ASCII text images that can be rendered in the terminal:
```
  $ ascii-image-converter -C --color-bg -d${WIDTH},${HEIGHT} /path_to_your_img/image_name.extension > newImagePath.txt

  ...
    #include "ascii_image.h"
    hdRawImage* image = readRawImage("newImagePath.txt")
  ...

  Since this output will weigh a lot, you can optionally compress your image: 
  ...
    #include "ascii_image.h" 
    hdRawImage* image = loadRawImage("newImagePath.txt");
    hdCompressedImage* cimg = compressRawImage(image);
    //uncompression: 
    hdRawImage* uncompressed = uncompressImage(cimg);
  ...

  You can also write down the raw and compressed images to be read later (that's the intended usage btw)
  ...
    #include "ascii_image.h" 
    hdRawImage* image = loadRawImage("newImagePath.txt"); 
    hdCompressedImage* cimg = compressRawImage(image); 
    
    writeRawImage(image, "rawImage");
    hdRawImage* readRawImage = readRawImage("rawImage");
     
    writeCompressedImage(cimg, "compressedImage");
    hdCompressedImage* readCompressedImage = readCompressedImage("compressedImage");
    hdRawImage* readUncompressedImage = uncompressImage(readCompressedImage);
  ...

```

How you can use Screen and Player Collisions and Sprite functions:
```
    
    First, you want to create an image, see the above code to see how to do that. You specifically want an hdRawImage*
    You also want a transparent white overlay image to indicate the collision map
    ...
      hdScreen* screen = initScreen(); 
      hdSprite* sprite = initSprite(image, NULL); //NULL can be replaced with anything extra which you want that sprite to store 
      hdSprite* cmap = initSprite(mapMask, NULL); 

      addCollisionMap(screen, cmap);
      addSprite(screen, sprite); 
        
      Player* plr = (Player*) calloc(1, sizeof(Player)); 
      Player_enableCollision(plr, screen);

      //in your loop (note that I'm not writing out the heartbeat context here):
      draw(screen); 
    ...

```

Networking:

```
Disclaimer: The engine uses exclusively UDP.

You want to start off by making an hdNetwork* object:

  ...
    hdNetwork* network = initializeNetworkQueue(); 
  ...

  From there, servers and clients naturally have different code

  - For the server: 

  ...
    int server_fd = setupUDP_Server(network); 
  ...

  Then, to receive and send data on the server use 
  
  ...
    
    hdPacket* buffer = (hdPacket*)(calloc(sizeof(hdPacket), 1));
    Server_receiveData(network, &buffer); //free()'s the old buffer btw

    //Unreliable message:
    hdPacket* out = createPacket(data, data_size); //you define data and data_size
    Server_broadcastData(network, out);

    //Reliable message (will keep resending): 
    QueueReliableNetworkMessage(network, out); //free()'s the packet btw
    loopNetworkQueue(network);

  ...

  - For the client:

  ...
    struct addrinfo* results; 
    hdNetwork* network = initializeNetworkQueue(); 
    int sockfd = setupUDP_Client(ip, &results, queue); 
    
    //Receiving data: 
    hdPacket* in = (hdPacket*) (calloc(sizeof(hdPacket), 1));
    Client_receiveData(queue, &in); //free()'s the old in btw

    //Unreliable send:
    hdPacket* out = createPacket(data, data_size); 
    Client_sendData(network, out);


    //Reliable send (will keep resending):
    QueueReliableNetworkMessage(network, out); 
    loopNetworkQueue(network); 


    
  ...

```


### Resources/ References:
- Zoraiz Hassan - [TheZoraiz](https://github.com/TheZoraiz)/[ascii-image-converter](https://github.com/TheZoraiz/ascii-image-converter)
