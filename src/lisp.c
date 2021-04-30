char* _str;
int q, r;
int i;
int j;
int k;
int _malloc_bytes;
void* _malloc_result;


#ifndef GCC
#define ELVM
#endif

#ifdef ELVM
#include "elvm.h"
#else
#include "lisp.h"
#endif

// #define QFTASM_HEAP_MEM_MAX 2846

// #include <stdio.h>
#define debug(x) //printf(x)
#define debug2(x,y,z) //printf(x,y,z)

// ATOM=1, since .type and .next of Value is a union, and .next is usually set to NULL
typedef enum {
    ATOM=1, INT=2, LAMBDA=3, LIST=4
} Valuetype;

typedef struct Value {
    union {
        Valuetype type;
        struct Value* next;
    };
    union {
        char* str;
        struct Value* value;
        int n;
        struct Lambda* lambda;
    };
} Value;

typedef struct StringTable {
    // char* varname;
    Value* value;
    struct StringTable* lesser;
    struct StringTable* greater;
} StringTable;

typedef enum {
    ENV_PERSISTENT=1, ENV_TEMPORARY=2
} Envtype;

typedef struct Env {
    char* varname;
    struct Env* prev;
    struct Value* value;
    struct Env* next;
} Env;

typedef enum {
    L_LAMBDA, L_MACRO, L_CLOSURE
} Lambdatype;

typedef struct Lambda {
    struct Value* argnames;
    struct Value* body;
    struct Env* env;
    Lambdatype type;
} Lambda;


char charbuf = 0;
char c;

char buf[32];
char* s1;
char* s2;
char* s3;

StringTable* stringTableHeadList[16];// = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
StringTable* stringTableHead = NULL;
StringTable* _stringtable;

Lambda* _lambda;

Env* _env;
Env* _env2;
Env* _env3;
Env* _evalenv;

Value* nil;
Value* true_value;
Value* _value;
Value* _list;
Value* initlist;
Value* curlist;

int sthash;

#define sthash_mod16() { sthash = sthash &~ 0b1111111111110000; }

void _div(int n, int m) {
    #define sign_n i
    #define sign j
    sign_n = n > 0;
    sign = 1;
    if (n < 0) {
        sign = 1 - sign;
        n = -n;
    }
    if (m < 0) {
        sign = 1 - sign;
        m = -m;
    }
    q = 0;
    while(n >= m){
        n -= m;
        q++;
    }
    q = sign ? q : -q;
    r = sign_n ? n : -n;
    #undef sign_n
    #undef sign
}

//================================================================================
// Parser
//================================================================================


#define newAtomNode(__str) {         \
    malloc_k(sizeof(Value), _value); \
    debug("newAtomNode\n");          \
    _value->type = ATOM;             \
    _value->str = __str;             \
}

#define newLambdaValue(__target, __argnames, __body, __env, __type) {  \
    malloc_k(sizeof(Lambda), __target);                                \
    debug("lambda 1\n");                                               \
    _lambda->argnames = __argnames;                                    \
    _lambda->body = __body;                                            \
    _lambda->env = __env;                                              \
    _lambda->type = __type;                                            \
}

#define newLambdaNode() {            \
    malloc_k(sizeof(Value), _value); \
    debug("lambda 2\n");             \
    _value->type = LAMBDA;           \
    _value->lambda = _lambda;        \
}

Value* newList(Value* node, Value* next) {
#define ret _list
    // _malloc_bytes = sizeof(Value);
    malloc_k(sizeof(Value), ret);
    // ret = (Value*) _malloc_result;
    debug("newList\n");
    ret->value = node;
    ret->next = next;
    return ret;
#undef ret
}

void parseExpr();


#define newStringTable(__stringtable, __value) {    \
    newAtomNode(_str);                              \
    malloc_k(sizeof(StringTable), __stringtable);   \
    debug("newStringTable\n");                      \
    _stringtable->value = __value;                  \
    _stringtable->lesser = NULL;                    \
    _stringtable->greater = NULL;                   \
}

// TODO: optimize _str[0] to char c
int getOrSetAtomFromStringTable_newflag = 0;
StringTable* stringtable;
StringTable** branch;
char* __targetstring;

