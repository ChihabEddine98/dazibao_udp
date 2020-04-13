#include <stdio.h>
#include <stdlib.h>
#include "biblio.h"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "biblio.h"


int main() {
    tlv_chain chain1, chain2;
    memset(&chain1, 0, sizeof(chain1));
    memset(&chain2, 0, sizeof(chain2));
    unsigned char chainbuff[1024]={0} ;
    int32_t l = 0;
    add_tlv(&chain1,0,0,NULL);
    tlv_chain_add_str(&chain1, "998967-44-33-44-12");



   /* tlv_chain_add_int32(&chain1, 31144);
    tlv_chain_add_str(&chain1, "george");
    tlv_chain_add_int32(&chain1, 7);
    tlv_chain_add_str(&chain1, "998967-44-33-44-12");
    tlv_chain_add_str(&chain1, "Grand Chamption Atlanta; details: Ave12");
    tlv_chain_add_int32(&chain1, 7900);
*/
    // serialization/deserialization test
    tlv_chain_toBuff(&chain1, chainbuff, &l);
    // parserV1(chainbuff, &chain2, l);

    char* paquet=chain2Paquet(chainbuff);
    parserPaquet(paquet);
   // Buff_to_tlv_chain(chainbuff, &chain2, l);

    // print the tlv chain contents
    afficher_tlv_chain(&chain2);

    // free each chain
    free(paquet);
    free_tlv_list(&chain1);
    free_tlv_list(&chain2);
    return 0;
    /*
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) {
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
        printf("Client : %s\n", buffer);
        sendto(sockfd, (const char *)hello, strlen(hello),
               MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
               len);
        printf("Hello message sent.\n");

    return 0;
     */
}
