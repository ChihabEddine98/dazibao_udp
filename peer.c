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
int main(int argc,char *argv[]) {
    if(argc!=3)
    {
        perror("wrong usage");
        exit(1);
    }
    char *hostname=argv[1];
    uint16_t port=atoi(argv[2]);

        int sockfd;
        char buffer[MAXLINE];
       // char *hello = "Hello from client";
        SA	 servaddr;

        // Creating socket file descriptor
        if ( (sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));

            servaddr.sin6_family = AF_INET6;

            int val=1;
            //int poly_port;
            setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));

            val=0;
            //int poly;
            setsockopt(sockfd,IPPROTO_IPV6,IPV6_V6ONLY,&val,sizeof(val));


            servaddr.sin6_port = htons(SERVER_PORT);
            int p;
            p=inet_pton(AF_INET6,SERVER_IP,&servaddr.sin6_addr);
            if(p==-1)
            {
                perror(" ip err ");
            }


        Data *datatable=malloc(sizeof(Data));
        insererData(datatable,MY_ID,10345,"hello world");
        Voisins *voisins=malloc(sizeof(Voisins));
        voisins->used=0;
        for (int j = 0; j <15 ; j++) {
          voisins->TableDevoisins[j]=NULL;
        }
        initaddr(voisins,hostname,port);
        int n=0;
        socklen_t  len;

    pthread_t thread1;
        arg *arg1=malloc(sizeof(arg));
        arg1->sockfd=sockfd;
        arg1->arg1=voisins;
       if(pthread_create(&thread1, NULL,miseAjour20s,arg1) == -1) {
        perror("pthread_create");
        return EXIT_FAILURE;
       }

    pthread_t thread2;
    arg2 *a=malloc(sizeof(arg2));
    a->sockfd=sockfd;
    a->arg1=voisins;
    a->datalist=datatable;
    if(pthread_create(&thread2,NULL,sendNet20s,a)==-1) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }


    fd_set sockLibres,sockActuels;

    FD_ZERO(&sockActuels);
    FD_SET(sockfd,&sockActuels);



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
                    n = recvfrom(sockfd, (char *)buffer, MAXLINE,0, (struct sockaddr *) &servaddr,&len);
                    if( n < 0 )
                    {
                        perror(" Recv From bug !");

                    }
                    else
                    {
                        // Parser Maquet heree !!! 
                        parserPaquet(datatable,voisins,buffer, &servaddr,sockfd,n);
                    }
                    
                }
            }

       }
       


    }

    }
