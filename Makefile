HEADERS = biblio.h

all: projet

projet:   biblio.o peer.o 
			gcc  -o projet   biblio.o peer.o  -lssl -lcrypto -pthread -Wall

main: main.o biblio.o
		gcc -o main main.o biblio.o -lssl -lcrypto


peer: peer.o biblio.o
		gcc -o peer peer.o biblio.o  -lssl -lcrypto -pthread

main.o: main.c biblio.c $(HEADERS) 
			gcc -o main.o -c main.c 

peer.o: peer.c $(HEADERS)
			gcc -o peer.o -c peer.c -Wall

biblio.o: biblio.c 
			gcc -o biblio.o -c biblio.c

clean:
		rm -rf *.o

clean_all: clean
		rm -rf projet main peer
		
		