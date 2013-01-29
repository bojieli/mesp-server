#ifndef _HTTP_CLIENT_ 
#define _HTTP_CLIENT_

#include <netinet/in.h>  
#include <sys/socket.h>

typedef struct { 
    int socket;
    int remote_port;     
    char remote_ip[16];  
    struct sockaddr_in _addr; 
    int connected;       
} tcpclient;

int tcpclient_create(tcpclient *pclient,const char *host, int port);
int tcpclient_conn(tcpclient *pclient);
int tcpclient_recv(tcpclient *pclient,char **lpbuff,int size);
int tcpclient_send(tcpclient *pclient,char *buff,int size);
int tcpclient_close(tcpclient *pclient);

int http_post(tcpclient *pclient, char *path, int reqlen, char *request, char **response, int *recvlen);

#endif
