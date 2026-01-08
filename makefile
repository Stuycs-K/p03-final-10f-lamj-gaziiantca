.PHONY: clean compile run sus

_BUILDNAME_ = test
BUILD = build
INCLUDE = include
SRC = src
CFLAGS = -lncurses -I$(INCLUDE) -DNDEBUG -lm


_OBJS = main.o ascii_image.o consts.o engine_clock.o player.o vector2.o linked_list.o
OBJS = $(patsubst %, $(BUILD)/%, $(_OBJS))

_HEADERS = ascii_image.h consts.h engine_clock.h player.h types.h vector2.h linked_list.h
HEADERS = $(patsubst %, $(INCLUDE)/%, $(_HEADERS))


run: compile
	./$(BUILD)/$(_BUILDNAME_)

compile: ./$(BUILD)/$(_BUILDNAME_)

./$(BUILD)/$(_BUILDNAME_): $(OBJS)
	gcc -o ./$(BUILD)/$(_BUILDNAME_) $(OBJS) $(HEADERS) $(CFLAGS)

$(BUILD)/%.o: $(SRC)/%.c $(HEADERS)
	gcc -c -o $@ $< $(CFLAGS)

sus:
	echo $(OBJS)

clean:
	rm -rf $(BUILD)/*
