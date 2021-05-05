#define QFTASM_RAMSTDIN_BUF_STARTPOSITION 500
#define QFTASM_STACK_SIZE 200

#define QFTASM_STDOUT 2
#define NULL 0
// #define isEOF(x) (!x)
// #define isNotEOF(x) (x)

#define EOF -1
#define isEOF(x) (x == EOF)
#define isNotEOF(x) (x != EOF)

//====================================================================================
#define num_ops 24

#define lambda_str 11
#define print_str 18
#define define_str 24
#define quote_str 31
#define list_str 37
#define if_str 42
#define car_str 45
#define while_str 49
#define progn_str 55
#define macro_str 61
#define lambdaast_str 67
#define eq_str 75
#define cons_str 78
#define plus_str 83
#define t_str 85
#define mod_str 87
#define eval_str 91
#define cdr_str 96
#define minus_str 100
#define ast_str 102
#define lt_str 104
#define gt_str 106
#define slash_str 108
#define atom_str 110

#define last_op atom_str

//====================================================================================

int getchar(void);
int putchar(int c);
void exit(int s);

extern int* _edata;

#define malloc_k(_malloc_bytes, _malloc_result) { \
    _malloc_result = (int)_edata;                 \
    _edata += _malloc_bytes;                      \
}

#define malloc_k_pos(_malloc_bytes, _malloc_result, __edata) { \
    _malloc_result = (int)__edata;                 \
    __edata += _malloc_bytes;                      \
}

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
