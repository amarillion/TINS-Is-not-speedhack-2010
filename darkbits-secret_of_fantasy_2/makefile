BIN = game
LIBS = `pkg-config allegro --libs` -laldmb -ldumb -ljpgal
SRC = $(wildcard src/*.cpp)
OBJ = $(addsuffix .o, $(basename $(SRC)))
CFLAGS = -W -Wall -Wno-unused -Isrc

$(BIN): $(OBJ)
	g++ -o $(BIN) $^ $(LIBS) $(LFLAGS)

$(OBJ): %.o: %.cpp
	g++ $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm $(OBJ) $(BIN)
