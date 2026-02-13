objects = RS_tools.o RS_Encoder.o

program : $(objects)
	g++ -Wall -o programme $(objects)
	./programme

RS_tools.o : RS_tools.cpp
	g++ -Wall -c RS_tools.cpp

RS_Encoder.o : RS_Encoder.cpp 
	g++ -Wall -c RS_Encoder.cpp

valgrind : $(programme)
	g++ -Wall -o programme $(objects)
	valgrind ./programme

clean:
	rm -f $(objects) programme