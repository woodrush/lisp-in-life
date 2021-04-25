void placeholder (){
    const char* __regarea = "\0\0\0\0\0\0\0\0\0";
    const char* opstr = "eval\0closure\0atom\0quote\0macro\0define\0while\0progn\0lambda\0>\0<\0+\0-\0*\0/\0t\0mod\0print\0cons\0cdr\0car\0eq\0if";
}

#define QFTASM_RAMSTDIN_BUF_STARTPOSITION 1592
#define QFTASM_MEM_OFFSET 0
// #define QFTASM_MEM_OFFSET 1024
// #define QFTASM_MEM_OFFSET 170

#define STDIN_BUF_POINTER_REG 1
#define curchar() (*((char*) *((char*)STDIN_BUF_POINTER_REG)))

#define NULL 0
#define EOF -1

//====================================================================================
#define num_ops 23

#define eval_str 10
#define closure_str 15
#define atom_str 23
#define quote_str 28
#define macro_str 34
#define define_str 40
#define while_str 47
#define progn_str 53
#define lambda_str 59
#define gt_str 66
#define lt_str 68
#define plus_str 70
#define minus_str 72
#define ast_str 74
#define slash_str 76
#define t_str 78
#define mod_str 80
#define print_str 84
#define cons_str 90
#define cdr_str 95
#define car_str 99
#define eq_str 103
#define if_str 106

#define last_op gt_str

//====================================================================================

char* _str;
int q, r;
int i;
int j;
int k;





int getchar(void);
int putchar(int c);
void exit(int s);

#define popchar() getchar()

extern int* _edata;

int malloc_bytes;
void* malloc_k() {
    int* r = _edata;
    _edata += malloc_bytes;
    if (r > _edata) {
        _str = "Memory overflow\n";
        for (; *_str; _str++){
            putchar(*_str);
        }
        exit(1);
    }
    return r;
}

void _div(int n, int m);

int __builtin_mul(int x, int y) {
    int sign = 1;
    if (x < 0) {
        sign = 1 - sign;
        x = -x;
    }
    if (y < 0) {
        sign = 1 - sign;
        y = -y;
    }
    int ret = 0;
    while(x--) {
        ret += y;
    }
    return sign ? ret : -ret;
}

int __builtin_div(int a, int b) {
    // int q, r;
    _div(a, b);
    return q;
}

int __builtin_mod(int a, int b) {
    // int q, r;
    _div(a, b);
    return r;
}
