```c
// TLV data structure

struct TLV
{	int8_t type; // type
	char  * data; // pointer to data
	int16_t size; // size of data
};
```

```c
// TLV chain data structure. Contains array of (50) tlv
// objects.
struct tlv_chain
{
    tlv  object[MAX_TLV_OBJECTS];
    uint8_t used; // keep track of tlv elements used
};
```
```c
struct Triplet {
   unsigned char  id[8];
    uint16_t numDeSeq;
    char *data;
    int incr;
    struct Triplet *suivant;
};
```
```c
struct Voisin {
    char *ip ;
    uint16_t port;
    struct timespec date;
    int permanent;
}Voisin;
```
```c
struct Voisins  {
    Voisin *TableDevoisins[Max_voisin];
    uint8_t used;
};
```
```c
struct Data{
    Triplet *tete;
    uint8_t used;
};
```
```c
struct arg {
    Voisins *arg1;
    int sockfd;
};
```
```c
struct arg2 {
    Voisins *arg1;
    Data *datalist;
    int sockfd;
};
```






