CC=g++-6
LIBS=-lSDL2 -lSDL2_ttf -lstdc++fs
OPTS=-Wall -std=c++1z

EXE=bin/codesketch.out
SRCS=src/main.cpp src/sketch.cpp

SKETCH=bin/sketch.out
SKETCHSRC=sketch/sketch.cpp

all: $(EXE) $(SKETCH)

run: $(EXE)
	./$(EXE)

$(EXE): $(SRCS)
	@mkdir -p bin
	$(CC) $^ $(OPTS) $(LIBS) -o $@

$(SKETCH): $(SKETCHSRC)
	@mkdir -p bin
	$(CC) $^ $(OPTS) $(LIBS) -o $@

clean:
	@rm -r bin
