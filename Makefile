all: midnight_commander

midnight_commander: mc.o
	gcc -g -o midnight_commander mc.o
	rm -f *.o  

mc.o: mc.c mc.h
	gcc -c -g mc.c


clean:
	-rm midnight_commander


