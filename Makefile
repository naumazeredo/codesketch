CC=g++-6
LIBS=-lSDL2 -lSDL2_ttf -lstdc++fs
OPTS=-Wall -std=c++1z

EXE=bin/codesketch.out
SRCS=src/main.cpp src/sketch.cpp src/text.cpp src/shell.cpp

all: $(EXE)

$(EXE): $(SRCS)
	@mkdir -p bin
	$(CC) $^ $(OPTS) $(LIBS) -o $@

run: $(EXE)
	./$(EXE)

clean:
	@rm -r bin
