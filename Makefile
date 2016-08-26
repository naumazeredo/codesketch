CC=g++
LIBS=-lsfml-graphics -lsfml-system -lsfml-window -lboost_system -lboost_filesystem
OPTS=-Wall -std=c++11

EXE=bin/codesketch.out
SRCS=src/main.cpp src/text.cpp src/shell.cpp src/sketch.cpp

all: $(EXE)

$(EXE): $(SRCS)
	@mkdir -p bin
	$(CC) $^ $(OPTS) $(LIBS) -o $@

run: $(EXE)
	./$(EXE)

clean:
	@rm -r bin
