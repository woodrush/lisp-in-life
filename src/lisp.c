#ifdef ELVM
#include "elvm.h"
#else
#include "lisp.h"
#endif

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
typedef enum {
    ATOM, LIST, INT, LAMBDA
} Valuetype;

typedef struct List {
    struct Value* value;
    struct List* next;
} List;

typedef struct Value {
    Valuetype type;
    union {
        char* str;
        struct List* list;
        int n;
        struct Lambda* lambda;
    };
} Value;

typedef struct StringTable {
    char* varname;
    struct StringTable* next;
} StringTable;


Value* _value;
#define str_in _str
Value* newAtomNode() {
#define ret _value
    malloc_bytes = sizeof(Value);
    ret = (Value*)malloc_k();
    ret->type = ATOM;
    ret->str = str_in;
    return ret;
#undef ret
}
#undef str_in

Value* nil;
List* _list;
#define list_in _list
Value* newListNode() {
#define ret _value
    malloc_bytes = sizeof(Value);
    ret = (Value*)malloc_k();
    ret->type = LIST;
    ret->list = list_in;
    return ret;
#undef ret
}
#undef list_in

List* newList(Value* node, List* next) {
#define ret _list
    malloc_bytes = sizeof(List);
    ret = (List*)malloc_k();
    ret->value = node;
    ret->next = next;
    return ret;
#undef ret
}

// void parseAtom();
// void parseList();
void parseExpr();

char c;
char buf[32];

StringTable* stringTableHead = NULL;
StringTable* _stringtable;

int stringtableaccess = 0;

void appendStringTable() {
    malloc_bytes = sizeof(StringTable);
    _stringtable = malloc_k();
    _stringtable->varname = _str;
    _stringtable->next = stringTableHead;
    stringTableHead = _stringtable;

    stringtableaccess++;
}

char* s1;
char* s2;

// void parseAtom() {
// }

List* parseListLoop() {
    parseExpr();
    Value* parsednode = _value;
    return parsednode ? newList(parsednode, parseListLoop()) : NULL;
}

// void parseList() {
//     if (curchar() != '('){
//         _value = NULL;
//         return;
//     }
//     popchar(); // '('

//     _list = parseListLoop();

//     popchar(); // ')'
//     _value = _list ? newListNode() : nil;
// }

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
        } while(c != '\n' && c != EOF);
        goto space;
    }

    if (c == '(') {
        goto parselist;
    }

    // Parse atom
#ifdef ELVM
    if (c == ')' || !c) {
#else
    if (c == ')' || !c || c == EOF) {
#endif
        _value = NULL;
        return;
    }

    i = 0;
#ifdef ELVM
    while (c != ' ' && c != '\n' && c != ')' && c != '(' && c != ';') {
#else
    while (c != ' ' && c != '\n' && c != ')' && c != '(' && c != ';' && c != EOF) {
#endif
        buf[i] = c;
        i++;
        popchar();
        c = curchar();
    }
    buf[i] = '\0';

    _stringtable = stringTableHead;

parseatomloop:;
    s1 = buf;
    s2 = _stringtable->varname;
    for(; *s1 || *s2; s1++, s2++) {
        if (*s1 != *s2) {
            // There is a string next to this string in the table
            if ((_stringtable = _stringtable->next)) {
                goto parseatomloop;
            }
            // This was the last string in the table
            goto newstr;
        }
    }
    // The strings are equal
    _str = _stringtable->varname;
    goto endatom;

newstr:;
    malloc_bytes = i+1;
    _str = malloc_k();
    s1 = _str;
    s2 = buf;
    // for (j=0; j<i; j++) {
    //     _str[j] = buf[j];
    // }
    for(; *s2; s1++, s2++) {
        *s1 = *s2;
    }
    appendStringTable();

endatom:;

    _value = newAtomNode();
    return;

parselist:
    // if (curchar() != '('){
    //     _value = NULL;
    //     return;
    // }
    popchar(); // '('

    _list = parseListLoop();

    popchar(); // ')'
    _value = _list ? newListNode() : nil;

    // if (!_value) {
    //     parseList();
    // }
}

//================================================================================
// Evaluator
//================================================================================
typedef struct Env {
    char* varname;
    struct Value* value;
    struct Env* next;
} Env;

typedef enum {
    L_LAMBDA, L_MACRO
} Lambdatype;

typedef struct Lambda {
    struct List* argnames;
    struct Value* body;
    struct Env* env;
    Lambdatype type;
} Lambda;


void newIntValue(){
#define ret _value
    malloc_bytes = sizeof(Value);
    ret = malloc_k();
    ret->type = INT;
    ret->n = i;
#undef ret
}

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
    env->varname = varname_in;
    env->value = value_in;
    env->next = env_in;
    return env;
}
#undef varname_in
#undef value_in
#undef env_in
#undef env

