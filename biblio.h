//
// Created by netbook on 29/03/2020.
//

#ifndef PROJETR_BIBLIO_H
#define PROJETR_BIBLIO_H
#include <stdint.h>

#define PORT	 1717
#define MAXLINE 1024
#define MAX_TLV_OBJECTS 50
#define MAX_DATA 100

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
    uint64_t idNode;
    Triplets *TableDeData[MAX_DATA];
}node;



///---------------------------------------------------------------------------------
int32_t tlv_chain_add_int32(tlv_chain *a, int32_t x);
int32_t tlv_chain_add_str( tlv_chain *a, const char *str);
int32_t add_tlv( tlv_chain *a, unsigned char type, int16_t size, const unsigned char *bytes);
int32_t tlv_chain_toBuff( tlv_chain *a, unsigned char *dest, int32_t *count);
int32_t Buff_to_tlv_chain(const unsigned char *src,  tlv_chain *dest, int32_t length);
int32_t tlv_chain_print(tlv_chain *a);
int32_t  free_tlv_list( tlv_chain *a);


void parserTLV(char type,char *buf,int *index);
void parserPaquet(char *buf);


#endif //PROJETR_BIBLIO_H
