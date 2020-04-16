//
// Created by netbook on 29/03/2020.
//
#include "biblio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


/// ---------------------------------------- fonctions Tlv --------------------------------



int32_t tlv_chain_add_int32(tlv_chain *a, int32_t x)
{
    return add_tlv(a, 1, 4, &x);
}

// add tlv object which contains null terminated string
int32_t tlv_chain_add_str( tlv_chain *a, const char *str)
{
    return add_tlv(a, 9, strlen(str) + 1, str);
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

int32_t parserV1(const unsigned char *src,  tlv_chain *list, uint16_t length)
{
    if(list == NULL || src == NULL)
        return -1;

    // we want an empty chain of tlv
    if(list->used != 0)
        return -1;

    int16_t counter = 0;
    printf("parser la\n");
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
void sendSerieTlvNode(Data *datalist,int sockfd){
Triplet *tmp=datalist->tete;
SA sin;
while (tmp!=NULL){
    sin.sin_family = AF_INET;
   // sin.sin_port = htons(tmp.);

    tmp=tmp->suivant;
}
}

void parserTLV(Data *datalist,Voisins *voisins,tlv_chain *list,int index,SA *addr,int sockfd){
    int length;
    char *body;
    char *data;
    char* paquet;
    char *id;
    char *h;
    uint16_t seq;
    SA servaddr;
    unsigned char chainbuff[1024]={0} ;
    uint16_t l = 0;
    switch (list->object[index].type){
        case PAD_1:
            printf("type 0\n");
            break;
        case PAD_N:
            printf("type 1\n");
            break;
        case NEIGH_R:
            printf("type 2");
            //Ce TLV demande au récepteur d’envoyer un TLVNeighbour
            Voisin *v=hasardVoisin(voisins);
            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons(v->port);
            servaddr.sin_addr.s_addr = inet_addr(v->ip);
            tlv_chain neigh;
            memset(&neigh, 0, sizeof(neigh));
            data=malloc(strlen(v->ip)+2);
            memcpy(data,v->ip,strlen(v->ip));
            memcpy(&data[strlen(v->ip)],&v->port,2);
            add_tlv(&neigh,NEIGH,strlen(data)+2,data);
            tlv_chain_toBuff(&neigh, chainbuff, &l);
            paquet=chain2Paquet(chainbuff,l);
            sendto(sockfd,(const char *)paquet,PAQ_SIZE,0,(const SA *)&servaddr,sizeof(servaddr));
            printf("\n paquet type 3 sent  \n");
            break;
        case NEIGH:
            printf("type 3\n");
            tlv_chain netHash;
            //Ce TLV contient l’adresse d’un voisin vivant de l’émetteur
             data=list->object[index].data;
             int16_t len=list->object[index].size;
             uint16_t port=0;
             char *ip=malloc(len-2);
             memcpy(ip,data,len-2);
             memcpy(&port,&data[len-2],2);
            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons(port);
            servaddr.sin_addr.s_addr = inet_addr(ip);
            char *net=NetworkHash(datalist);
            add_tlv(&netHash,NET_HASH,strlen(net),net);
            tlv_chain_toBuff(&netHash,chainbuff, &l);
            paquet=chain2Paquet(chainbuff,l);
            sendto(sockfd,(const char *)paquet,PAQ_SIZE,0,(const SA *)&servaddr,sizeof(servaddr));
            printf("\n paquet type 4 sent  \n");
            break;
        case NET_HASH:
            printf("type 4");
            //Ce TLV indique l’idée que se fait l’émetteur de l’état actuel du réseau
            char *myHash=NetworkHash(datalist);
            if(strcmp(myHash,list->object[index].data)!=0){
                tlv_chain netstate;
                memset(&netstate, 0, sizeof(netstate));
                add_tlv(&netstate,NET_STATE_R,0,NULL);
                tlv_chain_toBuff(&netstate, chainbuff, &l);
                paquet=chain2Paquet(chainbuff,l);
                sendto(sockfd,(const char *)paquet,PAQ_SIZE,0,(const SA *)addr,sizeof(addr));
                printf("\n paquet type 5 sent  \n");
            }

            break;
        case NET_STATE_R:
            printf("type 5");
            //Ce TLV demande au récepteur d’envoyer une série de TLVNode Hash
            sendSerieTlvNode(datalist,sockfd);
            break;
        case NODE_HASH:
            printf("type 6");
           //Ce TLV est envoyé en réponse à un TLVNetwork State Request.
            id=malloc(sizeof(char)*8);
            memcpy(id,list->object[index].data,8);
            memcpy(&seq,&(list->object[index].data[8]),2);
            seq=ntohs(seq);
            h=malloc(sizeof(char)*16);
            memcpy(h,&(list->object[index].data[10]),16);
            break;
        case NODE_STATE_R:
            printf("type 7");
            //Ce TLV demande au récepteur d’envoyer un TLVNode Statedécrivant l’état du nœud indiquépar le champNode Id
            break;
        case NODE_STATE:
            printf("type 8");
             // Ce TLV est envoyé en réponse à un TLVNode State Request
            break;
        case WARNING:
            printf("type 9");
            break;
        default:
            printf("default type");
    }

}


char* chain2Paquet (char *chain,uint16_t  len)
{
    printf(" chain 2 paquet");

    char *res=malloc(sizeof(char)*PAQ_SIZE);
   res[0]=0b01011111;
   res[1]=1;
    memcpy(res+2,&len,2);
    memcpy(res+4,chain,len);


    return res;

}

void parserPaquet(Data *datalist,Voisins *voisins,char *buf,SA *addr,int sockfd){
    int index=0;
    tlv_chain list;
    memset(&list, 0, sizeof(list));
    uint16_t len;
    char taile[2];

    if(buf[0]==95 && buf[1]==1)
    {
        uint16_t port=ntohs(addr->sin_port);
        char *ip=inet_ntoa(addr->sin_addr);
        if(rechercheEmetteur(voisins,ip,port)==1 || voisins->used==15){
            printf(" \nerror ---- : le paquet est ignoré \n");
            return;
        }
        miseAjourVoisins(voisins,ip,port);
        memcpy(&len,&buf[2],2);

        parserV1(buf+4,&list,len);
      while(index < list.used)
      {
        parserTLV(datalist,voisins,&list,index,addr,sockfd);
        index++;
      }

    }
    else 
    {
        printf(" error ---- : paquet non reconnu");
    }
    
    

}

int rechercheEmetteur(Voisins *voisins,char *ip, uint16_t port){
    int count=0;
    while(count<Max_voisin){
        if(voisins->TableDevoisins[count]!=NULL){
            if(voisins->TableDevoisins[count]->port==port && strcmp(ip,voisins->TableDevoisins[count]->ip)==0){
                return 1;
            }
        }
        count++;
    }
    return 0;
}
void addVoisin(Voisins *voisins,char *ip, uint16_t port){
    struct timespec now;
    int rc=clock_gettime(CLOCK_REALTIME,&now);
    if(rc<0)
    {
        perror("erreur geettime");
        exit(EXIT_FAILURE);
    }
    int count=0;
    while(count<Max_voisin){
        if(voisins->TableDevoisins[count]==NULL){
            voisins->TableDevoisins[count]=malloc(sizeof(Voisin));
            voisins->TableDevoisins[count]->port=port;
            voisins->TableDevoisins[count]->ip=malloc(sizeof(ip));
            strcpy(voisins->TableDevoisins[count]->ip,ip);
            voisins->TableDevoisins[count]->date=now;
            voisins->TableDevoisins[count]->permanent=0;
            return;
        }
        count++;
    }
}
void modifierVoisin(Voisins *voisins,char *ip, uint16_t port){
    struct timespec now;
    int rc=clock_gettime(CLOCK_REALTIME,&now);
    if(rc<0)
    {
        perror("erreur geettime");
        exit(EXIT_FAILURE);
    }
    int count=0;
    while(count<Max_voisin){
        if(voisins->TableDevoisins[count]!=NULL){
            if(voisins->TableDevoisins[count]->port==port && strcmp(ip,voisins->TableDevoisins[count]->ip)==0){
                voisins->TableDevoisins[count]->date=now;
            }
        }
        count++;
    }
}
void miseAjourVoisins(Voisins *voisins,char *ip, uint16_t port){
    if(rechercheEmetteur(voisins,ip,port)==1){
        modifierVoisin(voisins,ip,port);
    }else{
        addVoisin(voisins,ip,port);
    }
}
Voisin *hasardVoisin(Voisins *voisins){
    int nbgen=rand()%voisins->used;
    int count=0;
    while(count<Max_voisin){
        if(voisins->TableDevoisins[count]!=NULL){
            if(count==nbgen){
                return voisins->TableDevoisins[count];
            }
        }
        count++;
    }

}
void moinsde5voisins(Voisins *voisins,int sockfd){
    if (voisins->used<5){
        struct sockaddr_in servaddr;
        Voisin *v=hasardVoisin(voisins);
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(v->port);
        servaddr.sin_addr.s_addr = inet_addr(v->ip);
        int n, len;
        tlv_chain chain1, chain2;
        memset(&chain1, 0, sizeof(chain1));
        memset(&chain2, 0, sizeof(chain2));
        unsigned char chainbuff[1024]={0} ;
        uint16_t l = 0;
        add_tlv(&chain1,NEIGH_R,0,NULL);
        tlv_chain_toBuff(&chain1, chainbuff, &l);
        char* paquet=chain2Paquet(chainbuff,l);
        sendto(sockfd, (char *)paquet, sizeof(paquet),0, (const struct sockaddr *) &servaddr,sizeof(servaddr));
        printf("paquet  sent.\n");
    }
}

void miseAjour20s(Voisins *voisins){
    while (1){
        parcoursVoisins(voisins);
        sleep(20);
    }
}

void parcoursVoisins(Voisins *voisins){

struct timespec now;
int rc=clock_gettime(CLOCK_REALTIME,&now);
if(rc<0)
{
    perror("erreur geettime");
    exit(EXIT_FAILURE);
}


    int count=0;
    while(count<Max_voisin){
        if(voisins->TableDevoisins[count]!=NULL){
            if(now.tv_sec-voisins->TableDevoisins[count]->date.tv_sec>70 && voisins->TableDevoisins[count]->permanent==0){
                voisins->TableDevoisins[count]=NULL;
            }
        }
        count++;
    }
}

char *Hash(char *data){
    unsigned char *d = SHA256(data, strlen(data), 0);
    char *res=malloc(16*sizeof(char));
    memcpy(res,d,16);
    return res;
}
char *concatTriplet(Triplet *d){
    uint16_t seq=htons(d->numDeSeq);
    int len=strlen(d->data)+10;
    char *data=malloc(sizeof(char)*len);
    memcpy(data,d->id,8);
    memcpy(&data[8],&seq,2);
    memcpy(&data[10],d->data,strlen(d->data));
    return data;
}
char *NetworkHash(Data *datalist){
    Triplet *tmp=datalist->tete;
    char *hashi;
    int i=0;
    char *hashNet=malloc(16*datalist->used);
    while (tmp!=NULL){
        hashi=Hash(concatTriplet(tmp));
        memcpy(&hashNet[i],hashi,16);
        i+=16;
        tmp=tmp->suivant;
    }
    return Hash(hashNet);
}
void insererData(Data *datalist,char *id,uint16_t seq,char *donnee){
    Triplet *t=malloc(sizeof(Triplet));
    t->numDeSeq=seq;
    memcpy(t->id,id,8);
    t->data=malloc(strlen(donnee));
    memcpy(t->data,donnee,strlen(donnee));
    t->suivant=NULL;
    Triplet *tmp=datalist->tete;
    Triplet *pres=NULL;
    if(tmp==NULL){
        datalist->tete=t;
        datalist->used=1;
        return;
    }
    int cnt=1;
    while (tmp!=NULL && cnt==1){
        if(strcmp(tmp->id,id)>0){
            cnt=0;
        }else {
            pres = tmp;
            tmp = tmp->suivant;
        }
    }
    if(tmp==NULL){
        pres->suivant=t;
    }else if(pres==NULL){
        t->suivant=tmp;
        datalist->tete=t;
    } else{
        pres->suivant=t;
        t->suivant=tmp;
    }
    datalist->used+=1;
}