void printValue();

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

int isNumeric(){
    return c == '-' || ('0' <= c && c <= '9');
}

void eval(Value* node);
void evalAsInt(Value* node) {
    c = node->str[0];
    if (isNumeric()) {
        _str = node->str;
        parseInt();
    } else {
        eval(node);
        i = _value->n;
    }
}

typedef struct {
    union {
        char c_eval_;
        List* _list_eval_;
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

Env* _env3;

Value* true_value;
Env* _evalenv;

int printstr_times = 10;
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

    // Is an atom
    if (node->type == ATOM) {
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
    if (node->type == INT || node->type == LAMBDA) {
        _value = node;
        return;
    }

    // Is a list

    // Is ()
    if (!(node->list)) {
        _value = NULL;
        return;
    }

    // The head of the list is an atom
    if (node->list->value->type == ATOM) {
        #define headstr _str
        headstr = node->list->value->str;

#ifdef ELVM
        if ((int)last_op < (int)headstr) {
            goto eval_lambda;
        }
#endif
        // print_int(last_op);
        // if (printstr_times-- > 0) {
        //     print_int(headstr);
        // }

        // if (!(node->list->next)) {
        //     goto eval_lambda;
        // }
        arg1 = node->list->next->value;
        arg2list = node->list->next->next;

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
                _value = _value->list->value;
            }
            return;
        }
        if (_str == cdr_str) {
            eval(arg1);
            if (_value) {
                _list = _value->list->next;
                _value = _list ? newListNode() : NULL;
            }
            return;
        }
        if (_str == cons_str) {
            #define car node
            eval(arg1);
            car = _value;
            eval(arg2list->value);
            _list = newList(car, _value ? _value->list : NULL);
            _value = newListNode();
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
            curlist = node->list->next;
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
        if (_str == lambda_str || _str == macro_str) {
            malloc_bytes = sizeof(Lambda);
            _lambda = malloc_k();
            _lambda->argnames = arg1->list;
            _lambda->body = arg2list->value;
            _lambda->env = _evalenv;
            _lambda->type = headstr[0] == 'l' ? L_LAMBDA : L_MACRO;

            malloc_bytes = sizeof(Value);
            _value = malloc_k();
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
            nextlist = node->list->next;
            evalAsInt(nextlist->value);
            n_ = i;
            if (c_eval == '-' && !(nextlist->next)) {
                i = -n_;
            } else {
                for (nextlist = nextlist->next; nextlist; nextlist = nextlist->next) {
                    evalAsInt(nextlist->value);
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
            evalAsInt(arg2list->value);
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
    eval(node->list->value);
    curarg = node->list->next;
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
        curenv = newEnv();
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

#define v _value
void printValue() {
    List* list;

    k = v->type;
    if (k == INT) {
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
        _str = v->lambda->type == L_LAMBDA ? "#<Closure>" : "#<Macro>";
    } else if (k == ATOM) {
        _str = v->str;
    } else if (k == LIST){
        putchar('(');
        list = v->list;
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

List* initlist;
List* curlist;
Value* parsed;
int main (void) {
    _list = NULL;
    nil = newListNode();
    _str = t_str;
    true_value = newAtomNode();

#ifndef ELVM
    char* opstr_list[num_ops] = {define_str, if_str, quote_str, car_str, cdr_str, cons_str, atom_str, print_str, progn_str, while_str, lambda_str, macro_str, eval_str, eq_str, plus_str, minus_str, ast_str, slash_str, mod_str, gt_str, lt_str, t_str};
    for(i=0; i<num_ops; i++){
        _str = opstr_list[i];
        appendStringTable();
    }
#else
    s1 = 10;
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
    _str = (char*) progn_str;
    initlist = newList(newAtomNode(), NULL);
    curlist = initlist;
    while((parseExpr(), _value)) {
        curlist->next = newList(_value, NULL);
        curlist = curlist->next;
    }
    // printValue(newListNode(initlist));
#ifdef ELVM
    *((char*)QFTASM_RAMSTDIN_BUF_STARTPOSITION) = 0;
#endif
    _list = initlist;
    // _value = newListNode();
    // printValue();
    eval(newListNode());
}
