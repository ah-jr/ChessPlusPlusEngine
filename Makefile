chess: base.o pawn.o rook.o knight.o bishop.o queen.o king.o main.o
	g++ -o chess *.o

main.o: main.cpp
	g++ -c main.cpp 

king.o: pieces/king.cpp
	g++ -c pieces/king.cpp

queen.o: pieces/queen.cpp
	g++ -c pieces/queen.cpp

bishop.o: pieces/bishop.cpp
	g++ -c pieces/bishop.cpp

knight.o: pieces/knight.cpp
	g++ -c pieces/knight.cpp

rook.o: pieces/rook.cpp
	g++ -c pieces/rook.cpp

pawn.o: pieces/pawn.cpp
	g++ -c pieces/pawn.cpp

base.o: pieces/base.cpp
	g++ -c pieces/base.cpp

clean:
	del *.o chess