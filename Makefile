all : boring custom multi

boring : boring.o 
	cc -o boring boring.o

custom : custom.o
	cc -o custom custom.o

multi : multi.o
	cc -o multi multi.o

boring.o : boring.c 
	cc -c boring.c
custom.o : custom.c
	cc -c custom.c
multi.o : multi.c
	cc -c multi.c
clean :
	rm boring custom multi boring.o custom.o multi.o
