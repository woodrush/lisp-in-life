#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define num_ops 22
char* define_str = "define";
char* if_str = "if";
char* quote_str = "quote";
char* car_str = "car";
char* cdr_str = "cdr";
char* cons_str = "cons";
char* atom_str = "atom";
char* print_str = "print";
char* progn_str = "progn";
char* while_str = "while";
char* lambda_str = "lambda";
char* macro_str = "macro";
char* eval_str = "eval";
char* eq_str = "eq";
char* plus_str = "+";
char* minus_str = "-";
char* ast_str = "*";
char* slash_str = "/";
char* mod_str = "mod";
char* lt_str = "<";
char* gt_str = ">";
char* t_str = "t";

char* opstr_list[num_ops];


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

// extern char* _str;
// int eqstr(char* s2) {
//     return !strcmp(_str, s2);
// }
