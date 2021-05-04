#ifndef GCC
#define ELVM
#endif

#ifdef ELVM
#    define DEFLOCATION extern
#else
#    define DEFLOCATION
#endif

DEFLOCATION char* _str;
DEFLOCATION int q;
DEFLOCATION int r;
DEFLOCATION unsigned long long i;
DEFLOCATION int j;
DEFLOCATION unsigned long k;
DEFLOCATION int _malloc_bytes;
DEFLOCATION void* _malloc_result;
extern int evalhash;



#ifdef ELVM
#include "elvm.h"
#else
#include "lisp.h"
#endif

// #define QFTASM_HEAP_MEM_MAX 2846

#ifndef ELVM
#include <stdio.h>
#  define debug(x)      // printf(x)
#  define debug1(x,y)   // printf(x,y)
#  define debug1_2(x,y) // printf(x,y)
#  define debug2(x,y,z) // printf(x,y,z)
#else
#  define debug(x)
#  define debug1(x,y)
#  define debug1_2(x,y)
#  define debug2(x,y,z)
#endif

// ATOM=1, since .type and .next of Value is a union, and .next is usually set to NULL
#ifdef ELVM
typedef enum {
    ATOM   = (unsigned long long)1<<14,
    INT    = (unsigned long long)2<<14,
    LAMBDA = (unsigned long long)3<<14,
} Valuetype;
#else 
typedef enum {
    ATOM   = (unsigned long long)1<<62,
    INT    = (unsigned long long)2<<62,
    LAMBDA = (unsigned long long)3<<62,
} Valuetype;
#endif

#define typemask LAMBDA

#define isIntValue(x) ((((unsigned long long)(x)) & typemask) == INT)
#define isAtomValue(x) ((((unsigned long long)(x)) & typemask) == ATOM)


typedef struct Value {
    unsigned long type;
    union {
        char* str;
        int n;
        struct Lambda* lambda;
        struct Value* value;
    };
} Value;

typedef struct List {
    union {
        unsigned long type;
        struct List* next;
    };
    struct Value* value;
} List;


typedef struct StringTable {
    char* varname;
    // Value* value;
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
    struct List* argnames;
    struct List* body;
    struct Env* env;
    Lambdatype type;
} Lambda;


DEFLOCATION char charbuf;
DEFLOCATION char c;

char buf[32];
DEFLOCATION char* s1;
DEFLOCATION char* s2;
DEFLOCATION char* s3;


#include <values.h>

DEFLOCATION StringTable* _stringtable;

DEFLOCATION Lambda* _lambda;

DEFLOCATION Env* _env;
DEFLOCATION Env* _env2;
DEFLOCATION Env* _env3;

DEFLOCATION Value* true_value;
// DEFLOCATION Value* true_value = &t_value;

#ifdef ELVM
DEFLOCATION Env* _evalenv;
DEFLOCATION Value* nil;
DEFLOCATION List* initlist;
DEFLOCATION List* curlist;
#else
DEFLOCATION Env* _evalenv = &initialenv;
DEFLOCATION Value* nil = (Value*)&nil_value;
DEFLOCATION List* initlist = &nil_value;
DEFLOCATION List* curlist = &nil_value;
#endif


DEFLOCATION Value* _value;
DEFLOCATION List* _list;


DEFLOCATION int sthash;

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

#define str2Atom(__str) {                                   \
    debug("str2Atom\n");                                    \
    _value = (Value*) (((unsigned long long)__str) | ATOM); \
}

#define atom2Str(__value) {                                       \
    debug("atom2Str\n");                                          \
    _str = (char*) (((unsigned long long)__value) & (~typemask)); \
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

List* newList(Value* node, List* next) {
#define ret _list
    // _malloc_bytes = sizeof(Value);
    malloc_k(sizeof(List), ret);
    // ret = (Value*) _malloc_result;
    debug("newList\n");
    ret->value = node;
    ret->next = next;
    return ret;
#undef ret
}


#define newStringTable(__stringtable, __str) {      \
    malloc_k(sizeof(StringTable), __stringtable);   \
    debug("newStringTable\n");                      \
    _stringtable->varname = __str;                  \
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


#define newIntValue() {                                \
    debug("newIntValue\n");                            \
    _value = (Value*) (((unsigned long long)i) | INT); \
}

#define pushTailList(__value) {      \
    _list = newList(__value, NULL);  \
    listTail->next = _list;          \
    listTail = _list;                \
}

void parseExpr(List* listTail) {
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
            if (isEOF(c)) {
                // _value = listHead;
                return;
            }
        } while(c != '\n');
        // charbuf = 0;
        goto parseExprHead;
    }
    if (isEOF(c)) {
        // _value = listHead;
        return;
    }
    // Parse as a list
    if (c == '(') {
        c = getchar();

        debug("pushing list...\n");
        parseExpr(listTail);

        _list = listTail->next;
        pushTailList(_list ? (Value*)_list : nil);
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
    while (isNotEOF(c) && c != ' ' && c != '\n' && c != ')' && c != '(' && c != ';') {
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
    s2 = stringtable->varname;
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
            newStringTable(_stringtable, _str);
            *branch = _stringtable;
            goto getOrSetAtomFromStringTable_end;
        }
    }
    // The strings were equal
    debug("The strings have matched!\n");
    // _value = stringtable->value;
    _str = stringtable->varname;

getOrSetAtomFromStringTable_end:

    str2Atom(_str);
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
#define evalAsInt() {                                                         \
    if (!isIntValue(_value)) {                                                 \
        eval(_value);                                                         \
    }                                                                         \
    i = (unsigned long long)(((unsigned long long)_value) & (~typemask));      \
}

