.PHONY: clean compile run sus

_BUILDNAME_ = test
BUILD = build
INCLUDE = include
SRC = src
CFLAGS = -lncurses -I$(INCLUDE) -DNDEBUG -lm


_OBJS = main.o ascii_image.o engine_clock.o player.o vector2.o hashmap.o screen.o linked_list.o event_signals.o networking.o
OBJS = $(patsubst %, $(BUILD)/%, $(_OBJS))

_HEADERS = ascii_image.h engine_clock.h player.h types.h vector2.h hashmap.h screen.h linked_list.h event_signals.h networking.h
HEADERS = $(patsubst %, $(INCLUDE)/%, $(_HEADERS))


run: compile
	./$(BUILD)/$(_BUILDNAME_)

compile: ./$(BUILD)/$(_BUILDNAME_)
	@echo Finished building: $(_BUILDNAME_)

./$(BUILD)/$(_BUILDNAME_): $(OBJS)
	@gcc -o ./$(BUILD)/$(_BUILDNAME_) $(OBJS) $(HEADERS) $(CFLAGS)

$(BUILD)/%.o: $(SRC)/%.c $(HEADERS)
	@gcc -c -o $@ $< $(CFLAGS)

$(BUILD)/%.o: $(INCLUDE)/%.c $(HEADERS)
	gcc -c -o $@ $< $(CFLAGS)

sus:
	echo $(OBJS)

clean:
	rm -rf $(BUILD)/*
