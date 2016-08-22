CC=g++-6
LIBS=-lsfml-graphics -lsfml-system -lsfml-window -lstdc++fs
OPTS=-Wall -std=c++1z

EXE=bin/codesketch.out
SRCS=src/main.cpp src/text.cpp src/shell.cpp #src/sketch.cpp

all: $(EXE)

$(EXE): $(SRCS)
	@mkdir -p bin
	$(CC) $^ $(OPTS) $(LIBS) -o $@

run: $(EXE)
	./$(EXE)

clean:
	@rm -r bin
