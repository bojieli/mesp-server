#include <stdio.h>
#include <stdlib.h>
#include "mesp.h"

int demo_register() {
    int app_type, source;
    char username[MAXLEN], app_token[MAXLEN];
    prompt("app_type: 1 for Windows, 2 for Android, 3 for iOS");
    scanf("%d", &app_type);
    PUTCHAR(app_type);
    prompt("source: 1 for local, 2 for renren, 3 for sina weibo, 4 for tencent weibo");
    scanf("%d", &source);
    PUTCHAR(source);
    prompt("username");
    scanf("%s", username);
    PUTS(username);
    prompt("app token");
    scanf("%s", app_token);
    PUTS(app_token);
    CLOUD_POST("register");
    int status = GETCHAR();
    printf("Response: status = %d, ", status);
    if (status != 0)
        printf("errmsg = %s\n", recvbuf);
    else
        printf("token = %s\n", recvbuf+1);
    return 0;
}

int demo_save() {
    char token[MAXLEN];
    int entry_num;
    int source;
    prompt("token (given by register)");
    scanf("%s", token);
    PUTS(token);
    prompt("num of entries");
    scanf("%d", &entry_num);
    PUTLONG(entry_num);
    while (entry_num-- > 0) {
        prompt("source: 1 for pedometer, 2 for temperature, 3 for humidity, 4 for ultraviolet");
        scanf("%d", &source);
        PUTCHAR(source);
        switch (source) {
            int start_time, end_time, step_count, interval, data_count, time, data;
            case 1:
                prompt("start_time(long), end_time(long), step_count(long)");
                scanf("%d%d%d", &start_time, &end_time, &step_count);
                PUTLONG(start_time);
                PUTLONG(end_time);
                PUTLONG(step_count);
                break;
            case 2:
            case 3:
                prompt("start_time(long), interval(long), data_count(long)");
                scanf("%d%d%d", &start_time, &interval, &data_count);
                PUTLONG(start_time);
                PUTLONG(interval);
                PUTLONG(data_count);
                prompt("input %d data values", data_count);
                while (data_count-- > 0) {
                    scanf("%d", &data);
                    PUTLONG(data);
                }
                break;
            case 4:
                prompt("time(long), data(long)");
                scanf("%d%d", &time, &data);
                PUTLONG(time);
                PUTLONG(data);
                break;
            default:
                printf("Invalid entry type\n");
        }
    }
    CLOUD_POST("save");
    int status = GETCHAR();
    printf("Response: status = %d, ", status);
    if (status != 0)
        printf("errmsg = %s\n", recvbuf);
    else {
        printf("end serial no = %d\n", GETLONG());
    }
    return 0;
}

int demo_receive() {
    char token[MAXLEN];
    int sn_begin, sn_num;
    prompt("token (given by register)");
    scanf("%s", token);
    PUTS(token);
    prompt("beginning serial number");
    scanf("%d", &sn_begin);
    PUTLONG(sn_begin);
    prompt("number of serials (0 for unlimited)");
    scanf("%d", &sn_num);
    PUTLONG(sn_num);
    CLOUD_POST("receive");
    int status = GETCHAR();
    printf("Response: status = %d, ", status);
    if (status != 0) {
        printf("errmsg = %s\n", recvbuf);
        return 1;
    }
    int max_sn = GETLONG();
    int entries = GETLONG();
    printf("max serial no = %d, received entries = %d\n", max_sn, entries);
    while (entries-- > 0) {
        int source = GETCHAR();
        switch (source) {
                int data_count;
            case 1:
                printf("pedometer: start_time(%d), ", GETLONG());
                printf("end_time(%d), ", GETLONG());
                printf("step_count(%d)\n", GETLONG());
                break;
            case 2:
                printf("temperature: ");
                printf("start_time(%d), ", GETLONG());
                printf("interval(%d), ", GETLONG());
                data_count = GETLONG();
                printf("data_count(%d)\n", data_count);
                printf("values: ");
                while (data_count-- > 0)
                    printf("%d\t", GETLONG());
                printf("\n");
                break;
            case 3:
                printf("humidity: ");
                printf("start_time(%d), ", GETLONG());
                printf("interval(%d), ", GETLONG());
                data_count = GETLONG();
                printf("data_count(%d)\n", data_count);
                printf("values: ");
                while (data_count-- > 0)
                    printf("%d\t", GETLONG());
                printf("\n");
                break;
            case 4:
                printf("ultraviolet: time(%d), ", GETLONG());
                printf("data(%d)\n", GETLONG());
                break;
            default:
                printf("Invalid entry type\n");
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("An interactive shell for mesp testing\n");
        printf("Usage: ./mesp action\n");
        printf("actions: test register save receive\n");
        return 1;
    }
    PUTINIT();
    if (strcmp(argv[1], "test") == 0) {
        return do_test();
    } else if (strcmp(argv[1], "register") == 0) {
        return demo_register();
    } else if (strcmp(argv[1], "save") == 0) {
        return demo_save();
    } else if (strcmp(argv[1], "receive") == 0) {
        return demo_receive();
    } else {
        printf("Invalid action\n");
        return 1;
    }
}

