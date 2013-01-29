#ifndef __DATIQI_H__
#define __DATIQI_H__

#include "cloudsto.h"

typedef struct {
    char* student_no;
    char* ans;
    unsigned time;
} ansitem;

typedef struct anslist {
    ansitem a;
    struct anslist* next;
} anslist;

typedef struct problem {
    anslist* l;
    unsigned sn;
    unsigned start_time;
    struct problem* next;
} problem;

int do_register(char* username, char* password, char** token);
int do_login(char* username, char* password, char** token);
int do_save(char* token, problem* data);
int do_modify(char* token, problem* data);
int do_fetch(char* token, unsigned begin_sn, unsigned fetch_num, problem* data);

#define new_anslist(var) anslist var = {.next = NULL}
#define new_problem(var) problem var = {.l = malloc(sizeof(anslist)), .next = NULL}

#define append_list(list,item) do { \
    (item)->next = (list)->next; \
    (list)->next = (list)->next->next; \
} while(0)
void add_problem(problem* list, problem* newl);
void add_answer(anslist* ansl, ansitem ans);
unsigned count_answer(anslist* l);
unsigned count_problem(problem* p);

#define foreach_anslist(data,ans) for(ansitem ans; data->next != NULL && (memcpy(&ans, &data->next->a, sizeof(ansitem)) || 1); data = data->next)
#define foreach_problem(data,ans) for(problem *ans = data.next; ans != NULL; ans = ans->next)

#endif
