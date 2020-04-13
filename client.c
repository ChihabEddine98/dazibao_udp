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

#define PORT	 8080
#define MAXLINE 1024

// Driver code
int main() {
    /*
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
	servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;


    int n, len;

    unsigned char req[1024];
    memset(req, 0, 6);
    req[0] = 95;
    req[1] = 1;

    // const char *s = "szczaw";
    // unsigned char *d = SHA256(s, strlen(s), 0);

    req[2] = 0;
    req[3] = 2;
    req[4] = 2;
    req[5] = 0;


	if(sendto(sockfd, (const char *)req,strlen(req),0, (const struct sockaddr *) &servaddr,
			sizeof(servaddr))>0) {
        printf("message sent.\n");
    }
	while(1) {
        n = recvfrom(sockfd, (char *) buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *) &servaddr,
                     &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);
    }
	close(sockfd);
	return 0;
     */

    // Client side implementation of UDP client-server model



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

        int n, len;
    tlv_chain chain1, chain2;
    memset(&chain1, 0, sizeof(chain1));
    memset(&chain2, 0, sizeof(chain2));
    unsigned char chainbuff[1024]={0} ;
    uint16_t l = 0;

    tlv_chain_add_str(&chain1,"ya wediii jay7a");
    //add_tlv(&chain1,WARNING,strlen(msg),msg);


    tlv_chain_toBuff(&chain1, chainbuff, &l);
    char* paquet=chain2Paquet(chainbuff,l);



    sendto(sockfd, (char *)paquet, sizeof(paquet),
               MSG_CONFIRM, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));
        printf("paquet  sent.\n");

        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     0, (struct sockaddr *) &servaddr,
                     &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);

        close(sockfd);
        return 0;
    }
