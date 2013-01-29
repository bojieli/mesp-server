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
    struct problem* next;
} problem;

int do_register(char* username, char* password, char** token);
int do_login(char* username, char* password, char** token);
int do_save(char* token, unsigned start_time, anslist* data, unsigned* sn);
int do_modify(char* token, unsigned sn, unsigned start_time, anslist* data);
int do_fetch(char* token, unsigned begin_sn, unsigned fetch_num, problem* data);

#define new_anslist(var) anslist var = {.next = NULL}
#define new_problem(var) problem var = {.next = NULL}

#define append_list(list,item) do { \
    (item)->next = (list)->next; \
    (list)->next = (list)->next->next; \
} while(0)
void add_problem(problem* prob, anslist* ansl);
void add_answer(anslist* ansl, ansitem ans);
int count_answer(anslist* l);
int count_problem(problem* p);

#define foreach_anslist(data,ans) for(ansitem ans; data->next != NULL && (memcpy(&ans, &data->next->a, sizeof(ansitem)) || 1); data = data->next)
#define foreach_problem(data,ans) problem *__iter = &data; for(anslist* ans; __iter->next != NULL && ((ans = __iter->next->l) || 1); __iter = __iter->next)

#endif
