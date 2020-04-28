//
// Created by netbook on 29/03/2020.
//
#include "biblio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "math.h"


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
int16_t tlv_chain_toBuff( tlv_chain *a, unsigned char *dest, int16_t *count)
{
    if(a == NULL || dest == NULL)
        return -1;

    // Number of bytes serialized
    int16_t counter = 0;
    short lBe;
char len[2];
    for(int i = 0; i < a->used; i++)
    {
        dest[counter] = a->object[i].type;
        counter++;
       if(a->object[i].type!=0) {
          
           lBe=htons(a->object[i].size);
           memcpy(len,&lBe,2);
           memcpy(&dest[counter], &len[1], 1);
           counter += 1;
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
    printf("\nparser la and length =%d\n",length);
    while(counter < length)
    {
        if(list->used == MAX_TLV_OBJECTS)
            return -1;
        // deserialize type
        list->object[list->used].type = src[counter];
        printf("\n type V1=%d",list->object[list->used].type);

        counter++;

        if(list->object[list->used].type!=0) {
            // deserialize size
            memcpy(&list->object[list->used].size, &src[counter], 1);
            printf("\n tailleV1=%d",list->object[list->used].size);
            counter += 1;
            if(list->object[list->used].type!=2 && list->object[list->used].type!=5 ) {

                // deserialize data itself, only if data is not NULL
                if (list->object[list->used].size > 0) {
                    list->object[list->used].data = malloc(list->object[list->used].size);
                    memcpy(list->object[list->used].data, &src[counter], list->object[list->used].size);
                    printf("\n dataV1=%s",list->object[list->used].data);

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


Triplet *Getdataintable(Data *datalist,char id[8]){
    Triplet *tmp=datalist->tete;
    Triplet *res=NULL;
    while (tmp!=NULL){
        printf("\n [%s] [%s] cmp %d",id ,tmp->id,memcmp(id,tmp->id,8));
        if(memcmp(tmp->id,id,8)==0) return tmp;
        tmp=tmp->suivant;
    }
    return res;

}

void supprimerData(Data *datalist,char *id){
    Triplet *tmp=datalist->tete;
    Triplet *pres=NULL;
    int count=0;
    while(tmp!=NULL && count==0) {
        if (strcmp(id, tmp->id) == 0) {
            count = 1;
        } else {
            pres = tmp;
            tmp = tmp->suivant;
        }
    }
    if(pres==NULL){
        datalist->tete=datalist->tete->suivant;
    } else if (tmp!=NULL){
        pres->suivant=tmp->suivant;
    }
    return;
}

void NodeState(Data *datalist,char *node,int len,SA *addr,int sockfd){
    char *id=malloc(sizeof(char)*8);
    memcpy(id,node,8);
    uint16_t seq;
    memcpy(&seq,&node[8],2);
    seq=ntohs(seq);
    char *h=malloc(sizeof(char)*16);
    memcpy(h,&node[10],16);
    len=len-26;
    char *data=malloc(sizeof(char)*len);
    memcpy(data,&node[26],len);
    int num=pow(2,16);
    Triplet *d=Getdataintable(datalist,id);
    if(d!=NULL){
        if (strcmp(h,Hash(concatTriplet(d)))!=0){
         if(strcmp(id,"0e:7e:d5")==0){// cas 1
             if((seq-d->numDeSeq)%num<32768){
                 d->numDeSeq=(seq+1)%num;
             }
         }else{// cas 2
             if((seq-d->numDeSeq)%num<32768){
                 d->numDeSeq=(seq+1)%num;
                 supprimerData(datalist,id);
                 insererData(datalist,id,seq,d->data);
             }
         }

        }else{
            char *msg="erreur dans le hash";
            sendWarning(msg,addr,sockfd);
       /// rien a faire
        }
    }else{
        insererData(datalist,id,seq,data);
    }


}
int hex2dec(char hexVal[]) 
{    
    int len = strlen(hexVal); 
      
    // Initializing base value to 1, i.e 16^0 
    int base = 1; 
      
    int dec_val = 0; 
      
    // Extracting characters as digits from last character 
    for (int i=len-1; i>=0; i--) 
    {    
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
                  
            // incrementing base by power 
            base = base * 16; 
        } 
  
        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (hexVal[i]>='a' && hexVal[i]<='f') 
        { 
            dec_val += (hexVal[i] - 55)*base; 
          
            // incrementing base by power 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
} 

int nombreChiffres ( int nombre )
{
	int i = 1;
	if (nombre < 0)
	{
		nombre = -nombre;
	}
	while (nombre >= 10)
	{
		nombre /= 10;
		i++;
	}
/* 
 * Avec une boucle for
 * 	for (i=1; nombre >= 10; nombre /= 10, i++) ;
 */
	return i;
}
unsigned char* parseIp(unsigned char* ipHex)
{
    // for (int i = 0; i <16 ; i++) {
    //     printf(" %02x",ipHex[i]);
    //     }

    // printf("\n");
    unsigned char* ipRes=malloc(45*sizeof(char));
    unsigned char* initMapped="00000000000000000000ffff";
    unsigned char* initOurIp=malloc(12);
    char cmp[24],cmp2[32];
    int j=0;

    for (int i = 0; i <16 ; i++) {

        if (i < 12 && j < 24  )
        {
          initOurIp[i]=ipHex[i];
          sprintf(&cmp[j],"%02x",initOurIp[i]);
          j+=2;

        }


        
           
    }
        
    
        if(strcmp(cmp,initMapped)==0)
        {
            j=7;
            memcpy(ipRes,"::ffff:",7);
            for (size_t i = 12; i < 16; i++)
            {
               int k=ipHex[i];
               if(i==15)
               {
                  sprintf(&ipRes[j],"%d",k);
                  j+=nombreChiffres(k);

               }
               else
               {
                  sprintf(&ipRes[j],"%d.",k);
                  j+=nombreChiffres(k)+1;
               }
            }
            return ipRes;

            // for (size_t i = 12,j=0; i < 16,j<8 ; i++,j+=2)
            // {
            //    sprintf(&ipRes[i],"%d.",ipHex[i]);
            
            // }
            
        }
        else{
            j=0;
            unsigned char* h=malloc(4);
            for (size_t i = 0; i < 16; i+=2)
            {
              
               if(i==14)
               {
                  sprintf(&ipRes[j],"%02x%02x",ipHex[i],ipHex[i+1]);
                  j+=4;

               }
               else
               {
                  sprintf(&ipRes[j],"%02x%02x:",ipHex[i],ipHex[i+1]);
                  j+=5;
               }
            }

              return ipRes;
        }

        
        
        // if ipv6 or ipv4 mapped !

    return ipRes;
}
void sendWarning(char *msg,int sockfd,SA *addr){
    SA servaddr;
    servaddr.sin6_family = AF_INET6;
    uint16_t port=ntohs(addr->sin6_port);
    char ip[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6,&addr->sin6_addr,ip,sizeof(ip));
    servaddr.sin6_port = htons(port);
    int p1=inet_pton(AF_INET6,ip,&servaddr.sin6_addr);
    if(p1==-1)
    {
        perror(" ip err ");
    }
    tlv_chain chaine;
    unsigned char chainbuff[1024]={0} ;
    uint16_t l = 0;
    memset(&chaine, 0, sizeof(chaine));
    add_tlv(&chaine,WARNING,strlen(msg),msg);
    tlv_chain_toBuff(&chaine, chainbuff, &l);
    char *paquet=chain2Paquet(chainbuff,l);
    if (sendto(sockfd,(const char *)paquet,l+4,MSG_CONFIRM,(const SA *)&servaddr,sizeof(servaddr))>0)
        printf("\n paquet type 9 sent  \n");
    else printf("\n error , paquet type 9 non sent  \n");



}

void sendSerieTlvNode(Data *datalist,int sockfd,SA *addr){
    Triplet *tmp=datalist->tete;
    SA servaddr;
    servaddr.sin6_family = AF_INET6;

    int val=1;
   // int poly_port=setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));


    val=0;
    //int poly=setsockopt(sockfd,IPPROTO_IPV6,IPV6_V6ONLY,&val,sizeof(val));
    uint16_t port=ntohs(addr->sin6_port);
    char ip[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6,&addr->sin6_addr,ip,sizeof(ip));
    servaddr.sin6_port = htons(port);
    int p1=inet_pton(AF_INET6,ip,&servaddr.sin6_addr);
    if(p1==-1)
    {
        perror(" ip err ");
    }




    tlv_chain chaine;
    memset(&chaine, 0, sizeof(chaine));
    char *h;
    char *data=malloc(sizeof(char)*26);
    unsigned char chainbuff[1024]={0} ;
    uint16_t l = 0;
    int len;
    printf("\n port %d",addr->sin6_port);
    while (tmp!=NULL){
        h=Hash(concatTriplet(tmp));
        uint16_t seq=htons(tmp->numDeSeq);
        memcpy(data,tmp->id,8);
        memcpy(&data[8],&seq,2);
        memcpy(&data[10],h,16);
        add_tlv(&chaine,NODE_HASH,26,data);
        tmp=tmp->suivant;
    }
    tlv_chain_toBuff(&chaine, chainbuff, &l);
    char *paquet=chain2Paquet(chainbuff,l);
    printf("\n the L is %d \n",l);

    if(sendto(sockfd,(const char *)paquet,l+4,MSG_CONFIRM,(const SA *)&servaddr,sizeof(servaddr))>0)
        printf("\n serie TLV Node Hash 6 sent  \n");
    else
        printf("\n error , serie TLV Node Hash 6 non sent  \n");
}

void parserTLV(Data *datalist,Voisins *voisins,tlv_chain *list,int index,SA *addr,int sockfd){
    int length;
    char *body;
    char *data;
    char* paquet;
    char *id;
    char *h;
    int len;
    int p1;
    int val;
    int poly_port;
    int poly;
    uint16_t seq;
    SA servaddr;
    servaddr.sin6_family = AF_INET6;
    val=1;
  //  poly_port=setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));

    val=0;
    //poly=setsockopt(sockfd,IPPROTO_IPV6,IPV6_V6ONLY,&val,sizeof(val));

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
            servaddr.sin6_port = htons(v->port);
            int p;
            memcpy(&servaddr.sin6_addr,v->ip,16);

          //  p1=inet_pton(AF_INET6,v->ip,&servaddr.sin6_addr);
           // if(p1==-1)
           // {
             //   perror(" ip err ");
            //}
            tlv_chain neigh;
            memset(&neigh, 0, sizeof(neigh));
            data=malloc(strlen(v->ip)+2);
            memcpy(data,v->ip,strlen(v->ip));
            p=v->port;
            short p2=htons(p);
            memcpy(&data[strlen(v->ip)],&p2,2);
            add_tlv(&neigh,NEIGH,strlen(data),data);
            tlv_chain_toBuff(&neigh, chainbuff, &l);
            paquet=chain2Paquet(chainbuff,l);
            if (sendto(sockfd,(const char *)paquet,l+4,MSG_CONFIRM,(const SA *)&servaddr,sizeof(servaddr))>0)
             printf("\n paquet type 3 sent  \n");
            else printf("\n error , paquet type 3 non sent  \n");
            break;
        case NEIGH:
            printf("type 3 Recieved :\n");
            tlv_chain netHash;
            memset(&netHash, 0, sizeof(netHash));
            printf("\n ip=%s",list->object[index].data);
            //Ce TLV contient l’adresse d’un voisin vivant de l’émetteur
            data=list->object[index].data;
             int8_t length=list->object[index].size;
            printf("\n len=%d",l);
            uint16_t port=0;
            unsigned char *ip=malloc(16*sizeof(char));
            memcpy(ip,data,length-2);
            memcpy(&port,&data[length-2],2);
            short port2=ntohs(port);
            printf("\n port =%d ",port);
            printf("\n ip formatted : %s\n",parseIp(ip));

                //   for (int i = 0; i <16 ; i++) {
                //      printf(" %02x",ip[i]);
                //    }
            servaddr.sin6_family = AF_INET6;
            // servaddr.sin6_port = htons(port);
            // p1=inet_pton(AF_INET6,parseIp(ip),&servaddr.sin6_addr);
            servaddr.sin6_port = htons(SERVER_PORT);
            p1=inet_pton(AF_INET6,SERVER_IP,&servaddr.sin6_addr);
            if(p1==-1)
            {
                perror(" ip err ");
            }
            char *net=NetworkHash(datalist);
            add_tlv(&netHash,NET_HASH,strlen(net),net);
            tlv_chain_toBuff(&netHash,chainbuff, &l);
            paquet=chain2Paquet(chainbuff,l);
            if(sendto(sockfd,(const char *)paquet,l+4,MSG_CONFIRM,(const SA *)&servaddr,sizeof(servaddr))>0)
            {
                    printf("\n paquet type 4 sent  \n");
            }
            break;
        case NET_HASH:
            printf("type 4");
            //Ce TLV indique l’idée que se fait l’émetteur de l’état actuel du réseau
            port=ntohs(addr->sin6_port);
            // char *ip=inet_ntop(addr->sin6_addr);
            ip[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6,&addr->sin6_addr,ip,sizeof(ip));
            servaddr.sin6_port = htons(port);
            char *myHash=NetworkHash(datalist);
            if(strcmp(myHash,list->object[index].data)!=0){
                tlv_chain netstate;
                memset(&netstate, 0, sizeof(netstate));
                add_tlv(&netstate,NET_STATE_R,0,NULL);
                tlv_chain_toBuff(&netstate, chainbuff, &l);
                paquet=chain2Paquet(chainbuff,l);
                if(sendto(sockfd,(const char *)paquet,l+4,MSG_CONFIRM,(const SA *)&servaddr,sizeof(servaddr))>0)
                  printf("\n paquet type 5 sent  \n");
                else
                    printf("\n error , paquet type 5 non sent  \n");

            }

            break;
        case NET_STATE_R:
            printf("type 5");
            //Ce TLV demande au récepteur d’envoyer une série de TLVNode Hash
            sendSerieTlvNode(datalist,sockfd,addr);
            break;
        case NODE_HASH:
            printf("type 6");

            port=ntohs(addr->sin6_port);
            // char *ip=inet_ntop(addr->sin6_addr);
            ip[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6,&addr->sin6_addr,ip,sizeof(ip));
            servaddr.sin6_port = htons(port);
           //Ce TLV est envoyé en réponse à un TLVNetwork State Request.
            id=malloc(sizeof(char)*8);
            memcpy(id,list->object[index].data,8);
            memcpy(&seq,&(list->object[index].data[8]),2);
            seq=ntohs(seq);
            h=malloc(sizeof(char)*16);
            memcpy(h,&(list->object[index].data[10]),16);
            printf("\n hhhhh type 6666 ");
           Triplet *d=Getdataintable(datalist,id);
            if(d==NULL){
                /// rien a faire
            } else if (strcmp(h,Hash(concatTriplet(d)))!=0){
                tlv_chain netstatereq;
                memset(&netstatereq, 0, sizeof(netstatereq));
                add_tlv(&netstatereq,NODE_STATE_R,8,id);
                tlv_chain_toBuff(&netstatereq, chainbuff, &l);
                paquet=chain2Paquet(chainbuff,l);
                if(sendto(sockfd,(const char *)paquet,l+4,MSG_CONFIRM,(const SA *)&servaddr,sizeof(servaddr))>0)
                 printf("\n paquet type 7 sent  \n");
                else  printf("\n error , paquet type 7 non sent  \n");
            } else {
                /// rien a faire
            }
            break;
        case NODE_STATE_R:
            printf("type 7");
            //Ce TLV demande au récepteur d’envoyer un TLVNode Statedécrivant l’état du nœud indiquépar le champNode Id
            char *idnode=list->object[index].data;

            port=ntohs(addr->sin6_port);
            // char *ip=inet_ntop(addr->sin6_addr);
            ip[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6,&addr->sin6_addr,ip,sizeof(ip));
            servaddr.sin6_port = htons(port);
            tlv_chain node_state;
            memset(&node_state, 0, sizeof(node_state));
            Triplet *d1=Getdataintable(datalist,idnode);
            if (d1!=NULL) {
                int tailletosend=26 + strlen(d1->data);
                char *toSend = malloc(tailletosend*sizeof(char));
                memcpy(toSend, idnode, 8);
                uint16_t seqno =htons(d1->numDeSeq);
                memcpy(&toSend[8], &seqno, 2);
                char *nHash = Hash(concatTriplet(d1));
                printf("\nHAsh :%s",nHash);
                memcpy(&toSend[10], nHash, 16);
                memcpy(&toSend[26], d1->data, strlen(d1->data));
                printf("\nla taille de tosend est :%d",strlen(toSend));
                add_tlv(&node_state, NODE_STATE, tailletosend, toSend);
                tlv_chain_toBuff(&node_state, chainbuff, &l);
                paquet = chain2Paquet(chainbuff,l);
                printf("\nla taille l est:%d",l);
                if (sendto(sockfd, (const char *) paquet,l + 4, 0, (const SA *) &servaddr, sizeof(servaddr)) > 0)
                    printf("\n paquet type 8 sent  \n");
                else printf("\n error , paquet type 8 non sent  \n");
            } else printf("\n data non trouve  \n");


            break;
        case NODE_STATE:
            printf("type 8");
             // Ce TLV est envoyé en réponse à un TLVNode State Request
             NodeState(datalist,list->object[index].data,list->object[index].size,addr,sockfd);

            break;
        case WARNING:
            printf("\n WARNING:  \n");
            printf("- %s \n",list->object[index].data);
            break;
        default:
            printf("default type");
    }

}


char* chain2Paquet (char *chain,uint16_t  len)
{

    char *res=malloc(sizeof(char)*PAQ_SIZE);
    short lBe=htons(len);
    res[0]=0b01011111;
    res[1]=1;
    memcpy(res+2,&lBe,2);
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
        uint16_t port=ntohs(addr->sin6_port);
        // char *ip=inet_ntop(addr->sin6_addr);
        char ip[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6,&addr->sin6_addr,ip,sizeof(ip));
        if(rechercheEmetteur(voisins,ip,port)==0 && voisins->used==15){
            printf(" \nerror ---- : le paquet est ignoré \n");
            return;
        }
        printf("\n\nin the beg  ip =%s  and port =%d ",ip,port);
        miseAjourVoisins(voisins,addr,port);
        memcpy(&len,&buf[2],2);
        len=ntohs(len);
        parserV1(buf+4,&list,len);
        printf("\nused=%d\n",list.used);
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

int rechercheEmetteur(Voisins *voisins,SA *addr, uint16_t port){
    int count=0;
    while(count<Max_voisin){
        if(voisins->TableDevoisins[count]!=NULL){

            if(voisins->TableDevoisins[count]->port==port && memcmp(voisins->TableDevoisins[count]->ip,&addr->sin6_addr,16)==0){
                return 1;
            }
        }
        count++;
    }
    return 0;
}
void addVoisin(Voisins *voisins,SA *addr, uint16_t port){
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
            printf("\n ajouter ");
            voisins->TableDevoisins[count]=malloc(sizeof(Voisin));
            voisins->TableDevoisins[count]->port=port;
            voisins->TableDevoisins[count]->ip=malloc(45);
            memcpy(voisins->TableDevoisins[count]->ip,&addr->sin6_addr,16);
            voisins->TableDevoisins[count]->date=now;
            voisins->TableDevoisins[count]->permanent=0;
            voisins->used+=1;
            return;
        }
        count++;
    }
}
void modifierVoisin(Voisins *voisins,SA *addr, uint16_t port){
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
            if(voisins->TableDevoisins[count]->port==port && memcmp(voisins->TableDevoisins[count]->ip,&addr->sin6_addr,16)==0){
                printf("\nmodifier la data\n");
                voisins->TableDevoisins[count]->date=now;
            }
        }
        count++;
    }
}
void miseAjourVoisins(Voisins *voisins,SA *addr, uint16_t port){
    if(rechercheEmetteur(voisins,addr,port)==1){
        printf("\n modifier");
        modifierVoisin(voisins,addr,port);
    }else{
        printf("\n add");
        addVoisin(voisins,addr,port);
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
    if (voisins->used==0) return;
    if (voisins->used<5){
        printf("\nthe number of used %d\n",voisins->used);
        SA servaddr;
        Voisin *v=hasardVoisin(voisins);

        servaddr.sin6_family = AF_INET6;
        servaddr.sin6_port = htons(v->port);
        int p;
        p=inet_pton(AF_INET6,v->ip,&servaddr.sin6_addr);
        if(p==-1)
        {
            perror(" ip err ");
        }
        int n, len;
        tlv_chain chain1, chain2;
        memset(&chain1, 0, sizeof(chain1));
        memset(&chain2, 0, sizeof(chain2));
        unsigned char chainbuff[1024]={0} ;
        uint16_t l = 0;
        add_tlv(&chain1,NEIGH_R,0,NULL);
        tlv_chain_toBuff(&chain1, chainbuff, &l);
        char* paquet=chain2Paquet(chainbuff,l);

        if(sendto(sockfd, (char *)paquet, sizeof(paquet),0, (const struct sockaddr *) &servaddr,sizeof(servaddr))>0)
        {
            printf("paquet 2 (NEIGH_REQUEST) sent.\n");
        
        }
}}

void *miseAjour20s(void *args){
    arg *argss = (arg *)args;
    while (1){
        printf("\n on est la  and the num is %d",argss->arg1->used);
        parcoursVoisins(argss->arg1);
        moinsde5voisins(argss->arg1,argss->sockfd);
        sleep(20);
    }
   return NULL;
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
               printf("\n\ndelet");
                voisins->TableDevoisins[count]=NULL;
                voisins->used-=1;
            }
        }
        count++;
    }
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
    //memcpy(t->numDeSeq,seq,2);
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
        if(memcmp(tmp->id,id,8)>0){
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


