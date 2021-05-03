// void placeholder (){
//     const char* __regarea = "\0\0\0\0\0\0\0\0\0\0";
//     const char* opstr = "eval\0atom\0quote\0macro\0lambda*\0define\0while\0progn\0lambda\0>\0<\0+\0-\0*\0/\0t\0mod\0print\0cons\0cdr\0car\0list\0eq\0if";
// }

#define QFTASM_RAMSTDIN_BUF_STARTPOSITION 2688

#define NULL 0

//====================================================================================
#define num_ops 24

// extern char lambda_str_body;
// extern char print_str_body;
// extern char define_str_body;
// extern char quote_str_body;
// extern char list_str_body;
// extern char if_str_body;
// extern char car_str_body;
// extern char while_str_body;
// extern char progn_str_body;
// extern char macro_str_body;
// extern char lambdaast_str_body;
// extern char eq_str_body;
// extern char cons_str_body;
// extern char plus_str_body;
// extern char t_str_body;
// extern char mod_str_body;
// extern char eval_str_body;
// extern char cdr_str_body;
// extern char minus_str_body;
// extern char ast_str_body;
// extern char lt_str_body;
// extern char gt_str_body;
// extern char slash_str_body;

extern int evalhash;

char define_str_body[] = "define";
char if_str_body[] = "if";
char quote_str_body[] = "quote";
char car_str_body[] = "car";
char cdr_str_body[] = "cdr";
char cons_str_body[] = "cons";
char atom_str_body[] = "atom";
char print_str_body[] = "print";
char progn_str_body[] = "progn";
char while_str_body[] = "while";
char lambda_str_body[] = "lambda";
char macro_str_body[] = "macro";
char lambdaast_str_body[] = "lambda*";
char eval_str_body[] = "eval";
char eq_str_body[] = "eq";
char plus_str_body[] = "+";
char minus_str_body[] = "-";
char ast_str_body[] = "*";
char slash_str_body[] = "/";
char mod_str_body[] = "mod";
char lt_str_body[] = "<";
char gt_str_body[] = ">";
char t_str_body[] = "t";
char list_str_body[] = "list";

#define lambda_str (&lambda_str_body)
#define print_str (&print_str_body)
#define define_str (&define_str_body)
#define quote_str (&quote_str_body)
#define list_str (&list_str_body)
#define if_str (&if_str_body)
#define car_str (&car_str_body)
#define while_str (&while_str_body)
#define progn_str (&progn_str_body)
#define macro_str (&macro_str_body)
#define lambdaast_str (&lambdaast_str_body)
#define eq_str (&eq_str_body)
#define cons_str (&cons_str_body)
#define plus_str (&plus_str_body)
#define t_str (&t_str_body)
#define mod_str (&mod_str_body)
#define eval_str (&eval_str_body)
#define cdr_str (&cdr_str_body)
#define minus_str (&minus_str_body)
#define ast_str (&ast_str_body)
#define lt_str (&lt_str_body)
#define gt_str (&gt_str_body)
#define slash_str (&slash_str_body)
#define atom_str (&evalhash)

#define last_op list_str
// #define last_op &evalhash
#define opstring_head lambda_str

//====================================================================================

// char* _str;
// int q, r;
// int i;
// int j;
// int k;

// extern int QFTASM_PC;
// extern int QFTASM_STDIN;
// extern int QFTASM_STDOUT;
// extern int QFTASM_A;
// extern int QFTASM_B;
// extern int QFTASM_C;
// extern int QFTASM_D;
// extern int QFTASM_BP;
// extern int QFTASM_SP;
// extern int QFTASM_TEMP;
// extern int QFTASM_TEMP_2;



#define EOF -1
DEFLOCATION char c;
// #define getchar_c() { \
//     c = *((int*)(QFTASM_STDIN >> 1)); \
//     c = (QFTASM_STDIN &~ 0b1111111111111110) ? ((c &~ 0b11111111) >> 8) : (c &~ 0b1111111100000000); \
//     QFTASM_STDIN += 1; \
// }
#define getchar_c() (c = getchar())

// void getchar_() { \
//     c = *((int*)(QFTASM_STDIN >> 1)); \
//     c = (QFTASM_STDIN &~ 0b1111111111111110) ? ((c &~ 0b11111111) >> 8) : (c &~ 0b1111111100000000); \
//     QFTASM_STDIN += 1; \
// }


void getchar(void);
int putchar(int c);
void exit(int s);

extern int* _edata;

// int _malloc_bytes;
// void* _malloc_result;

// void malloc_k() {
//     _malloc_result = (int)_edata;
//     _edata += _malloc_bytes;
//     // if ((int*)r > _edata) {
//     //     _str = "Memory overflow\n";
//     //     for (; *_str; _str++){
//     //         putchar(*_str);
//     //     }
//     //     exit(1);
//     // }
//     // return (int*)r;
// }
#define malloc_k(_malloc_bytes, _malloc_result) { _malloc_result = (int)_edata; _edata += _malloc_bytes; }

void _div(int n, int m);

int __builtin_mul(int x, int y) {
    #define sign q
    #define ret r
    sign = 1;
    if (x < 0) {
        sign = 1 - sign;
        x = -x;
    }
    if (y < 0) {
        sign = 1 - sign;
        y = -y;
    }
    ret = 0;
    while(x--) {
        ret += y;
    }
    return sign ? ret : -ret;
    #undef sign
    #undef ret
}

int __builtin_div(int a, int b) {
    _div(a, b);
    return q;
}

int __builtin_mod(int a, int b) {
    _div(a, b);
    return r;
}
