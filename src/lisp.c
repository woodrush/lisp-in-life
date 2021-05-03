char* _str;
int q, r;
int i = 777;
int j = 888;
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

#ifndef ELVM
// #include <stdio.h>
#  define debug(x) //printf(x)
#  define debug1(x,y) //printf(x,y)
#  define debug2(x,y,z) //printf(x,y,z)
#else
#  define debug(x)
#  define debug1(x,y)
#  define debug2(x,y,z)
#endif

// ATOM=1, since .type and .next of Value is a union, and .next is usually set to NULL
typedef enum {
    // ATOM=1, INT=2, LAMBDA=3, LIST=4
    // INT  =  0b00 << 14,
    // ATOM = 0b01 << 14,
    // LAMBDA = 0b10 << 14,
    // LIST = 0b11 << 14,
    INT = 0,
    ATOM = 1,
    LAMBDA = 2,
    LIST = 3,
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

#define value_type(x) (x->n & LIST)
#define value_ptr(x) ((Value*)(((long)x) >> 2))

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


#include <values.h>

StringTable* _stringtable;

Lambda* _lambda;

Env* _env;
Env* _env2;
Env* _env3;
Env* _evalenv = &initialenv;


Value* nil = &nil_value;
Value* true_value = &t_value;

Value* _value;
Value* _list;
Value* initlist = &nil_value;
Value* curlist = &nil_value;

int sthash;

#define sthash_mod16() { sthash = sthash &~ 0b1111111111110000; }

int mlb;
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
    if (m == 2) {
        q = n >> 1;
        r = n &~ 65534;
    } else {
        q = 0;
        while(n >= m){
            n -= m;
            ++q;
        }
        q = sign ? q : -q;
        r = sign_n ? n : -n;
    }
    #undef sign_n
    #undef sign
}

//================================================================================
// Parser
//================================================================================


#define newAtomNode(__str) {         \
    malloc_k(sizeof(Value), _value); \
    debug("newAtomNode\n");          \
    _value->type = 0;             \
    _value->str = (char*)(((long)__str + (long)__str + (long)__str + (long)__str) ^ ATOM);             \
}

#define newLambdaStruct(__target, __argnames, __body, __env, __type) {  \
    malloc_k(sizeof(Lambda), __target);                                \
    debug("lambda 1\n");                                               \
    _lambda->argnames = __argnames;                                    \
    _lambda->body = __body;                                            \
    _lambda->env = __env;                                              \
    _lambda->type = __type;                                            \
}

#define newLambdaValue() {            \
    malloc_k(sizeof(Value), _value); \
    debug("lambda 2\n");             \
    _value->type = 0;           \
    _value->lambda = (Lambda*)(((long)_lambda + (long)_lambda + (long)_lambda + (long)_lambda) ^ LAMBDA);        \
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


#define newStringTable(__stringtable, __value) {    \
    newAtomNode(_str);                              \
    malloc_k(sizeof(StringTable), __stringtable);   \
    debug("newStringTable\n");                      \
    _stringtable->value = __value;                  \
    _stringtable->lesser = NULL;                    \
    _stringtable->greater = NULL;                   \
}

StringTable* stringtable;
StringTable** branch;


// j : sign
#define parseInt() {                 \
    j = 1;                           \
    i = 0;                           \
    if (_str[0] == '-') {            \
        j = 0;                       \
        ++_str;                      \
    }                                \
    while (*_str) {                  \
        /* i *= 10 */                \
        i += i;                      \
        k = i;                       \
        i += i;                      \
        i += i + k + (*_str - '0');  \
        ++_str;                      \
    }                                \
    i = j ? i : -i;                  \
}
#undef str
#undef sign

#define newIntValue() {              \
    malloc_k(sizeof(Value), _value); \
    debug("newIntValue\n");          \
    _value->type = 0;              \
    _value->n = (i + i + i + i) ^ (int)INT;                   \
}

#define pushTailList(__value) {             \
    _list = newList(__value, NULL);         \
    listTail->next = _list;  \
    listTail = _list;            \
}

