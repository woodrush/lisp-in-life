#ifdef ELVM
#include "elvm.h"
#else
#include "lisp.h"
#endif

// #include <stdio.h>
#define debug(x) //printf(x)

char charbuf = -1;

char popchar() {
    if (charbuf < 0) {
        return getchar();
    }
    char ret = charbuf;
    charbuf = -1;
    return ret;
}

char curchar() {
    if (charbuf < 0) {
        charbuf = getchar();
    }
    return charbuf;
}



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

void print_int(int k) {
    char* p;
    char buf[10];
    int i_, j_;
    if (k < 0) {
        putchar('-');
        k = -k;
    }
    p = buf + 5;
    *p = '\0';
    do {
        i_ = i;
        j_ = j;
        _div(k, 10);
        i = i_;
        j = j_;
        p--;
        *p = (r + '0');
        k = q;
    } while (k);
    for (; *p; p++){
        putchar(*p);
    }
    putchar('\n');
}

//================================================================================
// Parser
//================================================================================

// ATOM=1, since .type and .next of Value is a union, and .next is usually set to a null value
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
    struct StringTable* next;
} StringTable;


Value* _value;
#define str_in _str
void newAtomNode() {
#define ret _value
    malloc_bytes = sizeof(Value);
    _value = (Value*)malloc_k();
    debug("newAtomNode\n");
    _value->type = ATOM;
    _value->str = str_in;
    // return ret;
#undef ret
}
#undef str_in

Value* nil;
Value* _list;

Value* newList(Value* node, Value* next) {
#define ret _list
    malloc_bytes = sizeof(Value);
    ret = (Value*)malloc_k();
    debug("newList\n");
    ret->value = node;
    ret->next = next;
    return ret;
#undef ret
}

void parseExpr();

char c;
char buf[32];

StringTable* stringTableHead = NULL;
StringTable* _stringtable;

void appendStringTable() {
    malloc_bytes = sizeof(StringTable);
    _stringtable = malloc_k();
    debug("appendStringTable\n");
    newAtomNode();
    _stringtable->value = _value;
    _stringtable->next = stringTableHead;
    stringTableHead = _stringtable;
}

char* s1;
char* s2;

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
    malloc_bytes = sizeof(Value);
    ret = malloc_k();
    debug("newIntValue\n");
    ret->type = INT;
    ret->n = i;
#undef ret
}

