#ifndef __HTTP_H__
#define __HTTP_H__

#include <stdlib.h>

typedef HttpConnection tcpclient;

tcpclient* tcpclient_singleton(const char* server, const int port);
int http_post(tcpclient *pclient, char* path, size_t reqlen, char *request, char **response, size_t *recvlen);

#endif