typedef struct {
    union {
        char c_eval_;
        List* _list_eval_;
        Value* _value_eval;
    };
    union {
        Value* arg1_;
        List* _list_eval_2;
        Env* e2;
    };
    union {
        List* arg2list_;
        int n_;
        Env* e;
    };
} EvalStack;


void eval(Value* node) {
    EvalStack evalstack;
    debug("entering eval...\n");

#define _value_eval (evalstack._value_eval_)
#define _list_eval (evalstack._list_eval_)
#define _list_eval_2 (evalstack._list_eval_2)
#define arg1 (evalstack.arg1_)
#define arg2list (evalstack.arg2list_)
#define c_eval (evalstack.c_eval_)
#define n_ (evalstack.n_)
#define evalstack_env (evalstack.e)
#define evalstack_env2 (evalstack.e2)
#define nodetype k

    // If the top bit is 1, it is an integer, so return the value itself
    if (isIntValue(node)) {
        _value = node;
        return;
    }

    if (isAtomValue(node)) {
        atom2Str(node);
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

    nodetype = node->type;

    // // Is an int or a lambda
    // if (nodetype == INT || nodetype == LAMBDA) {
    //     _value = node;
    //     return;
    // }
    // Is a lambda
    if (nodetype == LAMBDA) {
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
    if (isAtomValue(node->value)) {
        #define headstr _str
        // headstr = node->value->str;
        atom2Str(node->value);

#ifdef ELVM
        if ((int)last_op < (int)headstr) {
            goto eval_lambda_call;
        }
#endif
        if (((List*)node)->next) {
            arg1 = ((List*)node)->next->value;
            if (((List*)node)->next->next) {
                arg2list = ((List*)node)->next->next;
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
            atom2Str(arg1);
            do {
                if (_env->varname == _str){
                    _env->value = _value;
                    return;
                }
            } while(_env->next && (_env = _env->next));
            _env3 = _env;
            debug("appending to global environment...\n");

            // Append to the global environment
            // _str = arg1->str;
            // atom2Str(arg1);
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
                _value = (Value*)((List*)_value)->next;
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
                arg1 = (Value*)((List*)node)->next;
                curlist = (Value*)initlist;
                while ((arg1 = (Value*)((List*)arg1)->next)) {
                    eval(arg1->value);
                    ((List*)curlist)->next = newList(_value, NULL);
                    curlist = (Value*)((List*)curlist)->next;
                }
                _value = (Value*)initlist;
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
            _value = (Value*)newList(car, (List*)_value);
            // _value = newListNode();
            return;
            #undef car
        // }
        // if (_str == print_str) {
eval_print:
            eval(arg1);
            _list_eval = (List*)_value;
            printValue();
            if( ((List*)((List*)node)->next)->next) {
                putchar('\n');
            }
            _value = (Value*)_list_eval;
            return;
        // }
        // if (_str == progn_str) {
eval_progn:
            #define curlist _list_eval
            curlist = ((List*)node)->next;
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
                (List*)(arg1->value ? arg1 : NULL),
                (List*)(arg2list->value),
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
            // Nil equality and integer equality, and atom equality.
            // Integers are passed as raw values with a flag at the top bit instead of a
            // pointer to a value, so equal integers always have the same n1 and n2.
            // Atoms are the same except the raw values are constant string pointers.
            if ((unsigned long long)n1 == (unsigned long long)n2) {
                _value = true_value;
            }
            // else if (isIntValue(n1) && isIntValue(n2) && n1 != n2) {
            //     _value = NULL;
            // }
            else if (!n1 || !n2) {
                _value = NULL;
            }
            // // Integer equality
            // else if (n1->type == INT && n2->type == INT && n1->n == n2->n) {
            //     _value = true_value;
            // }
            // // Atom equality
            // else if (n1->type == ATOM && n2->type == ATOM && (n1->str == n2->str)) {
            //     _value = true_value;
            // } 
            else {
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
            nextlist = ((List*)node)->next;
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
            _value = arg1;
            evalAsInt();
            j = i < n_;
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
            // Integers are passed as raw values, so their types must be evaluated first
            _value = !_value || (isIntValue(_value)) || (_value->type == ATOM) ? true_value : NULL;
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
    curarg = ((List*)node)->next;
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
        // _str = curargname->value->str;
        atom2Str(curargname->value);
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

    eval((Value*)curlambda->body);
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
#undef _value_eval
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
    List* list;
    if (!_value) {
        debug("<nil1>");
        list = NULL;
        goto printlist;
    }

    if (isIntValue(_value)) {
        debug("<int>");
        #define p _str
        k = ((unsigned long long)v) & (~typemask);
        debug1_2("[%lld]", (unsigned long long)v);
        debug1_2("[%ld]", k);
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
    } else if (isAtomValue(_value)){
        debug("<atom>");
        atom2Str(v);
    } else {
        k = v->type;
        if (k == LAMBDA) {
            debug("<lambda>");
            k = v->lambda->type;
            _str = (k == L_LAMBDA) ? "#<Lambda>" : (k == L_MACRO) ? "#<Macro>" : "#<Closure>";
        } else {
            debug("<list>");
            list = (List*)v;
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
    }
    for (; *_str; ++_str){
        putchar(*_str);
    }
}
#undef v

int main (void) {
    str2Atom(t_str);
    true_value = _value;
    c = getchar();
    do {
        parseExpr(curlist);
    } while((curlist = curlist->next));
    
    initlist = ((List*)nil)->next;
    ((List*)nil)->next = NULL;
    while (initlist) {
        eval(initlist->value);
        // 
        // _value = initlist->value;
        // printValue();
        initlist = initlist->next;
    }
#ifdef ELVM
    *((char*)(QFTASM_RAMSTDIN_BUF_STARTPOSITION)) = 0;
#endif
}
