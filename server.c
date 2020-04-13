
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "biblio.h"

#define PORT	 8080
#define MAXLINE 1024


int main(int argc, char const *argv[])
{
    /*
    char buffer[1024];
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(1717);
    sin.sin_addr.s_addr = INADDR_ANY;

    int client = socket(AF_INET, SOCK_DGRAM, 0);
    if (client < 0)
    {
        printf("error client\n");
        exit(1);
    }
    int to = sizeof(sin);
    //parserPaquet(paquet,&sin,client);
  /*  unsigned char req[1024];
    memset(req, 0, 6);
    req[0] = 95;
    req[1] = 1;

    // const char *s = "szczaw";
    // unsigned char *d = SHA256(s, strlen(s), 0);

    req[2] = 0;
    req[3] = 2;
    req[4] = 2;
    req[5] = 0;   

    //parserPaquet(paquet);


int n;

    while (1) {

        /*
         * recvfrom: receive a UDP datagram from a client

        bzero(buffer,PAQ_SIZE );
        n = recvfrom(client, buffer, PAQ_SIZE, 0,
                     (struct sockaddr *) &sin, &to);
        if (n < 0)
            perror("ERROR in recvfrom");
        else {
           printf("message recived\n");
            parserPaquet(buffer,&sin,client);
        }
    }
    return 0;
     */





        int sockfd;
        char buffer[MAXLINE];
        char *hello = "Hello from server";
        struct sockaddr_in servaddr, cliaddr;

        // Creating socket file descriptor
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(PORT);

        // Bind the socket with the server address
        if ( bind(sockfd, (const struct sockaddr *)&servaddr,
                  sizeof(servaddr)) < 0 )
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        int len, n;

        len = sizeof(cliaddr); //len is value/resuslt

        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                     &len);
        buffer[n] = '\0';
        parserPaquet(buffer,&cliaddr,sockfd);
        /*
        printf("Client : %s\n", buffer);
        sendto(sockfd, (const char *)hello, strlen(hello),
               MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
               len);
        printf("Hello message sent.\n");
*/
        return 0;

}


