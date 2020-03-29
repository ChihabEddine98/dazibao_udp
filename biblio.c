//
// Created by netbook on 29/03/2020.
//
#include "biblio.h"
#include <string.h>
#include <stdio.h>

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
    while(index<strlen(buf) && index<1024){
        parserTLV(buf[index],buf,&index);
    }

}

