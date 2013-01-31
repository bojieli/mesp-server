#ifndef __CLOUDSTO_H__
#define __CLOUDSTO_H__

#include <string.h>
#include "http.h"

#define CLOUD_SERVER "127.0.0.1"
#define CLOUD_PORT 8080

int cloudsto_post(char* path, size_t sendlen, unsigned char* sendbuf, size_t* recvlen, unsigned char** recvbuf);
int do_test();

#define MAXLEN 10000
extern unsigned char sendbuf[MAXLEN];
extern unsigned char *sendptr;
extern unsigned char *recvbuf;
extern unsigned char *recvend;
extern char *errmsg;
extern const char *CLOUD_BASE;
extern const unsigned char CLIENT_VERSION;

#define PUTINIT() do{ \
    sendptr = sendbuf; \
    memset(sendbuf, 0, sizeof(sendbuf)); \
    *sendptr++ = (unsigned char)CLIENT_VERSION; \
} while(0)
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
    size_t len = strlen(str); \
    PUTCHAR(len); \
    memcpy(sendptr, (str), len); \
    sendptr += len; \
} while(0)
#define CLOUD_POST(path) do { \
    size_t recvlen; \
    cloudsto_post((char*)path, sendptr-sendbuf, (unsigned char*)sendbuf, &recvlen, (unsigned char**)&recvbuf);\
    recvend = recvbuf + recvlen; \
} while(0)

inline int GETLONG() {
    int ret = (recvbuf[0]<<24) + (recvbuf[1]<<16) + (recvbuf[2]<<8) + recvbuf[3]; 
    recvbuf+=4;
    return ret;
}
#define GETCHAR() (*recvbuf++)
inline char* GETS() {
    char* str = malloc(*recvbuf)+1;
    memcpy(str, recvbuf+1, *recvbuf);
    str[(int)*recvbuf] = '\0';
    recvbuf += *recvbuf+1;
    return str;
}

#define prompt(msg, ...) printf(msg "\n > ", ##__VA_ARGS__);

#endif
