BIN = game
LIBS = `pkg-config allegro-4.9 --libs`
SRC = $(wildcard src/*.c) $(wildcard src/t3f/*.c)
OBJ = $(addsuffix .o, $(basename $(SRC)))
CFLAGS = -W -Wall -Wno-unused -Isrc -Isrc/t3f

$(BIN): $(OBJ)
	gcc -o $(BIN) $^ $(LIBS) $(LFLAGS)

$(OBJ): %.o: %.c
	gcc $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm $(OBJ) $(BIN)
