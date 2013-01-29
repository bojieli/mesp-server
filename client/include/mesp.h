#ifndef __MESP_H__
#define __MESP_H__

#include "cloudsto.h"

typedef enum { Windows, Android, iOS } app_type;
typedef enum { localAdmin, renren, sinaWeibo, tencentWeibo } account_source;
typedef enum { pedometer, temperature, humidity, ultraviolet } data_source;

typedef struct {
    unsigned start_time;
    unsigned end_time;
    unsigned step_count;
} pedometer_entry;

typedef struct {
    unsigned start_time;
    unsigned interval;
    unsigned data_count;
    unsigned* data; /* array of unsigned int */
} temperature_entry;

typedef temperature_entry humidity_entry;

typedef struct {
    unsigned time;
    unsigned data;
} ultraviolet_entry;

typedef struct {
    data_source src;
    union {
        pedometer_entry p;
        temperature_entry t;
        humidity_entry h;
        ultraviolet_entry u;
    } v; /* value */
} entry;

typedef struct entries {
    entry e;
    struct entries* next;
} entries;

int do_register(app_type app, account_source src, char* username, char* app_token, char** token);
int do_save(char* token, entries* data, unsigned* sn);
int do_receive(char* token, unsigned begin_sn, unsigned fetch_num, entries* data);

#define append_list(list,item) do { \
    item->next = list->next; \
    list->next = list->next->next; \
} while(0)
#define add_entry(entry,list) do { \
    entries __newentry; \
    __newentry.e = entry; \
    append_list(list,__newentry); \
} while(0)

int count_entry(entries l);

#define foreach_entry(data,ans) for(entry ans; data->next != NULL && (ans = data->next.a || 1); data = data->next)

#endif
