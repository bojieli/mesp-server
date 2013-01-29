#include "datiqi.h"
const char *CLOUD_BASE = "/datiqi/";

#define POST_COMMON(path) \
    if (CLOUD_POST(path)) \
        return -1; \
    int status = GETCHAR(); \
    if (status != 0) { \
        errmsg = malloc(strlen(recvbuf)+1); \
        strcpy(errmsg,recvbuf); \
        return status; \
    }

int do_register(char* username, char* password, char** token) {
    PUTINIT();
    PUTS(username);
    PUTS(password);
    POST_COMMON("register")
    *token = GETS();
    return 0;
}

int do_login(char* username, char* password, char** token) {
    PUTINIT();
    PUTS(username);
    PUTS(password);
    POST_COMMON("login")
    *token = GETS();
    return 0;
}

#define count_list_proto(funcname,listtype) \
int count_##funcname(listtype* l) { \
    int counter = 0; \
    while (l->next) { \
        l = l->next; \
        counter++; \
    } \
    return counter; \
}

count_list_proto(answer, anslist)
count_list_proto(problem, problem)

int do_save(char* token, unsigned start_time, anslist* data, unsigned* sn) {
    PUTINIT();
    PUTS(token);
    PUTLONG(start_time);
    PUTLONG(count_answer(data));
    foreach_anslist(data, ans) {
        PUTS(ans.student_no);
        PUTS(ans.ans);
        PUTLONG(ans.time);
    }
    POST_COMMON("save")
    *sn = GETLONG();
    return 0;
}

int do_modify(char* token, unsigned sn, unsigned start_time, anslist* data) {
    PUTINIT();
    PUTS(token);
    PUTLONG(sn);
    PUTLONG(start_time);
    PUTLONG(count_answer(data));
    foreach_anslist(data, ans) {
        PUTS(ans.student_no);
        PUTS(ans.ans);
        PUTLONG(ans.time);
    }
    POST_COMMON("modify");
    return 0;
}

int do_fetch(char* token, unsigned begin_sn, unsigned fetch_num, problem* data) {
    PUTINIT();
    PUTS(token);
    PUTLONG(begin_sn);
    PUTLONG(fetch_num);
    POST_COMMON("fetch");
    int problem_count = GETLONG();
    while (problem_count-- > 0) {
        int ans_count = GETLONG();
        new_anslist(l);
        while (ans_count-- > 0) {
            ansitem ans;
            ans.student_no = GETS();
            ans.ans = GETS();
            ans.time = GETLONG();
            add_answer(&l,ans);
        }
        add_problem(data,&l);
    }
    return 0;
}

void add_problem(problem* prob, anslist* ansl) {
    problem* __newprob = malloc(sizeof(problem));
    __newprob->l = ansl;
    append_list(prob, __newprob);
}

void add_answer(anslist* ansl, ansitem ans) {
    anslist* __ans = malloc(sizeof(anslist));
    __ans->a = ans;
    append_list(ansl, __ans);
}

