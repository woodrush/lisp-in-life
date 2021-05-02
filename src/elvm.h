// void placeholder (){
//     const char* __regarea = "\0\0\0\0\0\0\0\0\0\0";
//     const char* opstr = "eval\0atom\0quote\0macro\0lambda*\0define\0while\0progn\0lambda\0>\0<\0+\0-\0*\0/\0t\0mod\0print\0cons\0cdr\0car\0list\0eq\0if";
// }

#define QFTASM_RAMSTDIN_BUF_STARTPOSITION 2688

#define NULL 0

//====================================================================================
#define num_ops 24

extern int lambda_str;
// extern char* print_str;
// extern char* define_str;
// extern char* quote_str;
// extern char* list_str;
// extern char* if_str;
// extern char* car_str;
// extern char* while_str;
// extern char* progn_str;
// extern char* macro_str;
// extern char* lambdaast_str;
// extern char* eq_str;
// extern char* cons_str;
// extern char* plus_str;
// extern int t_str;
// extern char* mod_str;
// extern char* eval_str;
// extern char* cdr_str;
// extern char* minus_str;
// extern char* ast_str;
// extern char* lt_str;
// extern char* gt_str;
// extern char* slash_str;
// extern char* atom_str;


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

extern int evalhash;


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
