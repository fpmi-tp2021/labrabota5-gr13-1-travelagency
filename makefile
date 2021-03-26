all: bin/app

obj/main.o: src/main.c
	gcc -c src/main.c -o obj/main.o

obj/fuctionality.o: src/fuctionality.c
	gcc -c src/fuctionality.c -o obj/fuctionality.o

obj/sqlite3.o: src/sqlite3.c
	gcc -c src/sqlite3.c -o obj/sqlite3.o

bin/app: obj/sqlite3.o obj/fuctionality.o obj/main.o
	gcc -o bin/app obj/sqlite3.o obj/fuctionality.o obj/main.o

obj/fuctionality.o: src/fuctionality.c
	gcc src/fuctionality.c -c -o obj/fuctionality.o

obj/main.o: src/main.c
	gcc  src/main.c -c -o obj/main.o

bin/app: obj/fuctionality.o obj/main.o
	gcc -o bin/app obj/fuctionality.o obj/main.o


check:
 shell: /usr/bin/bash -e {0}
distcheck:
 shell: /usr/bin/bash -e {0}

clean:
	rm -rf obj/*.o