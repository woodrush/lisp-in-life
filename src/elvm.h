void placeholder (){
    const char* __header = "\0\0\0\0\0\0\0\0\0\0";
}

#define QFTASM_MEM_OFFSET 0
// #define QFTASM_MEM_OFFSET 1024
// #define QFTASM_MEM_OFFSET 170
#define QFTASM_NATIVE_ADDR(x) (x - QFTASM_MEM_OFFSET)

const char* STDIN_BUF_POINTER_REG = QFTASM_NATIVE_ADDR(1);
#define curchar() (*((char*) QFTASM_NATIVE_ADDR(*STDIN_BUF_POINTER_REG)))

#define NULL 0
#define EOF -1

int getchar(void);
int putchar(int c);
void exit(int s);

#define popchar() getchar()

extern int* _edata;


void printStr(char* p) {
    for (; *p; p++){
        putchar(*p);
    }
}

void* malloc(int n) {
    int* r = _edata;
    _edata += n;
    if (r > _edata) {
        printStr("Memory overflow\n");
        exit(1);
    }
    return r;
}

int eqstr(char* s1, char* s2) {
    for(; *s1 || *s2; s1++, s2++) {
        if (*s1 != *s2) {
            return 0;
        }
    }
    return 1;
}

void div(int n, int m, int* q, int* r) {
    int sign_n = n > 0;
    int sign = 1;
    if (n < 0) {
        sign = 1 - sign;
        n = -n;
    }
    if (m < 0) {
        sign = 1 - sign;
        m = -m;
    }
    *q = 0;
    while(n >= m){
        n -= m;
        (*q)++;
    }
    *q = sign ? *q : -(*q);
    *r = sign_n ? n : -n;
}

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
    int q, r;
    div(a, b, &q, &r);
    return q;
}

int __builtin_mod(int a, int b) {
    int q, r;
    div(a, b, &q, &r);
    return r;
}

void printInt(int v) {
    int sign = (v < 0) ? 0 : 1;
    if (!sign) {
        putchar('-');
        v = -v;
    }
    char buf[6];
    char* p = buf + 5;
    *p = '\0';
    do {
        int q, r;
        div(v, 10, &q, &r);
        p--;
        *p = (r + '0');
        v = q;
    } while (v);
    printStr(p);
}
