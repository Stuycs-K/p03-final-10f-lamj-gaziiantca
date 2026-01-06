.PHONY: clean compile run sus

_BUILDNAME_ = test
BUILD = build
INCLUDE = include
SRC = src
CFLAGS = -lncurses -I$(INCLUDE)


_OBJS = main.o 
OBJS = $(patsubst %, $(BUILD)/%, $(_OBJS))

_HEADERS = 
HEADERS = $(patsubst %, $(INCLUDE)/%, $(_HEADERS))


run: compile
	./$(BUILD)/$(_BUILDNAME_)

compile: ./$(BUILD)/test

./$(BUILD)/$(_BUILDNAME_): $(OBJS)
	gcc -o ./$(BUILD)/$(_BUILDNAME_) $(OBJS) $(HEADERS) $(CFLAGS)

$(BUILD)/%.o: $(SRC)/%.c $(HEADERS)
	gcc -c -o $@ $< $(CFLAGS)

sus:
	echo $(OBJS)

clean: 
	rm -rf $(BUILD)/*

