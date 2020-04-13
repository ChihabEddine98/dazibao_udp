//
// Created by netbook on 29/03/2020.
//
#include "biblio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/// ---------------------------------------- fonctions Tlv --------------------------------



int32_t tlv_chain_add_int32(tlv_chain *a, int32_t x)
{
    return add_tlv(a, 1, 4, &x);
}

// add tlv object which contains null terminated string
int32_t tlv_chain_add_str( tlv_chain *a, const char *str)
{
    return add_tlv(a, 3, strlen(str) + 1, str);
}


int32_t add_tlv( tlv_chain *a, unsigned char type, int16_t size, const unsigned char *bytes)
{
    if(a == NULL )
        return -1;

    // all elements used in chain?
    if(a->used == MAX_TLV_OBJECTS)
        return -1;

    int index = a->used;
    a->object[index].type = type;
    a->object[index].size = size;
    if(type!=0 && type!=5 && type!=2) {
        a->object[index].data = malloc(size);
        memcpy(a->object[index].data, bytes, size);
    } else{ a->object[index].data =NULL;}

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
       if(a->object[i].type!=0) {
           memcpy(&dest[counter], &a->object[i].size, 2);
           counter += 2;
           if(a->object[i].type!=2 && a->object[i].type!=5) {
               memcpy(&dest[counter], a->object[i].data, a->object[i].size);
               counter += a->object[i].size;
           }
       }
    }

    // Return number of bytes serialized
    *count = counter;
    dest[counter+1]='\0';

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

        if(a->object[i].type ==0 )
        {

            printf("type1=%d \n",a->object[i].type);

        }else {
            // string
            printf("type1 =%d, %s \n",a->object[i].type,a->object[i].data);
        }
    }


    return 0;
}

int32_t parserV1(const unsigned char *src,  tlv_chain *list, uint16_t length)
{
    if(list == NULL || src == NULL)
        return -1;

    // we want an empty chain of tlv
    if(list->used != 0)
        return -1;

    int16_t counter = 0;
    printf("verifier %d\n",src[0]==0);
    while(counter < length)
    {
        if(list->used == MAX_TLV_OBJECTS)
            return -1;
        // deserialize type
        list->object[list->used].type = src[counter];
        counter++;

        if(list->object[list->used].type!=0) {
            // deserialize size
            memcpy(&list->object[list->used].size, &src[counter], 2);
            counter += 2;
            if(list->object[list->used].type!=2 && list->object[list->used].type!=5 ) {

                // deserialize data itself, only if data is not NULL
                if (list->object[list->used].size > 0) {
                    list->object[list->used].data = malloc(list->object[list->used].size);
                    memcpy(list->object[list->used].data, &src[counter], list->object[list->used].size);
                    counter += list->object[list->used].size;
                } else {
                    list->object[list->used].data = NULL;
                }
            } else
                list->object[list->used].data = NULL;
        }else{
            list->object[list->used].size = 0;
            list->object[list->used].data = NULL;
        }

        // increase number of tlv objects reconstructed
        list->used++;
    }

    // success
    return 0;

}

///----------------------------------------------------------------------------

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
void parserTLV(tlv_chain *list,int index){
    int length;
    char *body;
    switch (list->object[index].type){
        case '0':
            printf("type 0");
            break;
        case '1':
            printf("type 1");
            break;
        case '2':
            printf("type 2");
            //Ce TLV demande au récepteur d’envoyer un TLVNeighbour

            break;
        case '3':
            printf("type 3");
            //Ce TLV contient l’adresse d’un voisin vivant de l’émetteur

            break;
        case '4':
            printf("type 4");
            //Ce TLV indique l’idée que se fait l’émetteur de l’état actuel du réseau

            break;
        case '5':
            printf("type 5");
            //Ce TLV demande au récepteur d’envoyer une série de TLVNode Hash
            break;
        case '6':
            printf("type 6");
           //Ce TLV est envoyé en réponse à un TLVNetwork State Request.

            break;
        case '7':
            printf("type 7");
            //Ce TLV demande au récepteur d’envoyer un TLVNode Statedécrivant l’état du nœud indiquépar le champNode Id

            break;
        case '8':
            printf("type 8");
             // Ce TLV est envoyé en réponse à un TLVNode State Request
            break;
        case '9':
            printf("type 8");
            break;
        default:
            printf("default type 9 : warning!!");
    }

}

char* chain2Paquet (char *chain,uint16_t  len)
{
    printf(" chain 2 paquet");

    char *res=malloc(sizeof(char)*PAQ_SIZE);
   res[0]=0b01011111;
   res[1]=1;
    memcpy(&res[2],&len,2);
    memcpy(&res[4],chain,len);


    return res;

}

void parserPaquet(char *buf){
    int index=0;
    tlv_chain list;
    uint16_t len;

    if(buf[0]==95 && buf[1]==1)
    {
        printf("on est la \n");
      memcpy(&len,&buf[2],2);
        printf("la taille %d\n",len);

        parserV1(buf,&list,len);
        printf("la taille de la list %d\n",list.used);

        afficher_tlv_chain(&list);

     /* while(index < list.used)
      {
        parserTLV(&list,index);
        index++;
      }
      */
    }
    else 
    {
        printf(" error ---- : paquet non reconnu");
    }
    
    

}


