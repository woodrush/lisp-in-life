// void placeholder (){
//     const char* __regarea = "\0\0\0\0\0\0\0\0\0\0";
//     const char* opstr = "eval\0atom\0quote\0macro\0lambda*\0define\0while\0progn\0lambda\0>\0<\0+\0-\0*\0/\0t\0mod\0print\0cons\0cdr\0car\0list\0eq\0if";
// }

#define QFTASM_RAMSTDIN_BUF_STARTPOSITION 2688

#define NULL 0

//====================================================================================
#define num_ops 24

extern char lambda_str_head;
extern char print_str_head;
extern char define_str_head;
extern char quote_str_head;
extern char list_str_head;
extern char if_str_head;
extern char car_str_head;
extern char while_str_head;
extern char progn_str_head;
extern char macro_str_head;
extern char lambdaast_str_head;
extern char eq_str_head;
extern char cons_str_head;
extern char plus_str_head;
extern char t_str_head;
extern char mod_str_head;
extern char eval_str_head;
extern char cdr_str_head;
extern char minus_str_head;
extern char ast_str_head;
extern char lt_str_head;
extern char gt_str_head;
extern char slash_str_head;

extern int evalhash;

#define lambda_str (&lambda_str_head)
#define print_str (&print_str_head)
#define define_str (&define_str_head)
#define quote_str (&quote_str_head)
#define list_str (&list_str_head)
#define if_str (&if_str_head)
#define car_str (&car_str_head)
#define while_str (&while_str_head)
#define progn_str (&progn_str_head)
#define macro_str (&macro_str_head)
#define lambdaast_str (&lambdaast_str_head)
#define eq_str (&eq_str_head)
#define cons_str (&cons_str_head)
#define plus_str (&plus_str_head)
#define t_str (&t_str_head)
#define mod_str (&mod_str_head)
#define eval_str (&eval_str_head)
#define cdr_str (&cdr_str_head)
#define minus_str (&minus_str_head)
#define ast_str (&ast_str_head)
#define lt_str (&lt_str_head)
#define gt_str (&gt_str_head)
#define slash_str (&slash_str_head)
#define atom_str (&evalhash)


#define last_op &evalhash
#define opstring_head &lambda_str

//====================================================================================

// char* _str;
// int q, r;
// int i;
// int j;
// int k;

extern int QFTASM_PC;
extern int QFTASM_STDIN;
extern int QFTASM_STDOUT;
extern int QFTASM_A;
extern int QFTASM_B;
extern int QFTASM_C;
extern int QFTASM_D;
extern int QFTASM_BP;
extern int QFTASM_SP;
extern int QFTASM_TEMP;
extern int QFTASM_TEMP_2;



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
