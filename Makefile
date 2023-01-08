chess_test.exe: chess.dll 
	g++ -o chess_test chess_test.cpp

chess.dll: base.o pawn.o rook.o knight.o bishop.o queen.o king.o chess_main.o chess_entry.o
	g++ -o chess.dll obj\*.o -mdll

chess_entry.o: chess_entry.cpp
	g++ -c chess_entry.cpp -o obj/chess_entry.o

chess_main.o: chess_main.cpp
	g++ -c chess_main.cpp -o obj/chess_main.o 

king.o: pieces/king.cpp
	g++ -c pieces/king.cpp -o obj/king.o

queen.o: pieces/queen.cpp
	g++ -c pieces/queen.cpp -o obj/queen.o

bishop.o: pieces/bishop.cpp
	g++ -c pieces/bishop.cpp -o obj/bishop.o

knight.o: pieces/knight.cpp
	g++ -c pieces/knight.cpp -o obj/knight.o

rook.o: pieces/rook.cpp
	g++ -c pieces/rook.cpp -o obj/rook.o

pawn.o: pieces/pawn.cpp
	g++ -c pieces/pawn.cpp -o obj/pawn.o

base.o: pieces/base.cpp
	g++ -c pieces/base.cpp -o obj/base.o

clean:
	del obj\*.o *.exe *.dll chess