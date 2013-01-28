#ifndef __DATIQI_H__
#define __DATIQI_H__

#include "cloudsto.h"
#define CLOUD_BASE "/datiqi/"

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
int do_save(char* token, unsigned start_time, anslist data, unsigned* sn);
int do_modify(char* token, unsigned sn, unsigned start_time, anslist data);
int do_fetch(char* token, unsigned begin_sn, unsigned fetch_num, problem* data);

#define append_list(list,field,item)
#define add_problem(problem,answer) append_list(problem,anslist,answer)
#define add_answer(anslist,answer) append_list(anslist,ansitem,answer)

#endif
