CXX ?=g++

ifeq ($(OS),Windows_NT)
	LIBS +=-lmingw32
	FLAGS+=-mwindows
endif

LIBS +=-lSDL2main -lSDL2_ttf -lSDL2
FLAGS +=--std=c++11 -O3 -Wall

EXE=15-puzzle

15-puzzle: main.cpp puzzle.cpp puzzle.hpp
	$(CXX) main.cpp puzzle.cpp $(LIBS) -o $(EXE) $(FLAGS)

clean:
	rm -f $(EXE)
