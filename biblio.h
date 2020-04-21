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
#include <pthread.h>
#include <openssl/sha.h>

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
#define MY_NODE_ID 0xaa82adc85debf5dae // >>> import uuid
                                     // print(uuid.uuid1()) 
                                     


typedef struct sockaddr_in SA ;





/// ------------------------- structure de données ------------------------------
// TLV data structure
typedef struct
{
    int8_t type;    // type
    char * data; // pointer to data
    int16_t size;   // size of data
}tlv;

// TLV chain data structure. Contains array of (50) tlv
// objects.
typedef struct
{
    tlv  object[MAX_TLV_OBJECTS];
    uint8_t used; // keep track of tlv elements used
}tlv_chain;

typedef struct Triplet {
    char  id[8];
    uint16_t numDeSeq;
    char *data;
    struct Triplet *suivant;
}Triplet;
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
    Triplet *tete;
    uint8_t used;
}Data;
typedef struct {
    Voisins *arg1;
    int sockfd;
}arg;


///---------------------------------------------------------------------------------
int32_t tlv_chain_add_int32(tlv_chain *a, int32_t x);
int32_t tlv_chain_add_str( tlv_chain *a, const char *str);
int32_t add_tlv( tlv_chain *a, unsigned char type, int16_t size, const unsigned char *bytes);
int16_t tlv_chain_toBuff( tlv_chain *a, unsigned char *dest, int16_t *count);
int32_t Buff_to_tlv_chain(const unsigned char *src,  tlv_chain *dest, int32_t length);
int32_t afficher_tlv_chain(tlv_chain *a);
int32_t  free_tlv_list( tlv_chain *a);
int32_t parserV1(const unsigned char *src,  tlv_chain *list, uint16_t length);
char* chain2Paquet (char *chain,uint16_t  len);

void parserTLV(Data *datalist,Voisins *voisins,tlv_chain *list,int index,SA *addr,int sockfd);
void parserPaquet(Data *datalist,Voisins *voisins,char *buf,SA *addr,int sockfd);

void parcoursVoisins(Voisins *voisins);
void moinsde5voisins(Voisins *voisins,int sockfd);
int rechercheEmetteur(Voisins *voisins,char *ip, uint16_t port);
void addVoisin(Voisins *voisins,char *ip, uint16_t port);
void miseAjourVoisins(Voisins *voisins,char *ip, uint16_t port);
void modifierVoisin(Voisins *voisins,char *ip, uint16_t port);
Voisin *hasardVoisin(Voisins *voisins);
void insererData(Data *datalist,char *id,uint16_t seq,char *donnee);
char *NetworkHash(Data *datalist);
char *concatTriplet(Triplet *d);
char *Hash(char *data);
void *miseAjour20s(void *args);
void sendSerieTlvNode(Data *datalist,int sockfd,SA *addr);
void nodestate(char *buffer,char *data,char *id,short seq,char *hash,int *size);
void supprimerData(Data *datalist,char *id);

#endif //PROJETR_BIBLIO_H
