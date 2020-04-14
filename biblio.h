//
// Created by netbook on 29/03/2020.
//

#ifndef PROJETR_BIBLIO_H
#define PROJETR_BIBLIO_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERVER_PORT 1212
#define SERVER_IP   "81.194.27.155"
#define PAQ_SIZE   1024
#define MAXLINE 1024
#define MAX_TLV_OBJECTS 60
#define MAX_DATA 100
#define Max_voisin 15

#define PAD_1 0
#define PAD_N 1
#define NEIGH_R 2
#define NEIGH 3
#define NET_HASH 4
#define NET_STATE_R 5
#define NODE_HASH 6
#define NODE_STATE_R 7  
#define NODE_STATE 8
#define WARNING 9


typedef struct sockaddr_in SA ;





/// ------------------------- structure de données ------------------------------
// TLV data structure
typedef struct
{
    int8_t type;    // type
    uint8_t * data; // pointer to data
    int16_t size;   // size of data
}tlv;

// TLV chain data structure. Contains array of (50) tlv
// objects.
typedef struct
{
    tlv  object[50];
    uint8_t used; // keep track of tlv elements used
}tlv_chain;

typedef struct  {
    uint64_t id;
    uint16_t numDeSeq;
    uint8_t *data;
}Triplets;
typedef struct {
    char *ip ;
    uint16_t port;
    struct timespec date;
    int permanent;
}Voisin;

typedef struct  {
    Voisin *TableDevoisins[Max_voisin];
    uint8_t used;
}Voisins;
typedef  struct{
    Triplets *TableDeData[MAX_DATA];
    uint8_t used;
}Data;



///---------------------------------------------------------------------------------
int32_t tlv_chain_add_int32(tlv_chain *a, int32_t x);
int32_t tlv_chain_add_str( tlv_chain *a, const char *str);
int32_t add_tlv( tlv_chain *a, unsigned char type, int16_t size, const unsigned char *bytes);
int32_t tlv_chain_toBuff( tlv_chain *a, unsigned char *dest, int32_t *count);
int32_t Buff_to_tlv_chain(const unsigned char *src,  tlv_chain *dest, int32_t length);
int32_t afficher_tlv_chain(tlv_chain *a);
int32_t  free_tlv_list( tlv_chain *a);
int32_t parserV1(const unsigned char *src,  tlv_chain *list, uint16_t length);
char* chain2Paquet (char *chain,uint16_t  len);

void parserTLV(tlv_chain *list,int index,SA *addr,int sockfd);
void parserPaquet(char *buf,SA *addr,int sockfd);

void parcoursVoisins(Voisins *voisins);
int rechercheEmetteur(Voisins *voisins,char *ip, uint16_t port);
void addVoisin(Voisins *voisins,char *ip, uint16_t port);
void miseAjourVoisins(Voisins *voisins,char *ip, uint16_t port);
void modifierVoisin(Voisins *voisins,char *ip, uint16_t port);


#endif //PROJETR_BIBLIO_H
