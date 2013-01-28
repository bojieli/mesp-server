#ifndef __CLOUDSTO_H__
#define __CLOUDSTO_H__

#include <string.h>
#include "http.h"

#define CLOUD_SERVER "127.0.0.1"
#define CLOUD_PORT 8080

int cloudsto_post(char* path, int sendlen, char* sendbuf, char** recvbuf);
int do_test();

#define MAXLEN 10000
char sendbuf[MAXLEN];
char *sendptr;
char *recvbuf;

#define PUTCHAR(var) do{ \
    *sendptr++ = (unsigned char)(var); \
} while(0)
#define PUTLONG(var) do{ \
    *sendptr++ = (var)>>24; \
    *sendptr++ = ((var)<<8)>>24; \
    *sendptr++ = ((var)<<16)>>24; \
    *sendptr++ = ((var)<<24)>>24; \
} while(0)
#define PUTS(str) do{ \
    int len = strlen(str); \
    PUTCHAR(len); \
    memcpy(sendptr, (str), len); \
    sendptr += len; \
} while(0)

#define GETLONG(var) int var = (recvbuf[0]<<24) + (recvbuf[1]<<16) + (recvbuf[2]<<8) + recvbuf[3]; recvbuf+=4;
#define GETCHAR(var) char var = *recvbuf++;
#define GETSTR(str) \
    char* str = malloc(*recvbuf)+1; \
    memcpy(str, recvbuf+1, *recvbuf); \
    str[*recvbuf] = '\0'; \
    recvbuf += *recvbuf+1;

#define prompt(msg, ...) printf(msg "\n > ", ##__VA_ARGS__);

#endif