void parseExpr() {
    // Remove whitespace
space:;
    c = curchar();
    if(c == ' ' || c == '\n') {
        popchar();
        goto space;
    }
    if (c == ';') {
        do {
            c = popchar();
// #ifdef ELVM
//         } while(c != '\n');
// #else
        } while(c != '\n' && c != EOF);
// #endif
        goto space;
    }

    // Parse as a list
    if (c == '(') {
        popchar(); // '('

        _value = parseListLoop();

        popchar(); // ')'

        _value = _value ? _value : nil;
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
    while (c != ' ' && c != '\n' && c != ')' && c != '(' && c != ';' && c != EOF) {
// #endif
        buf[i] = c;
        i++;
        popchar();
        c = curchar();
    }
    buf[i] = '\0';

    // If the expression is an integer literal, evaluate it
    c = buf[0];
    if (('0' <= c && c <= '9') || c == '-' && ('0' <= buf[1] && buf[1] <= '9')) {
        _str = buf;
        parseInt();
        newIntValue();
        return;
    }


    _stringtable = stringTableHead;

parseatomloop:;
    s1 = buf;
    s2 = _stringtable->value->str;
    for(; *s1 || *s2; s1++, s2++) {
        if (*s1 != *s2) {
            // There is a string next to this string in the table
            if ((_stringtable = _stringtable->next)) {
                goto parseatomloop;
            }

            // This was the last string in the table, so create a string
            malloc_bytes = i+1;
            _str = malloc_k();
            debug("parseAtom\n");
            s1 = _str;
            s2 = buf;
            for(; *s2; s1++, s2++) {
                *s1 = *s2;
            }
            appendStringTable();
            return;
        }
    }
    // The strings are equal
    _value = _stringtable->value;    
}

//================================================================================
// Evaluator
//================================================================================
typedef enum {
    ENV_PERSISTENT=0, ENV_TEMPORARY=1
} Envtype;

typedef struct Env {
    char* varname;
    union {
        Envtype type;
        struct Env* prev;
    };
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


Lambda* _lambda;

Env* _env;
Env* _env2;
#define varname_in _str
#define value_in _value
#define env_in _env
#define env _env2
Env* newEnv() {
    malloc_bytes = sizeof(Env);
    env = malloc_k();
    debug("newEnv\n");
    env->type = ENV_PERSISTENT;
    env->varname = varname_in;
    env->value = value_in;
    env->next = env_in;
    // env->prev = NULL;
    return env;
}
#undef varname_in
#undef value_in
#undef env_in
#undef env

Env* prependTemporaryEnv() {
    if (_env->prev) {
        _env2 = _env->prev;
    } else {
        malloc_bytes = sizeof(Env);
        _env2 = malloc_k();
        debug("newEnv\n");
        _env->prev = _env2;
    }
    // _env2->type = ENV_TEMPORARY;
    _env2->varname = _str;
    _env2->value = _value;
    _env2->next = _env;
    // _env2->prev = NULL;
    return _env2;
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

Env* _env3;

Value* true_value;
Env* _evalenv;

void eval(Value* node) {
// #define node evalarg->node
    EvalStack evalstack;

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
            goto eval_lambda;
        }
#endif
        arg1 = node->next->value;
        arg2list = node->next->next;

        if (_str == define_str) {
            #define ret _value
            eval(arg2list->value);
            _env = _evalenv;
            do {
                if (_env->varname == arg1->str){
                    _env->value = ret;
                    return;
                }
            } while(_env->next && (_env = _env->next));
            _env3 = _env;

            // Append to the global environment
            _str = arg1->str;
            _value = ret;
            _env = NULL;
            _env3->next = newEnv();
            return;
            #undef ret
        }
        if (_str == if_str) {
            #define condition _value
            eval(arg1);
            eval(condition ? arg2list->value : arg2list->next->value);
            return;
            #undef condition
        }
        if (_str == car_str) {
            eval(arg1);
            if (_value) {
                _value = _value->value;
            }
            return;
        }
        if (_str == cdr_str) {
            eval(arg1);
            if (_value) {
                _value = _value->next;
                // _value = _list ? newListNode() : NULL;
            }
            return;
        }
        if (_str == cons_str) {
            #define car node
            eval(arg1);
            car = _value;
            eval(arg2list->value);
            _value = newList(car, _value);
            // _value = newListNode();
            return;
            #undef car
        }
        if (_str == print_str) {
            eval(arg1);
            node = _value;
            printValue();
            if(arg2list) {
                putchar('\n');
            }
            _value = node;
            return;
        }
        if (_str == progn_str) {
            #define curlist _list_eval
            curlist = node->next;
            _value = NULL;
            while (curlist) {
                eval(curlist->value);
                curlist = curlist->next;
            }
            return;
            #undef curlist
        }
        if (_str == while_str) {
            _value = NULL;
            while (eval(arg1), _value) {
                eval(arg2list->value);
            }
            return;
        }
        if (_str == lambda_str || _str == macro_str || _str == closure_str) {
            malloc_bytes = sizeof(Lambda);
            _lambda = malloc_k();
            debug("lambda 1\n");
            _lambda->argnames = arg1;
            _lambda->body = arg2list->value;
            _lambda->env = _evalenv;
            _lambda->type = headstr[0] == 'l' ? L_LAMBDA : headstr[0] == 'm' ? L_MACRO : L_CLOSURE;

            malloc_bytes = sizeof(Value);
            _value = malloc_k();
            debug("lambda 2\n");
            _value->type = LAMBDA;
            _value->lambda = _lambda;
            return;
        }
        if (_str == eq_str) {
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
        }
        if (_str == plus_str || _str == minus_str || _str == ast_str || _str == slash_str || _str == mod_str) {
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
        }
        if (_str == lt_str || _str == gt_str) {
            c_eval = headstr[0];
            #define ret _value
            _value = arg2list->value;
            evalAsInt();
            n_ = i;
            eval(arg1);
            if (c_eval == '<') {
                _value = ret->n < n_ ? true_value : NULL;
            } else {
                _value = ret->n > n_ ? true_value : NULL;
            }
            return;
            #undef ret
        }
        if (_str == quote_str) {
            _value = arg1;
            return;
        }
        if (_str == atom_str) {
            eval(arg1);
            _value = !_value || (_value->type == ATOM) || (_value->type == INT) ? true_value : NULL;
            return;
        }
        if (_str == eval_str) {
            eval(arg1);
            eval(_value);
            return;
        }
        #undef headstr
    }

eval_lambda:;
    #define curargname _list_eval
    #define curarg _list_eval_2
    #define curenv evalstack_env
    #define curlambda ((Lambda*) node) 
    // If the head of the list is a list or an atom not any of the above,
    // it is expected for it to evaluate to a lambda.
    eval(node->value);
    curarg = node->next;
    node = (Value*)(_value->lambda);
    curargname = curlambda->argnames;


    // Macros should be evaluated in the environment they are called in,
    // instead of the environment they were defined in
    curenv = (curlambda->type == L_MACRO) ? _evalenv : curlambda->env;

    while (curargname) {
        if (curlambda->type == L_MACRO) {
            _value = curarg->value;
        } else {
            eval(curarg->value);
        }
        _str = curargname->value->str;
        _env = curenv;
        if (curlambda->type == L_CLOSURE) {
            curenv = newEnv();
        } else {
            curenv = prependTemporaryEnv();
        }
        curargname = curargname->next;
        curarg = curarg->next;
    }

    // For macros, evaluate the result before returning it
    evalstack_env2 = _evalenv;
    _evalenv = curenv;
    eval(curlambda->body);
    if (curlambda->type == L_MACRO) {
        _evalenv = curenv;
        eval(_value);
    }
    _evalenv = evalstack_env2;
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
    if (!_value || !(_value->value)) {
        debug("<nil>");
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
    } else if (k == LAMBDA) {
        debug("<lambda>");
        _str = v->lambda->type == L_LAMBDA ? "#<Lambda>" : v->lambda->type == L_MACRO ? "#<Macro>" : "#<Closure>";
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

Value* initlist;
Value* curlist;
Value* parsed;
int main (void) {
    // (Value*)LIST, since ->type and ->next are inside the same union
    nil = newList(NULL, (Value*)LIST);
    // TODO: get this value from the string table
    _str = t_str;
    newAtomNode();
    true_value = _value;

#ifndef ELVM
    char* opstr_list[num_ops] = {eval_str, closure_str, atom_str, quote_str, macro_str, define_str, while_str, progn_str, lambda_str, gt_str, lt_str, plus_str, minus_str, ast_str, slash_str, t_str, mod_str, print_str, cons_str, cdr_str, car_str, eq_str, if_str};
    for(i=0; i<num_ops; i++){
        _str = opstr_list[i];
        appendStringTable();
    }
#else
    s1 = eval_str;
    for(i=0; i<num_ops; i++){
        _str = s1;
        appendStringTable();
        for(; *s1; s1++){}
        s1++;
    }
#endif

    _str = "";
    _value = NULL;
    _env = NULL;
    _evalenv = newEnv();
    // TODO: get progn_str from the string table
    _str = (char*) progn_str;
    newAtomNode();
    initlist = newList(_value, NULL);
    curlist = initlist;
    while((parseExpr(), _value)) {
        curlist->next = newList(_value, NULL);
        curlist = curlist->next;
    }
#ifdef ELVM
    *((char*)(QFTASM_RAMSTDIN_BUF_STARTPOSITION)) = 0;
#endif
    // _list = initlist;
    // _value = newListNode();
    // _value = initlist;
    // printValue();
    // eval(_value);
    eval(initlist);
}
