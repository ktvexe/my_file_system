all: main

main: myfs.o main.o
	gcc -std=c99 -o main main.o myfs.o
myfs.o: myfs.c myfs.h
	gcc -std=c99 -c myfs.c
main.o: main.c myfs.h
	gcc -std=c99 -c main.c
clean:
	rm main.o myfs.o
