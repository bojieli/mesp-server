#include <stdio.h>
#include <stdlib.h>
#include "cloudsto.h"
#include "base64.h"

unsigned char sendbuf[MAXLEN];
unsigned char *sendptr;
unsigned char *recvbuf;
unsigned char *recvend;
char *errmsg;

int cloudsto_post(char* path, size_t sendlen, unsigned char* sendbuf, size_t* recvlen, unsigned char** recvbuf) {
#ifdef DEBUG
    printf("Sending %d bytes...\n", sendlen);
#endif

    char *fullpath = malloc(strlen(path) + strlen(CLOUD_BASE) + 2);
    strcpy(fullpath, CLOUD_BASE);
    strcat(fullpath, path);

    size_t encoded_sendlen;
    char *encoded_sendbuf = base64_encode(sendbuf, sendlen, &encoded_sendlen);
    size_t encoded_recvlen;
    char *encoded_recvbuf;

    int errorno = http_post(tcpclient_singleton(CLOUD_SERVER, CLOUD_PORT), fullpath, encoded_sendlen, encoded_sendbuf, &encoded_recvbuf, &encoded_recvlen);
    free(fullpath);
    *recvbuf = base64_decode(encoded_recvbuf, encoded_recvlen, recvlen);

    if (errorno) {
        printf("Network error: %d\n", errorno);
        if (*recvbuf && **recvbuf)
            printf("Error message: %s\n", *recvbuf);
        return 1;
    }
#ifdef DEBUG
    printf("Received %d bytes\n", recvlen);
#endif
    return 0;
}

int do_test() {
    size_t recvlen;
    if (cloudsto_post((char*)CLOUD_BASE, 0, sendbuf, &recvlen, &recvbuf))
        return 1;
    recvend += recvlen;
    printf("Response: status %d\n", GETCHAR());
    return 0;
}
