CC=g++
OPTS=-Wall -std=c++11

ifeq ($(OS), Windows_NT)
	LIBS=-lsfml-graphics -lsfml-system -lsfml-window -lboost_system.dll -lboost_filesystem.dll
	LIBS_PATH=-L"C:/SFML-2.4.1/lib" -L"C:/boost_1_63_0/stage/lib"
	INCLUDES=-I"C:/SFML-2.4.1/include" -I"C:/boost_1_63_0"
	DLLS_FOLDER=libs/win
	OPTS+=-mwindows
	EXE_NAME=codesketch.exe
	PACK_CMD=zip -r
else
	LIBS=-lsfml-graphics -lsfml-system -lsfml-window -lboost_system -lboost_filesystem
	#LIBS_PATH=-L"/mnt/c/SFML-2.4.2/lib" -L"/mnt/c/boost_1_63_0/stage/lib"
	#INCLUDES=-I"/mnt/c/SFML-2.4.2/include" -I"/mnt/c/boost_1_63_0"
	DLLS_FOLDER=libs/linux
	EXE_NAME=codesketch.out
	PACK_CMD=tar czvf
endif

SRCS=src/main.cpp src/text.cpp src/shell.cpp src/sketch.cpp

FONTS_FOLDER=fonts
BIN_FOLDER=bin
EXE=$(BIN_FOLDER)/$(EXE_NAME)

PACK_NAME=CodeSketch

all: $(EXE)

$(EXE): $(SRCS)
	@mkdir -p bin
	$(CC) $^ $(OPTS) $(INCLUDES) -o $@ $(LIBS_PATH) $(LIBS)

dep:
	@mkdir -p bin
	@echo "Copying link libraries..."
	@cp $(DLLS_FOLDER)/* $(BIN_FOLDER)
	@echo "Copying fonts..."
	@cp -r $(FONTS_FOLDER) $(BIN_FOLDER)

run: $(EXE) dep
	@echo "Executing CodeSketch..."
	@cd $(BIN_FOLDER) && ./$(EXE_NAME)

clean:
	@echo "Cleaning the mess..."
	@rm -rf $(BIN_FOLDER)
	@rm -f 

pack: $(EXE) dep
	cd bin && $(PACK_CMD) ../$(PACK_NAME) *
