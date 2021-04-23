#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char charbuf = -1;

char popchar() {
    if (charbuf < 0) {
        return getchar();
    }
    char ret = charbuf;
    charbuf = -1;
    return ret;
}

char curchar() {
    if (charbuf < 0) {
        charbuf = getchar();
    }
    return charbuf;
}

void printStr(char* p) {
    printf("%s", p);
}

void printInt(int n) {
    printf("%d", n);   
}

// int eqstr(char* s1, char* s2) {
//     return !strcmp(s1, s2);
// }

extern char* _str;
int eqstr(char* s2) {
    return !strcmp(_str, s2);
}