// Caution: always is __stringtable = stringTableHead when called
void getOrSetAtomFromStringTable () {
    stringtable = stringTableHeadList[sthash];
    if (!stringtable) {
        branch = (stringTableHeadList + sthash);
        goto getOrSetAtomFromStringTable_setstringtable;
    }
    // stringtable = __stringtable;
    // targetstring = __targetstring;

    // stringtable = stringTableHead;
getOrSetAtomFromStringTableHead:
    s1 = stringtable->value->str;
    s2 = __targetstring;
    debug2("%s v.s. %s (the input)\n", s1, s2);
    for (; *s1 || *s2; s1++, s2++) {

//         if (*s1 != *s2) {
//             // There is a string next to this string in the table
//             if ((_stringtable = _stringtable->next)) {
//                 goto parseatomloop;
//             }

//             // This was the last string in the table, so create a string
//             // _malloc_bytes = i+1;
//             malloc_k(i+1, _str);
//             // _str = (char*) _malloc_result;
//             debug("parseAtom\n");
//             s1 = _str;
//             s2 = buf;
//             for(; *s2; s1++, s2++) {
//                 *s1 = *s2;
//             }
//             appendStringTable();
//             return;
//         }

        // The strings were not equal
        if (*s1 != *s2) {
            // _stringtable = stringtable->lesser;
            // k = targetstring[0] < stringtable->value->str[0];
            branch = *s1 < *s2 ? &(stringtable->lesser) : &(stringtable->greater);
            // There are no more strings that could match in the table
            if ((stringtable = *branch)) {
                // There is a string table that we could proceed to search
                debug("Continuing search...\n");
                // getOrSetAtomFromStringTable(_stringtable, targetstring);
                goto getOrSetAtomFromStringTableHead;
                // return;
                // return _value;
            }
getOrSetAtomFromStringTable_setstringtable:
            if (getOrSetAtomFromStringTable_newflag) {
                debug("Creating new stringtable entry with a new string pointer...\n");

                // This was the last string in the table, so create a string
                // _malloc_bytes = i+1;
                malloc_k(i+1, _str);
                // s2 = _str;
                // _str = s1;
                // s1 = _value->str;
                s1 = _str;
                s2 = __targetstring;
                // s2 = _str;
                // _str = (char*) _malloc_result;
                debug("parseAtom\n");
                for(; *s2; s1++, s2++) {
                    *s1 = *s2;
                }
            }
            else {
                debug("Creating new stringtable entry with an existing string pointer...\n");
                _str = __targetstring;
            }
            newAtomNode(_str);
            newStringTable(_stringtable, _value);
            *branch = _stringtable;
            // if (k) {
            //     stringtable->lesser = _stringtable;
            // } else {
            //     stringtable->greater = _stringtable;
            // }
            return;
            // return _value;
        }
    }
    // The strings were equal
    debug("The strings have matched!\n");
    _value = stringtable->value;
    // return _value;
}

// #define appendStringTable() {                             \
//     malloc_k(sizeof(StringTable), _stringtable);          \
//     debug("appendStringTable\n");                         \
//     newAtomNode();                                        \
//     _stringtable->value = _value;                         \
//     _stringtable->next = stringTableHead;                 \
//     stringTableHead = _stringtable;                       \
// }


Value* parseListLoop() {
    parseExpr();
    Value* parsednode = _value;
    return parsednode ? newList(parsednode, parseListLoop()) : NULL;
}

int isNumeric(){
    return c == '-' || ('0' <= c && c <= '9');
}

#define str _str
#define sign j

void parseInt() {
    sign = 1;
    i = 0;
    if (str[0] == '-') {
        sign = 0;
        str++;
    }
    while (*str) {
        // i *= 10
        i += i;
        k = i;
        i += i;
        i += i + k + (*str - '0');
        str++;
    }
    i = sign ? i : -i;
}
#undef str
#undef sign

void newIntValue(){
#define ret _value
    // _malloc_bytes = sizeof(Value);
    malloc_k(sizeof(Value), ret);
    // ret = (Value*) _malloc_result;    
    debug("newIntValue\n");
    ret->type = INT;
    ret->n = i;
#undef ret
}

