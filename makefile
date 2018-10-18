all:	Vengeance

Vengeance:	main.o  magicmoves.o nonslidingmoves.o movegen.o make_unmake.o utility.o perft.o dynarray.o
	gcc main.o magicmoves.o nonslidingmoves.o movegen.o make_unmake.o utility.o perft.o dynarray.o -O3 -o Vengeance

main.o:	main.c
	gcc -c main.c

movegen.o:	movegen.c
	gcc -c movegen.c

magicmoves.o:	magicmoves.c
	gcc -c magicmoves.c

nonslidingmoves.o:	nonslidingmoves.c
	gcc -c nonslidingmoves.c

dynarray.o:    dynarray.c
	gcc -c dynarray.c

utility.o:	utility.c
	gcc -c utility.c

make_unmake.o:	make_unmake.c
	gcc -c make_unmake.c

perft.o:	perft.c
	gcc -c perft.c

clean:
	rm *o Vengeance
