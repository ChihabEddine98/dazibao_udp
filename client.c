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
    unsigned char *chainbuff=malloc(1024) ;
    uint16_t l = 0;

    char* data="Aymen T'es recherché en mailling list :D";
    char* nID="0e:7e:d5";
    u_int16_t seqNo=19241924;
    char *nHash=Hash("aymen");

    tlv_chain node_state;
    memset(&node_state, 0, sizeof(node_state));

    char* toSend=malloc(26+strlen(data));
    memcpy(toSend,nID,8);
    memcpy(toSend+8,&seqNo,2);
    memcpy(toSend+10,nHash,16);
    memcpy(toSend+26,data,strlen(data));


    printf("\n toSEND : %d\n",strlen(toSend));
    
    // tlv_chain_add_str(&chain1,"ya wediii jay7a");
    add_tlv(&node_state,NODE_STATE,strlen(toSend),toSend);



    tlv_chain_toBuff(&node_state, chainbuff, &l);


    char* paquet=chain2Paquet(chainbuff,l);

  //  printf("\n paquet : %d\n",strlen(paquet));

unsigned char buff[1024];
int size=0;
nodestate(buff,data,nID,seqNo,nHash,&size);
    printf("\n sizzzze : %d\n",size);

    sendto(sockfd, (char *)paquet,size,
               MSG_CONFIRM, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));
        printf("paquet  sent.\n");
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
