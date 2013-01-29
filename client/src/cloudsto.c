#include <stdio.h>
#include <stdlib.h>
#include "cloudsto.h"

char sendbuf[MAXLEN];
char *sendptr;
char *recvbuf;
char *errmsg;

int cloudsto_post(char* path, int sendlen, char* sendbuf, char** recvbuf) {
    printf("Sending %d bytes...\n", sendlen);
    tcpclient client;
    tcpclient_create(&client, CLOUD_SERVER, CLOUD_PORT);
    int recvlen;
    char *fullpath = malloc(strlen(path) + strlen(CLOUD_BASE) + 2);
    strcpy(fullpath, CLOUD_BASE);
    strcat(fullpath, path);
    int errorno = http_post(&client, fullpath, sendlen, sendbuf, recvbuf, &recvlen);
    free(fullpath);
    if (errorno) {
        printf("Network error: %d\n", errorno);
        if (*recvbuf && **recvbuf)
            printf("Error message: %s\n", *recvbuf);
        return 1;
    }
    printf("Received %d bytes\n", recvlen);
    return 0;
}

int do_test() {
    if (cloudsto_post((char*)CLOUD_BASE, 0, sendbuf, &recvbuf))
        return 1;
    printf("Response: status %d\n", GETCHAR());
    return 0;
}
