CXX ?=g++

ifeq ($(OS),Windows_NT)
	LIBS +=-lmingw32
	FLAGS+=-mwindows
endif

LIBS +=-lSDL2main -lSDL2_ttf -lSDL2
FLAGS +=--std=c++11 -O3 -Wall -pedantic

EXE=15-puzzle

15-puzzle: src/main.cpp src/puzzle.cpp src/puzzle.hpp
	$(CXX) src/main.cpp src/puzzle.cpp $(LIBS) -o $(EXE) $(FLAGS)

clean:
ifeq ($(OS),Windows_NT)
	del $(EXE).exe
else
	rm -f $(EXE)
endif
