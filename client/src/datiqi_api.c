#include "datiqi.h"
const char *CLOUD_BASE = "/datiqi/";
const unsigned char CLIENT_VERSION = 1;

#define POST_COMMON(path) \
    CLOUD_POST(path); \
    int status = GETCHAR(); \
    if (status != 0) { \
        errmsg = malloc(strlen((char*)recvbuf)+1); \
        strcpy(errmsg,(char*)recvbuf); \
        return status; \
    }

/** register account
 *  @param  username
 *  @param  password
 *  @return token
 *  @return (status)
 */
int do_register(char* username, char* password, char** token) {
    PUTINIT();
    PUTS(username);
    PUTS(password);
    POST_COMMON("register")
    *token = GETS();
    return 0;
}

/** login
 *  @param  username
 *  @param  password
 *  @return token
 *  @return (status)
 */
int do_login(char* username, char* password, char** token) {
    PUTINIT();
    PUTS(username);
    PUTS(password);
    POST_COMMON("login")
    *token = GETS();
    return 0;
}

#define count_list_proto(funcname,listtype) \
unsigned count_##funcname(listtype* l) { \
    unsigned counter = 0; \
    while (l->next) { \
        l = l->next; \
        counter++; \
    } \
    return counter; \
}

count_list_proto(answer, anslist)
count_list_proto(problem, problem)

/** save a problem
 *  @param  token
 *  @param  data->start_time
 *  @param  data->l (answer list)
 *  @return data->sn
 *  @return (status)
 */
int do_save(char* token, problem* data) {
    PUTINIT();
    PUTS(token);
    PUTLONG(data->start_time);
    PUTLONG(count_answer(data->l));
    foreach_anslist(data->l, ans) {
        PUTS(ans.student_no);
        PUTS(ans.ans);
        PUTLONG(ans.time);
    }
    POST_COMMON("save")
    data->sn = GETLONG();
    return 0;
}

/** modify a problem
 *  @param  token
 *  @param  data->sn
 *  @param  data->start_time
 *  @param  data->l (answer list)
 *  @return (status)
 */
int do_modify(char* token, problem* data) {
    PUTINIT();
    PUTS(token);
    PUTLONG(data->sn);
    PUTLONG(data->start_time);
    PUTLONG(count_answer(data->l));
    foreach_anslist(data->l, ans) {
        PUTS(ans.student_no);
        PUTS(ans.ans);
        PUTLONG(ans.time);
    }
    POST_COMMON("modify");
    return 0;
}

/** fetch problems
 *  @param  token
 *  @param  begin_sn (beginning serial number)
 *  @param  fetch_num (0 for unlimited)
 *  @return data (a list of problems)
 *  @return (status)
 */
int do_fetch(char* token, unsigned begin_sn, unsigned fetch_num, problem* data) {
    PUTINIT();
    PUTS(token);
    PUTLONG(begin_sn);
    PUTLONG(fetch_num);
    POST_COMMON("fetch");
    unsigned problem_count = GETLONG();
    while (problem_count-- > 0) {
        new_problem(p);
        p.sn = GETLONG();
        p.start_time = GETLONG();
        unsigned ans_count = GETLONG();
        while (ans_count-- > 0) {
            ansitem ans;
            ans.student_no = GETS();
            ans.ans = GETS();
            ans.time = GETLONG();
            add_answer(p.l,ans);
        }
        add_problem(data,&p);
    }
    return 0;
}

void add_problem(problem* list, problem* newl) {
    problem* __newprob = malloc(sizeof(problem));
    __newprob = newl;
    append_list(list, __newprob);
}

void add_answer(anslist* ansl, ansitem ans) {
    anslist* __ans = malloc(sizeof(anslist));
    __ans->a = ans;
    append_list(ansl, __ans);
}

