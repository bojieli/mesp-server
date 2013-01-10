#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http.h"

#define MAXLEN 10000
char sendbuf[MAXLEN] = {0};
char *sendptr = sendbuf;
char *recvbuf;

#define PUTCHAR(var) do{ \
    *sendptr++ = (var); \
} while(0)
#define PUTLONG(var) do{ \
    *sendptr++ = (var)>>24; \
    *sendptr++ = ((var)<<8)>>24; \
    *sendptr++ = ((var)<<16)>>24; \
    *sendptr++ = ((var)<<24)>>24; \
} while(0)
#define PUTS(str) do{ \
    int len = strlen(str); \
    PUTLONG(len); \
    memcpy(sendptr, (str), len); \
    sendptr += len; \
} while(0)

#define GETLONG(var) int var = ((*(recvbuf)++<<24) + (*(recvbuf)++<<16) + (*(recvbuf)++<<8) + (*(recvbuf)++));
#define GETCHAR(var) char var = (*(recvbuf)++);
#define GETSTR(str) \
    char* str = malloc(*recvbuf)+1; \
    memcpy(str, recvbuf+1, *recvbuf); \
    str[*recvbuf] = '\0'; \
    recvbuf += *recvbuf+1;

int cloudsto_post(char* path, int sendlen, char* sendbuf, char** recvbuf) {
    printf("Sending %d bytes...\n", sendlen);
    tcpclient client;
    tcpclient_create(&client, "127.0.0.1", 8000);
    int recvlen;
    int errorno = http_post(&client, path, sendlen, sendbuf, recvbuf, &recvlen);
    if (errorno) {
        printf("Network error: %d\n", errorno);
        return 1;
    }
    printf("Received %d bytes\n", recvlen);
    return 0;
}

int do_test() {
    if (cloudsto_post("/cloudsto/", 0, sendbuf, &recvbuf))
        return 1;
    GETCHAR(status)
    printf("Response: status %d\n", status);
    return 0;
}

int do_register() {
    char app_type, source;
    char username[MAXLEN], app_token[MAXLEN];
    printf("app_type(1~3) source(1~4) username(str) app_token(str)\n > ");
    scanf("%d%d%s%s", (int*)&app_type, (int*)&source, username, app_token);
    PUTCHAR(app_type);
    PUTCHAR(source);
    PUTS(username);
    PUTS(app_token);
    char *recvbuf = NULL;
    if (cloudsto_post("/cloudsto/register", sendptr-sendbuf, sendbuf, &recvbuf))
        return 1;
    GETCHAR(status)
    printf("Response: status = %d, ", status);
    if (status != 0)
        printf("errmsg = %s\n", recvbuf);
    else
        printf("token = %s\n", recvbuf);
    return 0;
}

int do_save() {
    char token[MAXLEN];
    int entry_num;
    char source;
    printf("token(str) entry_num(long) entry-1 entry-2...\n");
    scanf("%s%d", token, &entry_num);
    PUTS(token);
    PUTLONG(entry_num);
    while (entry_num-- > 0) {
        printf("source(byte): 1 for pedometer, 2 for temperature, 3 for humidity, 4 for ultraviolet\n > ");
        scanf("%d", (int*)&source);
        PUTCHAR(source);
        switch (source) {
            int start_time, end_time, step_count, interval, data_count, time, data;
            case 1:
                printf("start_time(long), end_time(long), step_count(long)\n > ");
                scanf("%d%d%d", &start_time, &end_time, &step_count);
                PUTLONG(start_time);
                PUTLONG(end_time);
                PUTLONG(step_count);
                break;
            case 2:
            case 3:
                printf("start_time(long), interval(long), data_count(long)\n > ");
                scanf("%d%d%d", &start_time, &interval, &data_count);
                PUTLONG(start_time);
                PUTLONG(interval);
                PUTLONG(data_count);
                printf("input %d data values\n > ", data_count);
                while (data_count-- > 0) {
                    scanf("%d", &data);
                    PUTLONG(data);
                }
                break;
            case 4:
                printf("time(long), data(long)\n > ");
                scanf("%d%d", &time, &data);
                PUTLONG(time);
                PUTLONG(data);
                break;
            default:
                printf("Invalid entry type\n");
        }
    }
    char *recvbuf = NULL;
    if (cloudsto_post("/cloudsto/receive", sendptr-sendbuf, sendbuf, &recvbuf))
        return 1;
    GETCHAR(status);
    printf("Response: status = %d, ", status);
    if (status != 0)
        printf("errmsg %s\n", recvbuf);
    else {
        GETLONG(sn);
        printf("end serial no = %d\n", sn);
    }
    return 0;
}

int do_receive() {
    char token[MAXLEN];
    int sn_begin, sn_num;
    printf("receive: token(str) sn_begin(long) sn_num(long)\n");
    scanf("%d%d", &sn_begin, &sn_num);
    PUTLONG(sn_begin);
    PUTLONG(sn_num);
    char *recvbuf = NULL;
    if (cloudsto_post("/cloudsto/receive", sendptr-sendbuf, sendbuf, &recvbuf))
        return 1;
    GETCHAR(status)
    printf("Response: status = %d, ", status);
    if (status != 0) {
        printf("errmsg %s\n", recvbuf);
        return 1;
    }
    GETLONG(max_sn)
    GETLONG(entries)
    printf("max serial no = %d, received entries = %d\n", max_sn, entries);
    while (entries-- > 0) {
        GETCHAR(source)
        switch (source) {
            case 1: {
                GETLONG(start_time)
                GETLONG(end_time)
                GETLONG(step_count)
                printf("pedometer: start_time(%d), end_time(%d), step_count(%d)\n", start_time, end_time, step_count);
                break;
            }
            case 2:
                printf("temperature: ");
                // fallthrough
            case 3: {
                printf("humidity: ");
                GETLONG(start_time)
                GETLONG(interval)
                GETLONG(data_count)
                printf("start_time(%d), interval(%d), data_count(%d)\n", start_time, interval, data_count);
                printf("values: ");
                while (data_count-- > 0) {
                    GETLONG(data)
                    printf("%d\t", data);
                }
                printf("\n");
                break;
            }
            case 4: {
                GETLONG(time)
                GETLONG(data)
                printf("ultraviolet: time(%d), data(%d)\n", time, data);
                break;
            }
            default:
                printf("Invalid entry type\n");
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("An interactive shell for cloudsto testing\n");
        printf("Usage: ./cloudsto action\n");
        printf("actions: test register save receive\n");
        return 1;
    }
    if (strcmp(argv[1], "test") == 0) {
        return do_test();
    } else if (strcmp(argv[1], "register") == 0) {
        return do_register();
    } else if (strcmp(argv[1], "save") == 0) {
        return do_save();
    } else if (strcmp(argv[1], "receive") == 0) {
        return do_receive();
    } else {
        printf("Invalid action\n");
        return 1;
    }
}

