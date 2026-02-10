1:
	flex -o virus-patterns1.c virus-patterns1.l 
	gcc -Wall -g -O -o scanner virus-patterns1.c -lfl
	chmod 777 scanner
2:
	flex -o virus-patterns2.c virus-patterns2.l 
	gcc -Wall -g -O -o scanner virus-patterns2.c -lfl
	chmod 777 scanner
clean:
	rm -f scanner virus-patterns1.c virus-patterns2.c