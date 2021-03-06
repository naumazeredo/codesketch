CC=g++
OPTS=-Wall -std=c++11

ifeq ($(OS), Windows_NT)
	LIBS=-lsfml-graphics -lsfml-system -lsfml-window -lboost_system-mgw63-1_63 -lboost_filesystem-mgw63-1_63
	LIBS_PATH=-L"C:/SFML-2.4.1/lib" -L"C:/boost_1_63_0/stage/lib"
	INCLUDES=-I"C:/SFML-2.4.1/include" -I"C:/boost_1_63_0"

	DLLS_FOLDER=libs/win
	OPTS+=-mwindows
	EXE_NAME=codesketch.exe

	PACK_CMD=zip -r
	PACK_NAME=CodeSketch-win.zip

	RUN=./$(EXE_NAME)
else
	LIBS=-lsfml-graphics -lsfml-system -lsfml-window -lboost_system -lboost_filesystem
	LIBS_PATH=-L"/mnt/c/SFML-2.4.2/lib" -L"/mnt/c/boost_1_63_0/stage/lib"
	INCLUDES=-I"/mnt/c/SFML-2.4.2/include" -I"/mnt/c/boost_1_63_0"

	DLLS_FOLDER=libs/linux
	EXE_NAME=codesketch.out

	PACK_CMD=tar czvf
	PACK_NAME=CodeSketch-linux.tar.gz

	RUN=DISPLAY=:0 ./CodeSketch-linux.sh
endif

SRCS=src/main.cpp src/text.cpp src/shell.cpp src/sketch.cpp

SKETCHES_FOLDER=sketches
FONTS_FOLDER=fonts
BIN_FOLDER=bin
EXE=$(BIN_FOLDER)/$(EXE_NAME)


all: $(EXE)

$(EXE): $(SRCS)
	@mkdir -p bin
	$(CC) $^ $(OPTS) $(INCLUDES) -o $@ $(LIBS_PATH) $(LIBS)

deps:
	@mkdir -p bin
	@echo "Copying link libraries..."
	@cp $(DLLS_FOLDER)/* $(BIN_FOLDER)
	@echo "Copying fonts..."
	@cp -r $(FONTS_FOLDER) $(BIN_FOLDER)
	@echo "Copying sketches..."
	@cp -r $(SKETCHES_FOLDER) $(BIN_FOLDER)

run: $(EXE) deps
	@echo "Executing CodeSketch..."
	@cd $(BIN_FOLDER) && $(RUN)

clean:
	@echo "Cleaning the mess..."
	@rm -rf $(BIN_FOLDER)
	@rm -f $(PACK_NAME)

pack: $(EXE) deps
	cd bin && $(PACK_CMD) ../$(PACK_NAME) *
