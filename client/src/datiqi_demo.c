#include <stdio.h>
#include <stdlib.h>
#include "datiqi.h"

int demo_register() {
    char username[MAXLEN], password[MAXLEN], password_repeat[MAXLEN];
    prompt("username");
    scanf("%s", username);
    prompt("password");
    scanf("%s", password);
    prompt("repeat password");
    scanf("%s", password_repeat);
    if (strcmp(password, password_repeat) != 0) {
        printf("Passwords mismatch!\n");
        return 1;
    }
    char *token;
    int status = do_register(username, password, &token);
    printf("Response: status = %d, ", status);
    if (status != 0)
        printf("errmsg = %s\n", errmsg);
    else
        printf("token = %s\n", token);
    return 0;
}

int demo_login() {
    char username[MAXLEN], password[MAXLEN];
    prompt("username");
    scanf("%s", username);
    prompt("password");
    scanf("%s", password);
    char *token;
    int status = do_login(username, password, &token);
    printf("Response: status = %d, ", status);
    if (status != 0)
        printf("errmsg = %s\n", errmsg);
    else
        printf("token = %s\n", token);
    return 0;
}

int demo_save() {
    char token[MAXLEN];
    int entry_num;
    new_problem(p);
    prompt("token");
    scanf("%s", token);
    prompt("start time");
    scanf("%d", &p.start_time);
    prompt("num of entries");
    scanf("%d", &entry_num);
    for (int i=1; i<=entry_num; i++) {
        ansitem ans;
        prompt("entry #%d: student number, answer string, time used (microseconds)", i);
        scanf("%s %s %d", ans.student_no, ans.ans, &ans.time);
        add_answer(p.l, ans);
    }
    int status = do_save(token, &p);
    if (status != 0)
        printf("errmsg = %s\n", errmsg);
    else
        printf("serial no = %d\n", p.sn);
    return 0;
}

int demo_modify() {
    char token[MAXLEN];
    int entry_num;
    new_problem(p);
    prompt("token");
    scanf("%s", token);
    prompt("serial number to modify");
    scanf("%d", &p.sn);
    prompt("start time");
    scanf("%d", &p.start_time);
    prompt("num of entries");
    scanf("%d", &entry_num);
    for (int i=1; i<=entry_num; i++) {
        ansitem ans;
        prompt("entry #%d: student no, answer string, time used (microseconds)", i);
        scanf("%s %s %d", ans.student_no, ans.ans, &ans.time);
        add_answer(p.l, ans);
    }
    int status = do_modify(token, &p);
    if (status != 0)
        printf("errmsg = %s\n", errmsg);
    else
        printf("success\n");
    return 0;
}

int demo_fetch() {
    char token[MAXLEN];
    int sn_begin, fetch_num;
    prompt("token");
    scanf("%s", token);
    prompt("beginning serial number");
    scanf("%d", &sn_begin);
    prompt("number of serials to fetch (0 for unlimited)");
    scanf("%d", &fetch_num);
    new_problem(data);
    int status = do_fetch(token, sn_begin, fetch_num, &data);
    if (status != 0) {
        printf("errmsg = %s\n", errmsg);
        return 0;
    }
    printf("received %d problems.\n", count_problem(&data));
    foreach_problem(data, oneprob) {
        printf("problem #%d start at %d:\n", oneprob->sn, oneprob->start_time);
        foreach_anslist(oneprob->l, ans)
            printf("  %15s %10s %10d\n", ans.student_no, ans.ans, ans.time);
        printf("\n");
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("An interactive shell for datiqi testing\n");
        printf("Usage: ./datiqi action\n");
        printf("Available actions: test register login save modify fetch\n");
        return 1;
    }
#define DISPATCH(action,func) \
    if (strcmp(argv[1], #action) == 0) \
        return func();

    DISPATCH(test, do_test)
    DISPATCH(register, demo_register)
    DISPATCH(login, demo_login)
    DISPATCH(save, demo_save)
    DISPATCH(modify, demo_modify)
    DISPATCH(fetch, demo_fetch)
#undef DISPATCH
    printf("Invalid action\n");
    return 1;
}

