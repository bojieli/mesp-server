/* for windows MFC */

#include "http_win.h"

tcpclient* tcpclient_singleton(const char* server, const int port) {
    static CInternetSession session;
    static CHttpConnection* pConnection;
    static int connected = 1;
    if (connected)
        return pConnection;

    session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000 * 10);
    session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF, 1000);
    session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 2);
    pConnection = session.GetHttpConnection(server, port);
    connected = 1; 
    return pConnection;
}

int http_post(tcpclient *pConnection, char* path, size_t reqlen, char *request, char **response, size_t *recvlen) {

    static CHttpFile* pFile = pConnection->OpenRequest( CHttpConnection::HTTP_VERB_POST,
                                                 path,
                                                 NULL,
                                                 1,
                                                 NULL,
                                                 TEXT("HTTP/1.1"),
                                                 INTERNET_FLAG_RELOAD);

    CString szHeaders = L"Connection: keep-alive";
    
    pFile->SendRequest( szHeaders,
                        szHeaders.getLength(),
                        request,
                        reqlen);
    
    DWORD dwRet;
    pFile->QueryInfoStatusCode(dwRet);
    
    if(dwRet != HTTP_STATUS_OK) {
        CString errText;
        errText.Format(L"POST error. error code: %d", dwRet);
        AfxMessageBox(errText);
        return dwRet;
    }
    else {
#define BUFFER_SIZE 16384
        *recvlen = 0;
        *response = malloc(BUFFER_SIZE);
        size_t numread;
        while ((numread = pFile->Read(response+(*recvlen), BUFFER_SIZE)) > 0) {
            *recvlen += numread;
            *response = realloc(*response, (*recvlen)+BUFFER_SIZE);
        }
        *response = realloc(*response, *recvlen + 1);
        (*response)[*recvlen] = '\0';
    }
    
    pFile->Close();
    return 0;
}
