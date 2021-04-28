// void placeholder (){
//     const char* __regarea = "\0\0\0\0\0\0\0\0\0\0";
//     const char* opstr = "eval\0atom\0quote\0macro\0lambda*\0define\0while\0progn\0lambda\0>\0<\0+\0-\0*\0/\0t\0mod\0print\0cons\0cdr\0car\0list\0eq\0if";
// }

#define QFTASM_RAMSTDIN_BUF_STARTPOSITION 688

#define NULL 0
#define EOF -1

//====================================================================================
#define num_ops 24

#define eval_str 11
#define atom_str 16
#define quote_str 21
#define macro_str 27
#define lambdaast_str 33
#define define_str 41
#define while_str 48
#define progn_str 54
#define lambda_str 60
#define gt_str 67
#define lt_str 69
#define plus_str 71
#define minus_str 73
#define ast_str 75
#define slash_str 77
#define t_str 79
#define mod_str 81
#define print_str 85
#define cons_str 91
#define cdr_str 96
#define car_str 100
#define list_str 104
#define eq_str 109
#define if_str 112

#define last_op if_str

//====================================================================================

// char* _str;
// int q, r;
// int i;
// int j;
// int k;


int getchar(void);
int putchar(int c);
void exit(int s);

extern int* _edata;

// int _malloc_bytes;
// void* _malloc_result;
void malloc_k() {
    _malloc_result = (int)_edata;
    _edata += _malloc_bytes;
    // if ((int*)r > _edata) {
    //     _str = "Memory overflow\n";
    //     for (; *_str; _str++){
    //         putchar(*_str);
    //     }
    //     exit(1);
    // }
    // return (int*)r;
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
