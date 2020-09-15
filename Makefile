CXXFLAGS = -Wall -std=c++11
NAMES = Face Pyraminx Eq_Pos_Table
OBJECTS = $(addsuffix .o, $(NAMES))
EXEC = interactive autosolve

all: $(EXEC)

# Some help here from https://stackoverflow.com/questions/1079832/how-can-i-configure-my-makefile-for-debug-and-release-builds
debug: CXXFLAGS += -g
release: CXXFLAGS += -O3

debug: $(EXEC)
release: $(EXEC)

interactive : $(OBJECTS) interactive.o
	$(CXX) $(CXXFLAGS) -o $@ $^

autosolve : $(OBJECTS) autosolve.o
	$(CXX) $(CXXFLAGS) -o $@ $^

interactive.o : Pyraminx.o interactive.cpp
autosolve.o: Pyraminx.o autosolve.cpp
Pyraminx.o : Pyraminx.cpp Pyraminx.h Face.o Eq_Pos_Table.o
Eq_Pos_Table.o : Eq_Pos_Table.h Eq_Pos_Table.cpp
Face.o : Face.h Face.cpp

clean:
	rm  *.o $(EXEC)

test: $(EXEC)
	./pyraminx < test_input

run: $(EXEC)
	./pyraminx
