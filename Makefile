HEADERS = biblio.h

all: projet

projet:   main.o biblio.o client.o server.o
			gcc  -o projet    main.o biblio.o client.o server.o

main: main.o biblio.o
		gcc -o main main.o biblio.o -lssl -lcrypto

server: server.o biblio.o
		gcc -o server server.o biblio.o

peer: client.o biblio.o
		gcc -o peer client.o biblio.o  -lssl -lcrypto -pthread

main.o: main.c biblio.c $(HEADERS) 
			gcc -o main.o -c main.c 

server.o: server.c $(HEADERS)
			gcc -o server.o -c server.c

client.o: peer.c $(HEADERS)
			gcc -o client.o -c peer.c -Wall

biblio.o: biblio.c 
			gcc -o biblio.o -c biblio.c

clean:
		rm -rf *.o

clean_all: clean
		rm -rf projet client server main peer
		
		