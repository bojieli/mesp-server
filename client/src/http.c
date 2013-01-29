#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "http.h"

#define BUFFER_SIZE 1024

int tcpclient_create(tcpclient *pclient,const char *host, int port){
    struct hostent *he;

    if(pclient == NULL) return -1;
    memset(pclient,0,sizeof(tcpclient));

    if((he = gethostbyname(host))==NULL){
        return -2;
    }

    pclient->remote_port = port;
    strcpy(pclient->remote_ip,inet_ntoa( *((struct in_addr *)he->h_addr) ));

    pclient->_addr.sin_family = AF_INET;
    pclient->_addr.sin_port = htons(pclient->remote_port);
    pclient->_addr.sin_addr = *((struct in_addr *)he->h_addr);

    if((pclient->socket = socket(AF_INET,SOCK_STREAM,0))==-1){
        return -3;
    }

    return 0;
}

int tcpclient_conn(tcpclient *pclient){
    if(pclient->connected)
        return 1;

    if(connect(pclient->socket, (struct sockaddr *)&pclient->_addr,sizeof(struct sockaddr))==-1){
        return -1;
    }

    pclient->connected = 1;
    return 0;
}

int tcpclient_recv(tcpclient *pclient,char **lpbuff,int size){
    int recvnum=0,tmpres=0;
    char buff[BUFFER_SIZE];

    *lpbuff = NULL;

    while(recvnum < size || size==0){
        tmpres = recv(pclient->socket, buff,BUFFER_SIZE,0);
        if(tmpres <= 0)
            break;
        recvnum += tmpres;

        if(*lpbuff == NULL){
            *lpbuff = (char*)malloc(recvnum);
            if(*lpbuff == NULL)
                return -2;
        }else{
            *lpbuff = (char*)realloc(*lpbuff,recvnum);
            if(*lpbuff == NULL)
                return -2;
        }

        memcpy(*lpbuff+recvnum-tmpres,buff,tmpres);
    }

    return recvnum;
}

int tcpclient_send(tcpclient *pclient,char *buff,int size){
    int sent=0,tmpres=0;

    while(sent < size){
        tmpres = send(pclient->socket,buff+sent,size-sent,0);
        if(tmpres == -1){
            return -1;
        }
        sent += tmpres;
    }
    return sent;
}

int tcpclient_close(tcpclient *pclient){
    close(pclient->socket);
    pclient->connected = 0;
    return 0;
}

int http_post(tcpclient *pclient, char *path, int reqlen, char *request, char **response, int *recvlen) {

    char post[300],host[100],content_len[100];
    char *lpbuf = NULL, *ptmp = NULL;
    const char *header = "User-Agent: MESPTester\r\nConnection: close\r\n";

    sprintf(post,"POST %s HTTP/1.1\r\n",path);
    sprintf(host,"Host: %s:%d\r\n", pclient->remote_ip, pclient->remote_port);
    sprintf(content_len,"Content-Length: %d\r\n\r\n",reqlen);

    int len = strlen(post)+strlen(host)+strlen(header)+strlen(content_len)+reqlen+1;
    lpbuf = (char*)malloc(len);
    if(lpbuf==NULL)
        return -1;

    strcpy(lpbuf,post);
    strcat(lpbuf,host);
    strcat(lpbuf,header);
    strcat(lpbuf,content_len);
    memcpy(lpbuf+strlen(lpbuf),request,reqlen);

    if(!pclient->connected){
        tcpclient_conn(pclient);
    }

    if(tcpclient_send(pclient,lpbuf,len)<0){
        free(lpbuf);
        return -1;
    }
    free(lpbuf);
    lpbuf = NULL;

    int resplen = tcpclient_recv(pclient,&lpbuf,0);
    if (resplen <= 0) {
        if(lpbuf) free(lpbuf);
        return -2;
    }

    ptmp = (char*)strstr(lpbuf,"\r\n\r\n");
    if (ptmp == NULL){
        free(lpbuf);
        return -3;
    }
    ptmp += 4; /* skip \r\n */

    len = resplen - (ptmp - lpbuf);
    *response = (char*)malloc(len+1);
    if (*response == NULL) {
        free(lpbuf);
        return -1;
    }
    memset(*response, 0, len+1);
    memcpy(*response, ptmp, len);

    /* HTTP/1.0 200 OK */
    memset(post, 0, sizeof(post));
    strncpy(post, lpbuf+strlen("HTTP/1.0 "), 3);
    if (atoi(post) != 200){
        free(lpbuf);
        return atoi(post);
    }

    ptmp = (char*)strstr(lpbuf, "Content-Length:");
    free(lpbuf);
    if (ptmp != NULL) {
        ptmp += strlen("Content-Length:");
        char *ptmp2 = (char*)strstr(ptmp,"\r\n");
        if (ptmp2 != NULL) {
            memset(post,0,sizeof(post));
            strncpy(post,ptmp,ptmp2-ptmp);
            if (atoi(post) < len) {
                (*response)[atoi(post)] = '\0';
                len = atoi(post);
            }
        }
    }

    *recvlen = len;
    return 0;
}
