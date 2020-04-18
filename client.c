// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "biblio.h"
#include <pthread.h>

#define PORT	 8080
#define MAXLINE 1024

// Driver code
int main() {


        int sockfd;
        char buffer[MAXLINE];
        char *hello = "Hello from client";
        struct sockaddr_in	 servaddr;

        // Creating socket file descriptor
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));

        // Filling server information
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERVER_PORT);
        servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
        Data *datatable=malloc(sizeof(Data));
        insererData(datatable,"0e:7e:d5",1,"hello world");
        Voisins *voisins=malloc(sizeof(Voisins));
        voisins->used=0;

    unsigned char *chainbuff=malloc(1024) ;
    uint16_t l = 0;
char *paquet;
    int n=sendto(sockfd, (char *)paquet,l+4,MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
    if (n>0) printf("paquet  sent.\n");
    // thread20s
    pthread_t thread1;
    /*if(pthread_create(&thread1, NULL,miseAjour20s,NULL) == -1) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }
*/

    while (1){
       // n = recvfrom(sockfd, (char *)buffer, MAXLINE,0, (struct sockaddr *) &servaddr,&len);
        if (n>0) {
            pthread_t thread2;
            // thread 2

        }


    }

/*
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     0, (struct sockaddr *) &servaddr,
                     &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);
*/
        close(sockfd);
        return 0;
    }
