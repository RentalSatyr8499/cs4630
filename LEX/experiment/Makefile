all:
	flex -o test.c test.l 
	gcc -Wall -g -O -o test test.c -lfl
	chmod 777 test
clean:
	rm -f test.c test