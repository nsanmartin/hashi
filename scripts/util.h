#ifndef __HASHI_SCRIPTS_UTIL__
#define __HASHI_SCRIPTS_UTIL__
char* skip_space(char* l) {
    while (*l && isspace(*l)) { ++l; }
    return l;
}

char* next_space(char* l) {
    while (*l && !isspace(*l)) { ++l; }
    return l;
}

#endif