void parseExpr() {
    // Remove whitespace
space:;
    if (!c) {
        c = getchar();
    }
    while (c == ' ' || c == '\n') {
        c = getchar();
    }
    if (c == ';') {
        do {
            c = getchar();
// #ifdef ELVM
//         } while(c != '\n');
// #else
        } while(c != '\n' && c != EOF);
// #endif
        charbuf = 0;
        goto space;
    }

    // Parse as a list
    if (c == '(') {
        c = 0; // '('

        _value = parseListLoop();

        c = 0; // ')'

        if (!_value) {
            _value = nil;
        }
        return;
    }

    // Parse as an atom
// #ifdef ELVM
//     if (c == ')' || !c) {
// #else
    if (c == ')' || !c || c == EOF) {
// #endif
        _value = NULL;
        return;
    }

    i = 0;
// #ifdef ELVM
//     while (c != ' ' && c != '\n' && c != ')' && c != '(' && c != ';') {
// #else
    sthash = 0;
    while (c && c != ' ' && c != '\n' && c != ')' && c != '(' && c != ';' && c != EOF) {
// #endif
        buf[i] = c;
        sthash += c;
        i++;
        c = getchar();
    }
    buf[i] = '\0';

    // If the expression is an integer literal, evaluate it
    charbuf = buf[0];
    if (('0' <= charbuf && charbuf <= '9') || charbuf == '-' && ('0' <= buf[1] && buf[1] <= '9')) {
        _str = buf;
        parseInt();
        newIntValue();
        return;
    }


    sthash_mod16();

    // _str = buf;
    // getOrSetAtomFromStringTable(stringTableHead, buf);

    // __stringtable = stringTableHead;
    __targetstring = buf;
    getOrSetAtomFromStringTable();

//     _stringtable = stringTableHead;


// parseatomloop:;
//     s1 = buf;
//     s2 = _stringtable->value->str;
//     for(; *s1 || *s2; s1++, s2++) {
//         if (*s1 != *s2) {
//             // There is a string next to this string in the table
//             if ((_stringtable = _stringtable->next)) {
//                 goto parseatomloop;
//             }

//             // This was the last string in the table, so create a string
//             // _malloc_bytes = i+1;
//             malloc_k(i+1, _str);
//             // _str = (char*) _malloc_result;
//             debug("parseAtom\n");
//             s1 = _str;
//             s2 = buf;
//             for(; *s2; s1++, s2++) {
//                 *s1 = *s2;
//             }
//             appendStringTable();
//             return;
//         }
//     }
//     // The strings are equal
//     _value = _stringtable->value;    
}

//================================================================================
// Evaluator
//================================================================================


Env* newEnv() {
    // _malloc_bytes = sizeof(Env);
    malloc_k(sizeof(Env), _env2);
    // _env2 = (Env*) _malloc_result;
    debug("newEnv\n");
    _env2->varname = _str;
    _env2->value = _value;
    _env2->next = _env;
    _env2->prev = (Env*)1;
    return _env2;
}


// Persistent envs:
// - Can have .next and .prev
// - Is never a .prev of any env (to avoid overwriting)
// Temporary envs:
// - Can have .next and .prev
// - Can be a .prev of some env (can be overwritten)

#define prependTemporaryEnv(_env2) {                                              \
    /* Caution: Raw pointer comparisons, may not work outside of QFT*/            \
                                                                                  \
    /* The current head env is an env that already has a previous env*/           \
    if (!(_env->prev == (Env*)1)) {                                               \
        /* Overwrite the contents of the env and return it */                     \
        _env2 = _env->prev;                                                       \
        _env2->varname = _str;                                                    \
        _env2->value = _value;                                                    \
                                                                                  \
    /* The current head env doesn't have a .prev yet */                           \
    } else {                                                                      \
        /* _env2->next gets set to _env */                                        \
        _env2 = newEnv();                                                         \
        /* Persistent envs also can have a .prev . */                             \
        /* We just have to make sure it doesn't become a .prev of any env, */     \
        /* at the time of its initialization */                                   \
        _env->prev = _env2;                                                       \
    }                                                                             \
}

