bin/app: obj/main.o obj/fuctionality.o
	gcc -o bin/app obj/Person.o obj/main.o

obj/main.o: src/main.c
	gcc -c src/main.c -o obj/main.o

obj/fuctionality.o: src/fuctionality.c
	gcc -c src/fuctionality.c -o obj/fuctionality.o

check:
 shell: /usr/bin/bash -e {0}
distcheck:
 shell: /usr/bin/bash -e {0}

clean:
	rm -rf obj/*.o