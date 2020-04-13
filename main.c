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
    uint16_t l = 0;
    add_tlv(&chain1,0,0,NULL);
    tlv_chain_add_str(&chain1, "998967-44-33-44-12");

    tlv_chain_toBuff(&chain1, chainbuff, &l);
    // parserV1(chainbuff, &chain2, l);

   char* paquet=chain2Paquet(chainbuff,l);
    //parserPaquet(paquet);
   // Buff_to_tlv_chain(chainbuff, &chain2, l);

    // print the tlv chain contents
    afficher_tlv_chain(&chain2);

    // free each chain
   // free(paquet);
    free_tlv_list(&chain1);
    free_tlv_list(&chain2);
    return 0;

}
