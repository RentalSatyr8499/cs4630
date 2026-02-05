all:
	gcc -Os -o infect1 infect1.c
	gcc -Os -o infect2 infect2.c
	cp ./obs/target1.exe .
	cp ./obs/target2.exe .
infect:
	./infect1 target1.exe
	./infect2 target2.exe
	chmod 777 target1.exe
	chmod 777 target2.exe
clean:
	rm -f *.o infect1 infect2 target*.exe
