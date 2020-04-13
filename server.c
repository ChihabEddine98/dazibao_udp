
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#define SERVER_PORT 1212
#define SERVER_IP   "81.194.27.155"


int main(int argc, char const *argv[])
{


    char buffer[1024];
    struct sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERVER_PORT);
    sin.sin_addr.s_addr = inet_addr(SERVER_IP);

    int client = socket(AF_INET, SOCK_DGRAM, 0);
    if (client < 0)
    {
        printf("error client\n");
        exit(1);
    }
    int to = sizeof(sin);
    unsigned char req[1024];
    memset(req, 0, 6);
    req[0] = 95;
    req[1] = 1;

    // const char *s = "szczaw";
    // unsigned char *d = SHA256(s, strlen(s), 0);

    req[2] = 0;
    req[3] = 2;
    req[4] = 0b0000010;
    req[5] = 0b0000000;   

    if ((sendto(client, req, sizeof(req), 0, (struct sockaddr *)&sin, to)) < 0)
    {
        printf(" error sendto\n");
        exit(1);
    }
    printf("le message : %s a ete envoye\n", req);
 
    return 0;
}


