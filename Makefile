CC=g++
LIBS=-lSDL2
OPTS=-Wall -std=c++11

EXE=bin/codesketch.out
SRCS=src/main.cpp

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
