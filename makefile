.PHONY: clean compile run sus streamers 

_BUILDNAME_ = test
BUILD = build
INCLUDE = include
SRC = src
CFLAGS = -lncurses -I$(INCLUDE) -DNDEBUG -lm


_OBJS = ascii_image.o engine_clock.o player.o vector2.o hashmap.o screen.o linked_list.o event_signals.o networking.o

OBJS = $(patsubst %, $(BUILD)/%, $(_OBJS))

_HEADERS = ascii_image.h engine_clock.h player.h types.h vector2.h hashmap.h screen.h linked_list.h event_signals.h networking.h
HEADERS = $(patsubst %, $(INCLUDE)/%, $(_HEADERS))


run: compile
	./$(BUILD)/$(_BUILDNAME_)

compile: ./$(BUILD)/$(_BUILDNAME_) 
	@echo Finished building: $(_BUILDNAME_)

./$(BUILD)/$(_BUILDNAME_): $(OBJS) ./$(BUILD)/main.o
	gcc -o ./$(BUILD)/$(_BUILDNAME_) ./$(BUILD)/main.o $(OBJS) $(HEADERS) $(CFLAGS)

streamers: ./$(BUILD)/player ./$(BUILD)/streamer

./$(BUILD)/streamer: $(OBJS) ./$(BUILD)/exampleVideoStreamer.o
	gcc -o ./$(BUILD)/streamer ./$(BUILD)/exampleVideoStreamer.o $(OBJS) $(HEADERS) $(CFLAGS)

./$(BUILD)/player: $(OBJS) ./$(BUILD)/exampleVideoPlayer.o
	gcc -o ./$(BUILD)/player ./$(BUILD)/exampleVideoPlayer.o $(OBJS) $(HEADERS) $(CFLAGS)

$(BUILD)/%.o: $(SRC)/%.c $(HEADERS)
	gcc -c -o $@ $< $(CFLAGS)

$(BUILD)/%.o: $(INCLUDE)/%.c $(HEADERS)
	gcc -c -o $@ $< $(CFLAGS)

sus:
	echo $(OBJS)

clean:
	rm -rf $(BUILD)/*
