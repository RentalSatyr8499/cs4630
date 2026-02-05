all:
	gcc -Os -o infect1 infect1.c
	gcc -Os -o infect2 infect2.c
	gcc -Os -o infect3 infect3.c

	cp ./obs/target1.exe .
	cp ./obs/target2.exe .
	cp ./obs/target3a.exe .

infect:
	./infect1 target1.exe
	./infect2 target2.exe
	./infect3 target3a.exe
	
	chmod 777 target1.exe
	chmod 777 target2.exe
	chmod 777 target3a.exe

clean:
	rm -f *.o infect1 infect2 infect3 target*.exe