void parseExpr(Value* listTail) {
parseExprHead:;
    // Remove whitespace
// space:;
    // if (!c) {
    //     c = getchar();
    // }
    while (c == ' ' || c == '\n') {
        c = getchar();
    }
    if (c == ';') {
        do {
            c = getchar();
            if (c == EOF) {
                // _value = listHead;
                return;
            }
        } while(c != '\n');
        // charbuf = 0;
        goto parseExprHead;
    }
    if (c == EOF) {
        // _value = listHead;
        return;
    }
    // Parse as a list
    if (c == '(') {
        c = getchar();

        debug("pushing list...\n");
        parseExpr(listTail);

        _list = listTail->next;
        pushTailList(_list ? _list : nil);
        goto parseExprHead;
    }

    // Parse as an atom
// #ifdef ELVM
//     if (c == ')' || !c) {
// #else
    if (c == ')') {
        debug1("popping list...\n%s", "");

        c = getchar();
        return;
    }

    i = 0;
// #ifdef ELVM
//     while (c != ' ' && c != '\n' && c != ')' && c != '(' && c != ';') {
// #else
    sthash = 0;
    while (c != ' ' && c != '\n' && c != ')' && c != '(' && c != ';' && c != EOF) {
// #endif
        // putchar(c);
        buf[i] = c;
        sthash += c;
        ++i;
        c = getchar();
    }
    buf[i] = '\0';


    // If the expression is an integer literal, evaluate it
    charbuf = buf[0];
    if (('0' <= charbuf && charbuf <= '9') || (charbuf == '-' && ('0' <= buf[1] && buf[1] <= '9'))) {
        _str = buf;
        parseInt();
        newIntValue();
        pushTailList(_value);
        goto parseExprHead;
    }


    sthash_mod16();

    stringtable = stringTableHeadList[sthash];
    if (!stringtable) {
        branch = (stringTableHeadList + sthash);
        goto getOrSetAtomFromStringTable_setstringtable;
    }
getOrSetAtomFromStringTableHead:
    s1 = buf;
    s2 = stringtable->value->str;
    debug2("%s v.s. %s (the input)\n", s1, s2);
    for (; *s1 || *s2; ++s1, ++s2) {
        // The strings were not equal
        if (*s1 != *s2) {
            branch = *s1 < *s2 ? &(stringtable->lesser) : &(stringtable->greater);
            // There are no more strings that could match in the table
            if ((stringtable = *branch)) {
                // There is a string table that we could proceed to search
                debug("Continuing search...\n");
                goto getOrSetAtomFromStringTableHead;
            }
getOrSetAtomFromStringTable_setstringtable:
            debug("Creating new stringtable entry with a new string pointer...\n");

            // This was the last string in the table, so create a string
            malloc_k(i+1, _str);
            s1 = _str;
            s2 = buf;
            debug("parseAtom\n");
            for(; *s2; ++s1, ++s2) {
                *s1 = *s2;
            }
            newAtomNode(_str);
            newStringTable(_stringtable, _value);
            *branch = _stringtable;
            goto getOrSetAtomFromStringTable_end;
        }
    }
    // The strings were equal
    debug("The strings have matched!\n");
    _value = stringtable->value;

getOrSetAtomFromStringTable_end:


    pushTailList(_value);
    goto parseExprHead;
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
#define evalAsInt() {           \
    if (value_type(_value) != INT) {  \
        eval(_value);           \
    }                           \
    i = _value->n;              \
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

extern int evalhash;

void eval(Value* node) {
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

    nodetype = value_type(node);
    node = (Value*)((long)node >> 2);
    // Is an atom
    if (nodetype == ATOM) {
        _str = node->str;
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
    if (value_type(node->value) == ATOM) {
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
    goto *((void*)*((int*)((int)&evalhash + (((int)_str) >> 1) )));
#else

        void* test__[2] = {&&eval_define, &&eval_if};

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
            newLambdaStruct(
                _lambda,
                (arg1->value ? arg1 : NULL),
                (arg2list->value),
                _evalenv,
                (headstr[0] == 'm' ? L_MACRO : headstr[6] == '*' ? L_LAMBDA :  L_CLOSURE)
            );
            newLambdaValue();
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
            else if (value_type(n1) == INT && value_type(n2) == INT && n1->n == n2->n) {
                _value = true_value;
            }
            // Atom equality
            else if (value_type(n1) == ATOM && value_type(n2) == ATOM && (n1->str == n2->str)) {
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
            _value = !_value || (value_type(_value) == ATOM) || (value_type(_value) == INT) ? true_value : NULL;
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
        // _evalenv = _evalenv;
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
        list = NULL;
        goto printlist;
    }

    k = value_type(v);
    putchar('t');
    putchar('0' + k);
    v = value_ptr(v->value);
    // putchar('a' + (long)v);
    // _value = (Value*)(((long)_value) >> 2);
    // putchar('0' + (long)_value);
    if (k == INT) {
        debug("<int>");
        #define p _str
        // k = v->n;
        k = (long)v;
        if (k < 0) {
            putchar('-');
            k = -k;
        }
        p = buf + 5;
        *p = '\0';
        do {
            c = k >> 1;
            _div(c, 5);
            --p;
            *p = (r + r + (k &~ 65534 ? 1 : 0) + '0');
            k = q;
        } while (k);
        #undef p
    } else if (k == LAMBDA) {
        debug("<lambda>");
        // k = v->lambda->type;
        k = ((Lambda*)v)->type;
        _str = (k == L_LAMBDA) ? "#<Lambda>" : (k == L_MACRO) ? "#<Macro>" : "#<Closure>";
    } else if (k == ATOM) {
        debug("<atom>");
        // _str = v->str;
        _str = (char*)v;
    } else {
        debug("<list>");
        list = v;
printlist:
        putchar('(');
        while(list && (_value = list->value)) {
            // _value = list->value;
            printValue();
            if ((list = list->next)) {
                putchar(' ');
            }
        }
        putchar(')');
        return;
    }
    for (; *_str; ++_str){
        putchar(*_str);
    }
}
#undef v

char aaa[] = "asdf";
int main (void) {
    // return 0;
    i = 123;
    newIntValue();
    printValue();

    newAtomNode(aaa);
    printValue();

    newLambdaStruct(_lambda,0,0,0,0);
    newLambdaValue();
    printValue();
    return 0;

    c = getchar();
    do {
        parseExpr(curlist);
    } while((curlist = curlist->next));
    
    initlist = nil->next;
    nil->next = NULL;
    while (initlist) {
        eval(initlist->value);
        initlist = initlist->next;
    }
#ifdef ELVM
    *((char*)(QFTASM_RAMSTDIN_BUF_STARTPOSITION)) = 0;
#endif
}
