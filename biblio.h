//
// Created by netbook on 29/03/2020.
//

#ifndef PROJETR_BIBLIO_H
#define PROJETR_BIBLIO_H

#define PORT	 1717
#define MAXLINE 1024

/// ------------------------- structure de données ------------------------------
typedef struct  {
    char id[8];
    char numDeSeq[2];
    char data[192];
}Triplets;

typedef struct {
char id[8];
Triplets *TableDeData;
}node;

typedef struct TLV{
  char type;
  char length;
  char *body;
}TLV;

///---------------------------------------------------------------------------------
void parserTLV(char type,char *buf,int *index);
void parserPaquet(char *buf);


#endif //PROJETR_BIBLIO_H
