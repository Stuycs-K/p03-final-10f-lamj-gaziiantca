# Dev Log: Jesse Lam

### 2026-01-06 - Backseating the Proposal.md
- Added base file structure (10 min)

- Criticized the proposal (5 min)

- Spent 5 minutes adding to the DEVLOG

- Backseating the makefile (5 minutes)

- Added vector2 class (10 min)



### 2026-01-07 - Started on player class. Added engine_clock

- Worked on player.h and player.c to add playable agents.

- Added Vector2 members for position and velocity as well as initialization with player name. (20 min)

- Added engine_clock to wait until next frame based on fps value and record deltaTime values (1hr)

- Added function to test frame timer and budget wasd inputs for changing player status (10 min)

### 2026-01-08 - Working on events

- Backseating image rendering (10 min)

- Added preliminary linkedlist used to hold event callbacks (20 min)

- Completed doubly-linked LinkedList class in preparation for adding events (1 hr)

- Edited some header definitions (Vector2, Player). Added event_signals.h header definitions. (20 min)

### 2026-01-09 - Adding events

- Backseating bad image compression (1 min)

- Worked on adding Signal/Event funciton definitions (40 min)

### 2026-01-11 (Weekend) - Completing events and fixing errors

- Reduced print statements in makefile

- Fixed engine_clock functions to have appropriate naming scheme (5min)

- Completed event_signals.c (30 min)
  - Added context argument to signals to pass arguments into function pointers
  - Added signal_new and fixed Disconnect code

- Added events to the current testing gameloop. Subsequently fixed bugs. (40 min)
  - Added a player.moved event to player objects.
  - Added event test for player.moved
  - Fixed issue with linked list not setting front/end correctly
  - Fixed issue with callbacks not being executed

### 2026-01-12 - Working on multiplayer. Helping debug screen.

- Spent class time debugging and deeply criticizing screen.c's rotation feature (40 min)

- Added test UDP client and server code (40 min)

### 2026-01-13 - Attempting to fix UDP for client. Worked on collision maps instead

- Attempted to fix UDP setup for client side and failing (40 min)

- Added function to add a sprite as a collision map to screen struct (20 min)

- Added Player_enableCollision to add a connection to Player.moved to return a player to the last valid position if moving into a collidable. (40 min)

### 2026-01-14 - Added photo-editing software to create a collision map. Trying to debug collision and screen code

- Used GIMP on school computers to create collision map "mask" for map (20 min)

- Criticized Aleksandr's code more (20 min)

- Trying to debug collision and camera (30 min)

### 2026-01-15 - Finally debugged collisions.

- More failure at debugging collisions (20 min)

- Spent 10 minutes trying to regenerate outdated map file. And locked myself out of my home directory. (Aleksandr's fault)

- Spent another 10 minutes wondering why the map generation was mysteriously broken (tmux doesn't support colors)

- In a historical moment of genius, 2 days worth of bugs came from accidentally using the ASCII pixel's character value as the green color value. Finally fixed collisions (NaN hours)
  - Fixed incorrect cmap bound checking
  - Fixed screen tracking character wrong (only Aleksandr would make character centered at top left of screen)
  - Fixed checking pixel colors
  - Fixed draw() clearing premature debug messages/prints

- Improved skeld map accuracies and resolution (15 min)

- Reformatted main loop into a Heartbeat event in each frame. Improved readability. (40 min)


