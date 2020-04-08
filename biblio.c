//
// Created by netbook on 29/03/2020.
//
#include "biblio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *Getbody(char *buf,int *index,int length){
    char val[length];
    int i=0;
    while(i<length){
        val[i]=buf[*index];
        *index=*index+1;
    }
    return val;
}

char *GetIp(char *buf,int *index){
    char ip[16];
    int i=0;
    while(i<16){
        ip[i]=buf[*index];
        *index=*index+1;
    }
    return ip;
}
char *GetPort(char *buf,int *index){
    char port[2];
    int i=0;
    while(i<2){
        port[i]=buf[*index];
        *index=*index+1;
    }
    return port;
}
void parserTLV(char type,char *buf,int *index){
    int length;
    char *body;
    switch (type){
        case '0':
            printf("type 0");
            *index=*index+1;
            break;
        case '1':
            printf("type 1");
            *index=*index+1;
            length=buf[*index]- '0';
            *index=*index+1+length;
            break;
        case '2':
            printf("type 2");
            *index=*index+1;
            length=buf[*index]- '0';
            *index=*index+1;
            //Ce TLV demande au récepteur d’envoyer un TLVNeighbour

            break;
        case '3':
            printf("type 3");
            *index=*index+1;
            length=buf[*index]- '0';
            *index=*index+1;
            char *ip=GetIp(buf,index);
            char *port=GetPort(buf,index);
            //Ce TLV contient l’adresse d’un voisin vivant de l’émetteur

            break;
        case '4':
            printf("type 4");
            *index=*index+1;
            length=buf[*index]- '0';
            *index=*index+1;
            char *NetworkHash=Getbody(buf,index,length);
            //Ce TLV indique l’idée que se fait l’émetteur de l’état actuel du réseau

            break;
        case '5':
            printf("type 5");
            *index=*index+1;
            length=buf[*index]- '0';
            *index=*index+1;
            //Ce TLV demande au récepteur d’envoyer une série de TLVNode Hash
            break;
        case '6':
            printf("type 6");
            *index=*index+1;
            length=buf[*index]- '0';
            *index=*index+1;
            body=Getbody(buf,index,length);
           //Ce TLV est envoyé en réponse à un TLVNetwork State Request.

            break;
        case '7':
            printf("type 7");
            *index=*index+1;
            length=buf[*index]- '0';
            *index=*index+1;
            char *id=Getbody(buf,index,length);
            //Ce TLV demande au récepteur d’envoyer un TLVNode Statedécrivant l’état du nœud indiquépar le champNode Id

            break;
        case '8':
            printf("type 8");
            *index=*index+1;
            length=buf[*index]- '0';
            *index=*index+1;
             body=Getbody(buf,index,length);
             // Ce TLV est envoyé en réponse à un TLVNode State Request
            break;
        case '9':
            printf("type 8");
            *index=*index+1;
            length=buf[*index]- '0';
            *index=*index+1;
            body=Getbody(buf,index,length);
            printf("warning :%s",body);
            break;
        default:
            printf("default warning!!");
    }

}


void parserPaquet(char *buf){
    int index=0;
    while(index<strlen(buf) && index<MAXLINE){
        parserTLV(buf[index],buf,&index);
    }

}


/// ---------------------------------------- fonctions Tlv --------------------------------



int32_t tlv_chain_add_int32(tlv_chain *a, int32_t x)
{
    return add_tlv(a, 1, 4, &x);
}

// add tlv object which contains null terminated string
int32_t tlv_chain_add_str( tlv_chain *a, const char *str)
{
    return add_tlv(a, 2, strlen(str) + 1, str);
}

int32_t add_tlv( tlv_chain *a, unsigned char type, int16_t size, const unsigned char *bytes)
{
    if(a == NULL || bytes == NULL)
        return -1;

    // all elements used in chain?
    if(a->used == MAX_TLV_OBJECTS)
        return -1;

    int index = a->used;
    a->object[index].type = type;
    a->object[index].size = size;
    a->object[index].data = malloc(size);
    memcpy(a->object[index].data, bytes, size);

    // increase number of tlv objects used in this chain
    a->used++;

    // success
    return 0;
}

int32_t free_tlv_list( tlv_chain *a)
{
    if(a == NULL)
        return -1;

    for(int i =0; i < a->used; i++)
    {
        free(a->object[i].data);

        a->object[i].data = NULL;
    }

    return 0;
}

// serialize the tlv chain into byte array
int32_t tlv_chain_toBuff( tlv_chain *a, unsigned char *dest, int32_t *count)
{
    if(a == NULL || dest == NULL)
        return -1;

    // Number of bytes serialized
    int32_t counter = 0;

    for(int i = 0; i < a->used; i++)
    {
        dest[counter] = a->object[i].type;
        counter++;

        memcpy(&dest[counter], &a->object[i].size, 2);
        counter += 2;

        memcpy(&dest[counter], a->object[i].data, a->object[i].size);
        counter += a->object[i].size;
    }

    // Return number of bytes serialized
    *count = counter;

    // success
    return 0;
}

int32_t Buff_to_tlv_chain(const unsigned char *src,  tlv_chain *dest, int32_t length)
{
    if(dest == NULL || src == NULL)
        return -1;

    // we want an empty chain of tlv
    if(dest->used != 0)
        return -1;

    int32_t counter = 0;
    while(counter < length)
    {
        if(dest->used == MAX_TLV_OBJECTS)
            return -1;
        // deserialize type
        dest->object[dest->used].type = src[counter];
        counter++;

        // deserialize size
        memcpy(&dest->object[dest->used].size, &src[counter], 2);
        counter+=2;

        // deserialize data itself, only if data is not NULL
        if(dest->object[dest->used].size > 0)
        {
            dest->object[dest->used].data = malloc(dest->object[dest->used].size);
            memcpy(dest->object[dest->used].data, &src[counter], dest->object[dest->used].size);
            counter += dest->object[dest->used].size;
        }else
        {
            dest->object[dest->used].data = NULL;
        }

        // increase number of tlv objects reconstructed
        dest->used++;
    }

    // success
    return 0;

}



int32_t afficher_tlv_chain( tlv_chain *a)
{
    if(a == NULL)
        return -1;

    // go through each used tlv object in the chain
    for(int i =0; i < a->used; i++)
    {

        if(a->object[i].type == 1)
        {
            // int32
            int32_t x;
            memcpy(&x, a->object[i].data, sizeof(int32_t));
            printf("%d \n",x);

        }else if(a->object[i].type == 2){
            // string
            printf("%s \n",a->object[i].data);
        }
    }


    return 0;
}

