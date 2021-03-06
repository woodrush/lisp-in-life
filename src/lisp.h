#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define isEOF(x) (x == EOF)
#define isNotEOF(x) (x != EOF)

char define_str[] = "define";
char if_str[] = "if";
char quote_str[] = "quote";
char car_str[] = "car";
char cdr_str[] = "cdr";
char cons_str[] = "cons";
char atom_str[] = "atom";
char print_str[] = "print";
char progn_str[] = "progn";
char while_str[] = "while";
char lambda_str[] = "lambda";
char macro_str[] = "macro";
char lambdaast_str[] = "lambda*";
char eval_str[] = "eval";
char eq_str[] = "eq";
char plus_str[] = "+";
char minus_str[] = "-";
char ast_str[] = "*";
char slash_str[] = "/";
char mod_str[] = "mod";
char lt_str[] = "<";
char gt_str[] = ">";
char t_str[] = "t";
char list_str[] = "list";
char macroast_str[] = "macro*";

#define malloc_k(_malloc_bytes, _malloc_result) { _malloc_result = malloc(_malloc_bytes); }
#define malloc_k_pos(_malloc_bytes, _malloc_result, __edata) { _malloc_result = malloc(_malloc_bytes); }