void printValue();



void eval(Value* node);
void evalAsInt() {
    if (_value->type == INT) {
        i = _value->n;
        return;
    }
    c = _value->str[0];
    if (isNumeric()) {
        _str = _value->str;
        parseInt();
    } else {
        eval(_value);
        i = _value->n;
    }
}

typedef struct {
    union {
        char c_eval_;
        Value* _list_eval_;
    };
    union {
        Value* arg1_;
        Value* _list_eval_2;
        Env* e2;
    };
    union {
        Value* arg2list_;
        int n_;
        Env* e;
    };
} EvalStack;

void* evalhash[51];


void eval(Value* node) {
#ifdef ELVM
    if (!*evalhash) {
        // evalhash[0] = 0;
        // evalhash[lambda_str-10] = &&eval_createlambda;
        // evalhash[print_str-10] = &&eval_print;
        // evalhash[define_str-10] = &&eval_define;
        // evalhash[quote_str-10] = &&eval_quote;
        // evalhash[list_str-10] = &&eval_list;
        // evalhash[if_str-10] = &&eval_if;
        // evalhash[car_str-10] = &&eval_car;
        // evalhash[while_str-10] = &&eval_while;
        // evalhash[progn_str-10] = &&eval_progn;
        // evalhash[macro_str-10] = &&eval_createlambda;
        // evalhash[lambdaast_str-10] = &&eval_createlambda;
        // evalhash[eq_str-10] = &&eval_eq;
        // evalhash[cons_str-10] = &&eval_cons;
        // evalhash[plus_str-10] = &&eval_arith;
        // evalhash[mod_str-10] = &&eval_arith;
        // evalhash[eval_str-10] = &&eval_eval;
        // evalhash[cdr_str-10] = &&eval_cdr;
        // evalhash[minus_str -10] = &&eval_arith;
        // evalhash[ast_str -10] = &&eval_arith;
        // evalhash[lt_str -10] = &&eval_cmp;
        // evalhash[gt_str -10] = &&eval_cmp;
        // evalhash[slash_str -10] = &&eval_arith;
        // evalhash[atom_str -10] = &&eval_atom;
        evalhash[(lambda_str-11)>>1] = &&eval_createlambda;
        evalhash[(print_str-11)>>1] = &&eval_print;
        evalhash[(define_str-11)>>1] = &&eval_define;
        evalhash[(quote_str-11)>>1] = &&eval_quote;
        evalhash[(list_str-11)>>1] = &&eval_list;
        evalhash[(if_str-11)>>1] = &&eval_if;
        evalhash[(car_str-11)>>1] = &&eval_car;
        evalhash[(while_str-11)>>1] = &&eval_while;
        evalhash[(progn_str-11)>>1] = &&eval_progn;
        evalhash[(macro_str-11)>>1] = &&eval_createlambda;
        evalhash[(lambdaast_str-11)>>1] = &&eval_createlambda;
        evalhash[(eq_str-11)>>1] = &&eval_eq;
        evalhash[(cons_str-11)>>1] = &&eval_cons;
        evalhash[(plus_str-11)>>1] = &&eval_arith;
        evalhash[(mod_str-11)>>1] = &&eval_arith;
        evalhash[(eval_str-11)>>1] = &&eval_eval;
        evalhash[(cdr_str-11)>>1] = &&eval_cdr;
        evalhash[(minus_str -11)>>1] = &&eval_arith;
        evalhash[(ast_str -11)>>1] = &&eval_arith;
        evalhash[(lt_str -11)>>1] = &&eval_cmp;
        evalhash[(gt_str -11)>>1] = &&eval_cmp;
        evalhash[(slash_str -11)>>1] = &&eval_arith;
        evalhash[(atom_str -11)>>1] = &&eval_atom;
    }
#endif
    // void* evalhash[51] = {
    //     &&eval_createlambda, 0, 0,
    //     &&eval_print, 0, 0,
    //     &&eval_define, 0, 0, 0,
    //     &&eval_quote, 0, 0,
    //     &&eval_list, 0,
    //     &&eval_if, 0,
    //     &&eval_car, 0,
    //     &&eval_while, 0, 0,
    //     &&eval_progn, 0, 0,
    //     &&eval_createlambda, 0, 0,
    //     &&eval_createlambda, 0, 0, 0,
    //     &&eval_eq,
    //     &&eval_cons, 0, 0,
    //     &&eval_arith,
    //     0,
    //     &&eval_arith, 0,
    //     &&eval_eval, 0,
    //     &&eval_cdr, 0,
    //     &&eval_arith,
    //     &&eval_arith,
    //     &&eval_cmp,
    //     &&eval_cmp,
    //     &&eval_arith,
    //     &&eval_atom,
    // };
    // void* evalhash[101] = {
    //     &&eval_createlambda, 0, 0, 0, 0, 0, 0,
    //     &&eval_print, 0, 0, 0, 0, 0,
    //     &&eval_define, 0, 0, 0, 0, 0, 0,
    //     &&eval_quote, 0, 0, 0, 0, 0,
    //     &&eval_list, 0, 0, 0, 0,
    //     &&eval_if, 0, 0,
    //     &&eval_car, 0, 0, 0,
    //     &&eval_while, 0, 0, 0, 0, 0,
    //     &&eval_progn, 0, 0, 0, 0, 0,
    //     &&eval_createlambda, 0, 0, 0, 0, 0,
    //     &&eval_createlambda, 0, 0, 0, 0, 0, 0, 0,
    //     &&eval_eq, 0, 0,
    //     &&eval_cons, 0, 0, 0, 0,
    //     &&eval_arith, 0,
    //     0, 0,
    //     &&eval_arith, 0, 0, 0,
    //     &&eval_eval, 0, 0, 0, 0,
    //     &&eval_cdr, 0, 0, 0,
    //     &&eval_arith, 0,
    //     &&eval_arith, 0,
    //     &&eval_cmp, 0,
    //     &&eval_cmp, 0,
    //     &&eval_arith, 0,
    //     &&eval_atom,
    // };

// #define node evalarg->node
    EvalStack evalstack;
    debug("entering eval...\n");

#define _list_eval (evalstack._list_eval_)
#define _list_eval_2 (evalstack._list_eval_2)
#define arg1 (evalstack.arg1_)
#define arg2list (evalstack.arg2list_)
#define c_eval (evalstack.c_eval_)
#define n_ (evalstack.n_)
#define evalstack_env (evalstack.e)
#define evalstack_env2 (evalstack.e2)
#define nodetype i

    nodetype = node->type;
    // Is an atom
    if (nodetype == ATOM) {
        _str = node->str;
        c = node->str[0];
        if (isNumeric()) {
            parseInt();
            newIntValue();
            return;
        }
        _env = _evalenv;
        // Get the variable's value from the environment
        do {
            if (_str == _env->varname) {
                _value = _env->value;
                return;
            }
        } while ((_env = _env->next));
        _value = NULL;
        return;
    }

    // Is an int or a lambda
    if (nodetype == INT || nodetype == LAMBDA) {
        _value = node;
        return;
    }

    // Is a list

    // Is ()
    if (!(node->value)) {
        _value = NULL;
        return;
    }

    // The head of the list is an atom
    if (node->value->type == ATOM) {
        #define headstr _str
        headstr = node->value->str;

#ifdef ELVM
        if ((int)last_op < (int)headstr) {
            goto eval_lambda_call;
        }
#endif
        if (node->next) {
            arg1 = node->next->value;
            if (node->next->next) {
                arg2list = node->next->next;
            }
        }

#ifdef ELVM
    // k = ((int)_str - 11)/2;
    // k = ((int)_str - 11);
    // computed_goto = evalhash[k];
    goto *(evalhash[((int)_str - 11) >> 1]);

// #define lambda_str 11
// #define print_str 18
// #define define_str 24
// #define quote_str 31
// #define list_str 37
// #define if_str 42
// #define car_str 45
// #define while_str 49
// #define progn_str 55
// #define macro_str 61
// #define lambdaast_str 67
// #define eq_str 75
// #define cons_str 78
// #define plus_str 83
// #define t_str 85
// #define mod_str 87
// #define eval_str 91
// #define cdr_str 96
// #define minus_str 100
// #define ast_str 102
// #define lt_str 104
// #define gt_str 106
// #define slash_str 108
// #define atom_str 110

#else

        void* test__[2] = {&&eval_define, &&eval_if};
        // test__ = &&eval_define;

             if (_str == define_str) {goto *test__[0];}
        else if (_str == if_str) {goto *test__[1];}
        else if (_str == quote_str) goto eval_quote;
        else if (_str == car_str) goto eval_car;
        else if (_str == cdr_str) goto eval_cdr;
        else if (_str == cons_str) goto eval_cons;
        else if (_str == atom_str) goto eval_atom;
        else if (_str == print_str) goto eval_print;
        else if (_str == progn_str) goto eval_progn;
        else if (_str == while_str) goto eval_while;
        else if (_str == lambda_str || _str == macro_str || _str == lambdaast_str) goto eval_createlambda;
        else if (_str == eval_str) goto eval_eval;
        else if (_str == eq_str) goto eval_eq;
        else if (_str == plus_str || _str == minus_str || _str == ast_str || _str == slash_str || _str == mod_str) goto eval_arith;
        else if (_str == lt_str || _str == gt_str) goto eval_cmp;
        else if (_str == list_str) goto eval_list;
        else goto eval_lambda_call;
#endif
        // if (_str == define_str) {
eval_define:;
            eval(arg2list->value);
            _env = _evalenv;
            do {
                if (_env->varname == arg1->str){
                    _env->value = _value;
                    return;
                }
            } while(_env->next && (_env = _env->next));
            _env3 = _env;
            debug("appending to global environment...\n");

            // Append to the global environment
            _str = arg1->str;
            _env = NULL;
            _env3->next = newEnv();
            debug("appended to global environment.\n");
            return;
        // }
        // if (_str == if_str) {
eval_if:
            #define condition _value
            eval(arg1);
            eval(condition ? arg2list->value : arg2list->next->value);
            return;
            #undef condition
        // }
        // if (_str == car_str) {
eval_car:
            eval(arg1);
            if (_value) {
                _value = _value->value;
            }
            return;
        // }
        // if (_str == cdr_str) {
eval_cdr:
            eval(arg1);
            if (_value) {
                _value = _value->next;
            }
            return;
        // }
        // if (_str == list_str) {
eval_list:
            #define initlist _list_eval
            #define curlist node
            if (!arg1) {
                _value = nil;
            } else {
                eval(arg1);
                initlist = newList(_value, NULL);
                arg1 = node->next;
                curlist = initlist;
                while ((arg1 = arg1->next)) {
                    eval(arg1->value);
                    curlist->next = newList(_value, NULL);
                    curlist = curlist->next;
                }
                _value = initlist;
            }
            return;
            #undef initlist
            #undef curlist
        // }
        // if (_str == cons_str) {
eval_cons:
            #define car node
            eval(arg1);
            car = _value;
            eval(arg2list->value);
            _value = newList(car, _value);
            // _value = newListNode();
            return;
            #undef car
        // }
        // if (_str == print_str) {
eval_print:
            eval(arg1);
            _list_eval = _value;
            printValue();
            if(node->next->next) {
                putchar('\n');
            }
            _value = _list_eval;
            return;
        // }
        // if (_str == progn_str) {
eval_progn:
            #define curlist _list_eval
            curlist = node->next;
            _value = NULL;
            while (curlist) {
                eval(curlist->value);
                curlist = curlist->next;
            }
            return;
            #undef curlist
        // }
        // if (_str == while_str) {
eval_while:
            _value = NULL;
            while (eval(arg1), _value) {
                eval(arg2list->value);
            }
            return;
        // }
        // if (_str == lambda_str || _str == macro_str || _str == lambdaast_str) {
eval_createlambda:
            newLambdaValue(
                _lambda,
                (arg1->value ? arg1 : NULL),
                (arg2list->value),
                _evalenv,
                (headstr[0] == 'm' ? L_MACRO : headstr[6] == '*' ? L_LAMBDA :  L_CLOSURE)
            );
            newLambdaNode();
            return;
        // }
        // if (_str == eq_str) {
eval_eq:
            eval(arg1);
            node = _value;
            eval(arg2list->value);
            #define n1 node
            #define n2 _value
            // Nil equality
            if (n1 == n2) {
                _value = true_value;
            }
            else if (!n1 || !n2) {
                _value = NULL;
            }
            // Integer equality
            else if (n1->type == INT && n2->type == INT && n1->n == n2->n) {
                _value = true_value;
            }
            // Atom equality
            else if (n1->type == ATOM && n2->type == ATOM && (n1->str == n2->str)) {
                _value = true_value;
            } else {
                _value = NULL;
            }
            return;
            #undef n1
            #undef n2
        // }
        // if (_str == plus_str || _str == minus_str || _str == ast_str || _str == slash_str || _str == mod_str) {
eval_arith:
            c_eval = headstr[0];

            #define nextlist _list_eval_2
            nextlist = node->next;
            _value = nextlist->value;
            evalAsInt();
            n_ = i;
            if (c_eval == '-' && !(nextlist->next)) {
                i = -n_;
            } else {
                for (nextlist = nextlist->next; nextlist; nextlist = nextlist->next) {
                    _value = nextlist->value;
                    evalAsInt();
                    n_ = (
                        c_eval == '+' ? (n_ + i) :
                        c_eval == '-' ? (n_ - i) :
                        c_eval == '*' ? (n_ * i) :
                        c_eval == '/' ? (n_ / i) :
                        (n_%i)
                    );
                }
                i = n_;
            }
            #undef nextlist
            newIntValue();
            return;
        // }
        // if (_str == lt_str || _str == gt_str) {
eval_cmp:
            c_eval = headstr[0];
            _value = arg2list->value;
            evalAsInt();
            n_ = i;
            eval(arg1);
            j = _value->n < n_;
            _value = (c_eval == '<' ? j : !j) ? true_value : NULL;
            return;
        // }
        // if (_str == quote_str) {
eval_quote:
            _value = arg1;
            return;
        // }
        // if (_str == atom_str) {
eval_atom:
            eval(arg1);
            _value = !_value || (_value->type == ATOM) || (_value->type == INT) ? true_value : NULL;
            return;
        // }
        // if (_str == eval_str) {
eval_eval:
            eval(arg1);
            eval(_value);
            return;
        // }
        #undef headstr
    }

eval_lambda_call:
    debug("calling lambda...\n");
    #define curargname _list_eval
    #define curarg _list_eval_2
    #define curenv evalstack_env
    #define curlambda ((Lambda*) node) 
    // If the head of the list is a list or an atom not any of the above,
    // it is expected for it to evaluate to a lambda.
    curarg = node->next;
    eval(node->value);
    node = (Value*)(_value->lambda);
    // curlambda = _value->lambda;
    curargname = curlambda->argnames;


    // Th body of the macro should be evaluated in the environment they are called in,
    // instead of the environment they were defined in
    curenv = (curlambda->type == L_MACRO) ? _evalenv : curlambda->env;

    while (curargname) {
        // Set argument to nil if there are no arguments
        if (curlambda->type == L_MACRO) {
            if (curarg) {
                _value = curarg->value;
            } else {
                _value = nil;                
            }
        } else {
            if (curarg) {
                eval(curarg->value);
            } else {
                _value = NULL;
            }
        }
        _str = curargname->value->str;
        _env = curenv;
        if (curlambda->type == L_CLOSURE) {
            curenv = newEnv();
        } else {
            prependTemporaryEnv(curenv);
        }
        curargname = curargname->next;
        if (curarg) {
            curarg = curarg->next;
        }
    }

    // For macros, evaluate the result before returning it
    // Env* tempenv = _evalenv;
    // Env* temp2 = curenv;
    // _evalenv = temp2;
    evalstack_env2 = _evalenv;
    _evalenv = curenv;

    eval(curlambda->body);
    if (curlambda->type == L_MACRO) {
        _evalenv = _evalenv;
        // _evalenv = curlambda->env;
        // _evalenv = temp2;
        eval(_value);
    }
    _evalenv = evalstack_env2;
    // _evalenv = tempenv;
    #undef curargname
    #undef curarg
    #undef curenv
    #undef curlambda
}
#undef _list_eval
#undef _list_eval_2
#undef arg1
#undef arg2list
#undef c_eval
#undef n_
#undef evalstack_env
#undef evalstack_env2
#undef nodetype

