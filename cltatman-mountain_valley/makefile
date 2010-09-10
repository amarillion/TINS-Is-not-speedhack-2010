BIN = game
LIBS = `pkg-config allegro --libs` -laldmb -ldumb -ljpgal
SRC = $(wildcard code/Source/back/*.cpp) $(wildcard code/Source/game/*.cpp)
OBJ = $(addsuffix .o, $(basename $(SRC)))
CFLAGS = -W -Wall -Wno-unused -Icode/Include/game -Icode/Include/back

$(BIN): $(OBJ)
	g++ -o $(BIN) $^ $(LIBS) $(LFLAGS)

$(OBJ): %.o: %.cpp
	g++ $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm $(OBJ) $(BIN)
