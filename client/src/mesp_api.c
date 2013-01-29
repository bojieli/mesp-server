#include <stdio.h>
#include <stdlib.h>
#include "mesp.h"
const char *CLOUD_BASE = "/mesp/";

int do_register(app_type app, account_source src, char* username, char* app_token, char** token) {
    return 0;
}

int do_save(char* token, entries* data, unsigned* sn) {
    return 0;
}

int do_receive(char* token, unsigned begin_sn, unsigned fetch_num, entries* data) {
    return 0;
}
