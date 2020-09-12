CPPFLAGS = -Wall -g -std=c++11
NAMES = main Face Pyraminx Eq_Pos_Table
OBJECTS = $(addsuffix .o, $(NAMES))
EXEC = pyraminx

$(EXEC) : $(OBJECTS)
	$(CXX) $(FLAGS) -o $@ $^ 

main.o : Pyraminx.o main.cpp
Pyraminx.o : Pyraminx.cpp Face.o Eq_Pos_Table.o
Eq_Pos_Table.o : Eq_Pos_Table.cpp
Face.o : Face.cpp
	$(CXX) $(CPPFLAGS) -c $^  

clean:
	rm  *.o $(EXEC)

test: $(EXEC)
	./pyraminx < test_input

run: $(EXEC)
	./pyraminx
