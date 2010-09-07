#
# invoke "make BUILD=DEBUG" or "make BUILD=RELEASE"
# 
# You can override this from the command line,
# e.g. make BUILD=DEBUG
#BUILD=DEBUG
BUILD=RELEASE
#BUILD=STATIC
#BUILD=STATIC_DEBUG

CXX = g++
LD = g++
CFLAGS = -Iinclude -W -Wall -Wno-unused
LFLAGS = 
LIBS = -lalfont
NAME = mexica

ifeq ($(BUILD),RELEASE)
	CFLAGS += -O3
	LFLAGS += -s
endif
ifeq ($(BUILD),DEBUG)
	CFLAGS += -g -DDEBUG
endif
ifeq ($(BUILD),STATIC)
	CFLAGS += -O3 -DALLEGRO_STATICLINK
	LFLAGS += -s
endif
ifeq ($(BUILD),STATIC_DEBUG)
	CFLAGS += -g -DDEBUG -DALLEGRO_STATICLINK
endif

ifndef WINDOWS
ifdef MINGDIR
	WINDOWS = 1
endif
endif

ifdef WINDOWS
	OBJDIR = obj/win
	CFLAGS += -D__GTHREAD_HIDE_WIN32API
	LFLAGS += -Wl,--subsystem,windows
	ifeq ($(BUILD),RELEASE)
		LIBS += -lalleg
	endif
	ifeq ($(BUILD),DEBUG)
		LIBS += -lalld
	endif
	ifeq ($(BUILD),STATIC_DEBUG)
		LIBS += -lalleg-debug-static -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound
	endif
	ifeq ($(BUILD),STATIC)
		LIBS += -lalleg-static -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound
	endif
	BIN = $(NAME).exe
	ICONOBJ = $(OBJDIR)/icon.o	
else
	OBJDIR = obj/unx
	ifeq ($(BUILD),RELEASE)
		LIBS += `pkg-config allegro --libs logg`
	endif
	ifeq ($(BUILD),DEBUG)
		LIBS += `pkg-config allegro-debug --libs logg`
	endif
	ifeq ($(BUILD),STATIC)
		LIBS += `pkg-config allegro --libs logg --static`
	endif
	ifeq ($(BUILD),STATIC_DEBUG)
		LIBS += `pkg-config allegro-debug --libs logg --static`
	endif
	BIN = $(NAME)
endif

SRC = $(wildcard src/*.cpp)
OBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o, $(basename $(notdir $(SRC)))))
DEP = $(addprefix $(OBJDIR)/,$(addsuffix .d, $(basename $(notdir $(SRC)))))

SRC_AE = $(wildcard animedit/*.cpp)
OBJ_AE = $(addprefix $(OBJDIR)/,$(addsuffix .o, $(basename $(notdir $(SRC_AE)))))

.PHONY: game
game: $(OBJ) $(ICONOBJ)
	$(LD) -o $(BIN) $^ $(LIBS) $(LFLAGS)

.PHONY: all
all: game animeditor

$(OBJ): $(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -MMD -o $@ -c $<

$(OBJ_AE): $(OBJDIR)/%.o: animedit/%.cpp
	$(CXX) $(CFLAGS) -MMD -o $@ -c $<

animeditor: $(OBJ_AE) $(OBJDIR)/anim.o $(OBJDIR)/resources.o $(OBJDIR)/dom.o $(OBJDIR)/level.o
	$(LD) -o animeditor $^ $(LIBS) $(LFLAGS)

$(ICONOBJ) : icon.rc icon.ico
	windres -I rc -O coff -i icon.rc -o $(ICONOBJ)

# include the .d files
-include $(DEP)

RMFILES = $(OBJ) $(DEP)

.PHONY: clean
clean:
ifdef WINDOWS
	del /q $(subst /,\,$(RMFILES))
else
	rm -f $(RMFILES)
endif

.PHONY:distclean
distclean: clean
ifdef WINDOWS
	del /q $(subst /,\,$(BIN))
else
	rm -f $(BIN)
endif

