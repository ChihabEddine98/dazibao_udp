// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include "biblio.h"
#include <sys/select.h>




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
        memset(chainbuff,0,1024);
        uint16_t l = 0;
        char *paquet;
        int n=0;

        tlv_chain neigh;
        memset(&neigh, 0, sizeof(neigh));
        add_tlv(&neigh,NEIGH_R,0,NULL);
        tlv_chain_toBuff(&neigh, chainbuff, &l);
        paquet=chain2Paquet(chainbuff,l);
        n=sendto(sockfd,(const char *)paquet,l+4,0,(const SA *)&servaddr,sizeof(servaddr));
        printf("\n paquet type 2 sent  \n");

        // unsigned char req[1024];
        // memset(req, 0, 6);
        // req[0] = 95;
        // req[1] = 1;

        // const char *s = "szczaw";
        // unsigned char *d = SHA256(s, strlen(s), 0)


       // n=sendto(sockfd, (char *)paquet,l+4,MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
       if (n>0) printf("paquet  sent.\n");
        // thread20s
        pthread_t thread1;
        arg *arg1=malloc(sizeof(arg));
        arg1->sockfd=sockfd;
        arg1->arg1=voisins;
       if(pthread_create(&thread1, NULL,miseAjour20s,arg1) == -1) {
        perror("pthread_create");
        return EXIT_FAILURE;
       }

    fd_set sockLibres,sockActuels;

    FD_ZERO(&sockActuels);
    FD_SET(sockfd,&sockActuels);

    int recvLen=0;


    while (1){
       // n = recvfrom(sockfd, (char *)buffer, MAXLINE,0, (struct sockaddr *) &servaddr,&len);

       sockLibres=sockActuels;

       if(select(FD_SETSIZE,&sockLibres,NULL,NULL,NULL)< 0)
       {
           perror(" Select bug !");
           exit(1);
       }
       for (size_t i = 0; i < FD_SETSIZE; i++)
       {
            if (FD_ISSET(i,&sockLibres))
            {
                if(i== sockfd)
                {
                    n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (SA*) &servaddr,&recvLen);
                    printf(" \n recieved %d bytes \n",n);
                    if( n < 0 )
                    {
                        perror(" Recv From bug ");

                    }
                    else
                    {
                        // Parser Maquet heree !!! 
                        parserPaquet(datatable,voisins,buffer, &servaddr,sockfd);
                    }
                    
                }
            }

       }
       


    }

/*
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     0, (struct sockaddr *) &servaddr,
                     &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);
*/
//pthread_join(&thread1,NULL);
        close(sockfd);
        return 0;
    }
