#include <stdio.h>
#include <stdlib.h>
#include "cloudsto.h"
#include "base64.h"

char sendbuf[MAXLEN];
char *sendptr;
char *recvbuf;
char *errmsg;

int cloudsto_post(char* path, int sendlen, char* sendbuf, int* recvlen, char** recvbuf) {
#ifdef DEBUG
    printf("Sending %d bytes...\n", sendlen);
#endif
    tcpclient client;
    tcpclient_create(&client, CLOUD_SERVER, CLOUD_PORT);

    char *fullpath = malloc(strlen(path) + strlen(CLOUD_BASE) + 2);
    strcpy(fullpath, CLOUD_BASE);
    strcat(fullpath, path);

    int encoded_sendlen;
    char *encoded_sendbuf = base64_encode(sendlen, sendbuf, &encode_sendlen);
    int encoded_recvlen;
    char *encoded_recvbuf;

    int errorno = http_post(&client, fullpath, encoded_sendlen, encoded_sendbuf, encoded_recvbuf, &encoded_recvlen);
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
    if (cloudsto_post((char*)CLOUD_BASE, 0, sendbuf, &recvbuf))
        return 1;
    printf("Response: status %d\n", GETCHAR());
    return 0;
}
