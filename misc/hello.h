// `putchar` and `getchar` is provided by default by the compiler
extern int putchar(int c);

// `%`, `*`, and `\` must be manually provided as `__builtin_mod`, `__builtin_mul`, `__builtin_div`
int __builtin_mod (int n, int m) {
    int r = n;
    while(r >= m){
        r -= m;
    }
    return r;
}

// Utility functions in `stdio.h`, such as `printf`, must be provided manually
int printf (char* s) {
    for (char* i = s; *i; i++) {
        putchar(*i);
    }
    return 0;
}