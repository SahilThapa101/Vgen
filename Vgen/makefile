all: Vengeance

Vengeance: main.o magicmoves.o nonslidingmoves.o movegen.o make_unmake.o utility.o perft.o dynarray.o evaluate.o search.o uci.o fen.o vtime.o
	x86_64-w64-mingw32-gcc main.o magicmoves.o nonslidingmoves.o movegen.o make_unmake.o utility.o perft.o dynarray.o evaluate.o search.o uci.o fen.o vtime.o -static -ggdb -O3 -o Vengeance

main.o:	main.c
	x86_64-w64-mingw32-gcc -c main.c

movegen.o: movegen.c
	x86_64-w64-mingw32-gcc -c movegen.c

magicmoves.o: magicmoves.c
	x86_64-w64-mingw32-gcc -c magicmoves.c

nonslidingmoves.o: nonslidingmoves.c
	x86_64-w64-mingw32-gcc -c nonslidingmoves.c

dynarray.o: dynarray.c
	x86_64-w64-mingw32-gcc -c dynarray.c

utility.o: utility.c
	x86_64-w64-mingw32-gcc -c utility.c

make_unmake.o: make_unmake.c
	x86_64-w64-mingw32-gcc -c make_unmake.c

perft.o: perft.c
	x86_64-w64-mingw32-gcc -c perft.c

evaluate.o: evaluate.c
	x86_64-w64-mingw32-gcc -c evaluate.c 

search.o: search.c
	x86_64-w64-mingw32-gcc -c search.c

uci.o:	uci.c
	x86_64-w64-mingw32-gcc -c uci.c

fen.o:	fen.c
	x86_64-w64-mingw32-gcc -c fen.c

vtime.o: vtime.c
	x86_64-w64-mingw32-gcc -c vtime.c

clean:
	rm *o *exe Vengeance
