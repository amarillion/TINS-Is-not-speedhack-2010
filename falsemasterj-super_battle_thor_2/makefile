#######################################################################
#
# Adapted from the SpeedHack 2009 template.
#
# Instructions:
#
# make
#   Compiles all .c and .cpp files in the src directory to .o
#   files in the obj directory, and links them into an
#   executable named 'game' or 'game.exe' in the currect directory.
#
# make clean
#   Removes all .o files from the obj directory.
#
# make veryclean
#   Removes all .o files and the game executable.
#
# Optional parameters:
#
# STATICLINK=1
#   Compiles/removes a statically linked version of the game without
#   DLL dependencies. The static object files are put in obj/static
#   and the executable has '_static' appended to the name.
#
# NAME=game_name
#   Sets the name of the game executable. By default the game
#   executable is called 'game' or 'game.exe'.
#
# If you use add-on libraries, add them to the lines starting with
# 'LIBS='. Make sure you enter the libraries in both lines, for the
# normal and static version!
#
#######################################################################

CC = gcc
CXX = g++
LD = g++
CFLAGS = -Iinclude -O3 -s -W -Wall
RM = rm -f

ifndef NAME
	NAME = super_battle_thor_2
endif

ifdef STATICLINK
	LIBS = `pkg-config --cflags --static --libs allegro-static-4.9 allegro_image-static-4.9 allegro_audio-static-4.9 allegro_acodec-static-4.9 allegro_physfs-static-4.9 allegro_font-static-4.9 allegro_ttf-static-4.9 allegro_primitives-static-4.9 allegro_dialog-static-4.9 allegro_color-static-4.9 allegro_main-static-4.9 allegro_memfile-static-4.9` -lXrender
	OBJDIR = obj/static
	BIN = $(NAME)_static
else
	LIBS = `pkg-config --cflags --libs allegro-4.9 allegro_image-4.9 allegro_audio-4.9 allegro_acodec-4.9 allegro_physfs-4.9 allegro_font-4.9 allegro_ttf-4.9 allegro_primitives-4.9 allegro_dialog-4.9 allegro_color-4.9 allegro_main-4.9 allegro_memfile-4.9`
	OBJDIR = obj
	BIN = $(NAME)
endif

OBJ_CPP := $(addprefix $(OBJDIR)/, $(subst src/,,$(patsubst %.cpp,%.o,$(wildcard src/*.cpp))))
OBJ_C := $(addprefix $(OBJDIR)/, $(subst src/,,$(patsubst %.c,%.o,$(wildcard src/*.c))))

all: game

$(OBJDIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

game: $(OBJ_C) $(OBJ_CPP)
	$(LD) -o $(BIN) $(OBJ_C) $(OBJ_CPP) $(LIBS) $(LFLAGS)

clean:
ifneq ($(OBJ_C),)
	-$(RM) $(OBJ_C)
endif
ifneq ($(OBJ_CPP),)
	-$(RM) $(OBJ_CPP)
endif

veryclean: clean
	-$(RM) $(BIN)