#define v _value
void printValue() {
    Value* list;
    if (!_value) {
        debug("<nil1>");
        putchar('(');
        putchar(')');
        return;
    }

    k = v->type;
    if (k == INT) {
        debug("<int>");
        #define p _str
        k = v->n;
        if (k < 0) {
            putchar('-');
            k = -k;
        }
        p = buf + 5;
        *p = '\0';
        do {
            _div(k, 10);
            p--;
            *p = (r + '0');
            k = q;
        } while (k);
        #undef p
    } else if (!(_value->value)) {
        debug("<nil2>");
        putchar('(');
        putchar(')');
        return;
    } else if (k == LAMBDA) {
        debug("<lambda>");
        k = v->lambda->type;
        _str = (k == L_LAMBDA) ? "#<Lambda>" : (k == L_MACRO) ? "#<Macro>" : "#<Closure>";
    } else if (k == ATOM) {
        debug("<atom>");
        _str = v->str;
    } else {
        debug("<list>");
        putchar('(');
        list = v;
        while(list) {
            _value = list->value;
            printValue();
            if ((list = list->next)) {
                putchar(' ');
            }
        }
        putchar(')');
        return;
    }
    for (; *_str; _str++){
        putchar(*_str);
    }
}
#undef v

int main (void) {
    // evalhash[0] = 1;
#ifndef memdumpopt2
    // (Value*)LIST, since ->type and ->next are inside the same union
    nil = newList(NULL, (Value*)LIST);
    // TODO: get this value from the string table
    _str = t_str;
    newAtomNode(t_str);
    true_value = _value;

    newStringTable(_stringtable, _value);
    stringTableHead = _stringtable;
#  ifndef ELVM
    char* opstr_list[num_ops] = {eval_str, lambdaast_str, atom_str, quote_str, macro_str, define_str, while_str, progn_str, lambda_str, gt_str, lt_str, plus_str, minus_str, ast_str, slash_str, t_str, mod_str, print_str, cons_str, cdr_str, car_str, eq_str, if_str, list_str};
    for(j=0; j<num_ops; j++){
        _str = opstr_list[j];
        s1 = _str;
        i = 0;
        sthash = 0;
        for(; *s1; s1++,i++){sthash += *s1;}
        sthash_mod16();
        // getOrSetAtomFromStringTable(stringTableHead, _str);
        // __stringtable = stringTableHead;
        __targetstring = _str;
        getOrSetAtomFromStringTable();

    }
#  else
    s3 = opstring_head;
    // s1 = eval_str;
    for(j=0; j<num_ops; j++){
        _str = s3;
        // s3 = _str;
        i = 0;
        sthash = 0;
        for(; *s3; s3++,i++){sthash += *s3;}
        sthash_mod16();
        // getOrSetAtomFromStringTable(stringTableHead, _str);
        // __stringtable = stringTableHead;
        __targetstring = _str;
        getOrSetAtomFromStringTable();

        // for(; *s1; s1++){}
        s3++;
    }
#  endif
    getOrSetAtomFromStringTable_newflag = 1;

    _str = "";
    _value = NULL;
    _env = NULL;
    _evalenv = newEnv();
    // TODO: get progn_str from the string table
    // _str = (char*) progn_str;
    newAtomNode(progn_str);
    initlist = newList(_value, NULL);
    curlist = initlist;

#endif
#ifndef memdumpopt1
    while((parseExpr(), _value)) {
        curlist->next = newList(_value, NULL);
        curlist = curlist->next;
    }
    
    // _list = initlist;
    // _value = newListNode();
    // _value = initlist;
    // printValue();
    // eval(_value);

    eval(initlist);
#  ifdef ELVM
    *((char*)(QFTASM_RAMSTDIN_BUF_STARTPOSITION)) = 0;
#  endif
#endif
}
