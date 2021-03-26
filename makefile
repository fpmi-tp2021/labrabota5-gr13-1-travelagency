all: bin/app

obj/fuctionality.o: src/fuctionality.c
	gcc -c -o obj/fuctionality.o src/fuctionality.c 


obj/main.o: src/main.c obj/fuctionality.o
	gcc -c -o obj/main.o src/main.c

bin/app: obj/fuctionality.o obj/main.o
	gcc -o bin/app obj/fuctionality.o obj/main.o

check:
 shell: /usr/bin/bash -e {0}
distcheck:
 shell: /usr/bin/bash -e {0}

clean:
	rm -rf obj/*.o