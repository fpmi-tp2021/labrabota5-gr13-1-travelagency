bin/app: obj/main.o build/fuctionality.o build/sqlite3.o
	gcc -o bin/app build/main.o build/fuctionality.o build/sqlite3.o -lpthread -ldl

build/main.o: src/main.c
	gcc -c src/main.c -o build/main.o

build/fuctionality.o: src/fuctionality.c
	gcc -c src/fuctionality.c -o build/fuctionality.o

build/sqlite3.o: src/sqlite3.c
	gcc -c src/sqlite3.c -o obj/sqlite3.o -lpthread -ldl

clean:
	rm -f *.o bin/app
