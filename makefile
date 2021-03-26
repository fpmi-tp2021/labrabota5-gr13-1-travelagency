bin/main: obj/main.o obj/fuctionality.o
	gcc -o bin/main obj/main.o obj/fuctionality.o

obj/main.o: src/main.c
	gcc -c src/main.c -o obj/main.o

obj/fuctionality.o: src/fuctionality.c
	gcc -c src/fuctionality.c -o obj/fuctionality.o

clean:
	rm -f obj/*.o bin/main

check:
 shell: /usr/bin/bash -e {0}
distcheck:
 shell: /usr/bin/bash -e {0}