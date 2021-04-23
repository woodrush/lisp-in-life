#ifdef ELVM
#include "elvm.h"
#else
#include "lisp.h"
#endif


#define debug(x)

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


char* _str;
Value* _value;
#define str_in _str
Value* newAtomNode() {
#define ret _value
    ret = (Value*)malloc(sizeof(Value));
    debug("malloc at newAtomNode\n");
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
    ret = (Value*)malloc(sizeof(Value));
    debug("malloc at newListNode\n");
    ret->type = LIST;
    ret->list = list_in;
    return ret;
#undef ret
}
#undef list_in

List* newList(Value* node, List* next) {
#define ret _list
    ret = (List*)malloc(sizeof(List));
    debug("malloc at newList\n");
    ret->value = node;
    ret->next = next;
    return ret;
#undef ret
}

void parseAtom();
void parseList();
void parseExpr();

char c;
char buf[32];
int i;
int j;

StringTable* stringTableHead = NULL;

void appendStringTable() {
    StringTable* ret = malloc(sizeof(StringTable));
    ret->varname = _str;
    ret->next = stringTableHead;
    stringTableHead = ret;
}

void parseAtom() {
    c = curchar();
    if (c == ')' || c == '(' || !c || c == EOF) {
        // return NULL;
        _value = NULL;
        return;
    }

    // char buf[32];
    i = 0;
    while (c != ' ' && c != '\n' && c != ')' && c != '(' && c != ';' && c != EOF) {
        buf[i] = c;
        i++;
        popchar();
        c = curchar();
    }
    buf[i] = '\0';

    // _str = malloc(i+1);

    _str = buf;
    StringTable* st = stringTableHead;

    // for (; st; st = st->next) {
    //     // printf("%s", st->varname);
    // if (st) {
parseatomloop:;
        char* s1 = _str;
        char* s2 = st->varname;
        for(; *s1 || *s2; s1++, s2++) {
            if (*s1 != *s2) {
                if (st->next) {
                    st = st->next;
                    goto parseatomloop;
                }
                goto newstr;
            }
        }
        _str = st->varname;
        goto endatom;
        // return 1;


        // if (eqstr(st->varname)) {
        //     _str = st->varname;
        //     // printf("string matches! %s\n", st->varname);
        //     goto endatom;
        // }
    // }

newstr:;
    _str = malloc(i+1);
    for (j=0; j<i; j++) {
        _str[j] = buf[j];
    }
    appendStringTable();
    // printf("New string: %s\n", _str);

endatom:;
    _value = newAtomNode();
    // return newAtomNode();
}

List* parseListLoop() {
    parseExpr();
    Value* parsednode = _value;
    return parsednode ? newList(parsednode, parseListLoop()) : NULL;
}

void parseList() {
    if (curchar() != '('){
        _value = NULL;
        return;
    }
    popchar(); // '('

    _list = parseListLoop();

    popchar(); // ')'
    _value = _list ? newListNode() : nil;
    // _value = newListNode();
    // return newListNode();
}

void parseExpr() {
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
    // _value = parseAtom();
    parseAtom();
    if (!_value) {
        parseList();
    }
    // return _value;
    // return _value ? _value : parseList();
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


// Value* newIntValue(int n){
void newIntValue(){
#define ret _value
    ret = malloc(sizeof(Value));
    debug("malloc at newIntValue\n");
    ret->type = INT;
    ret->n = i;
    // return ret;
#undef ret
}

Lambda* _lambda;
// Value* newLambdaValue(){
//     _value = malloc(sizeof(Value));
//     debug("malloc at newLambdaValue\n");
//     _value->type = LAMBDA;
//     _value->lambda = _lambda;
//     return _value;
// }

// Value* getVariableValue(char* varname, Env* env) {
//     do {
//         if (eqstr(varname, env->varname)) {
//             return env->value;
//         }
//     } while ((env = env->next));
//     return NULL;
// }

Env* _env;
Env* _env2;
#define varname_in _str
#define value_in _value
#define env_in _env
#define env _env2
Env* newEnv() {
    env = malloc(sizeof(Env));
    debug("malloc at newEnv\n");
    env->varname = varname_in;
    env->value = value_in;
    env->next = env_in;
    return env;
}
#undef varname_in
#undef value_in
#undef env_in
#undef env

// Value* eqAtom(Value* n1, Value* n2) {
//     // Integer equality
//     if (n1->type == INT && n2->type == INT && n1->n == n2->n) {
//         return n1;
//     }
//     // Atom equality
//     if (n1->type == ATOM && n2->type == ATOM &&
//         eqstr(n1->str, n2->str)) {
//         return n1;
//     }
//     return NULL;
// }

#define v _value
void printValue() {
    i = v->type;
    if (i == INT) {
        printInt(v->n);
    } else if (i == LAMBDA) {
        printStr(v->lambda->type == L_LAMBDA ? "#<Closure>" : "#<Macro>");
    } else if (i == ATOM) {
        printStr(v->str);
    } else if (i == LIST){
        putchar('(');
        List* list = v->list;
        // if (!list) {
        //     printStr("Empty list!!!\n");
        // }
        while(list) {
            _value = list->value;
            printValue();
            list = list->next;
            if (list) {
                putchar(' ');
            }
        }
        putchar(')');
    }
}
#undef v


#define str _str
#define sign j
int k;
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
        // i = 10*i + (*str - '0');
        str++;
    }
    i = sign ? i : -i;
}
#undef str
#undef sign

void eval(Value* node);
void evalAsInt(Value* node) {
    c = node->str[0];
    if (c == '-' || ('0' <= c && c <= '9')) {
        _str = node->str;
        // i = parseInt();
        parseInt();
    } else {
        eval(node);
        i = _value->n;
    }
    // return _value->n;
}

Valuetype eval_rettype;
Env* _evalenv;
Env* _env3;

typedef struct {
    Value* arg1_;
    union {
        char c_eval_;
        List* _list_eval_;
    };
    union {
        List* arg2list_;
        int n_;
    };
} EvalStack;

// const char* define_str = "define";
// const char* progn_str = "progn";

void eval(Value* node) {
    EvalStack evalstack;


#define _list_eval (evalstack._list_eval_)
#define arg1 (evalstack.arg1_)
#define arg2list (evalstack.arg2list_)
#define c_eval (evalstack.c_eval_)
// #define m_ (evalstack.m_)
#define n_ (evalstack.n_)

    // List* _list_eval;
    // Value* arg1;
    // List* arg2list;
    // char c_eval;
    // int m;
    // int n;

    // Is an atom
    if (node->type == ATOM) {
        _str = node->str;
        c = node->str[0];
        if (('0' <= c && c <= '9') || c == '-') {
            // i = parseInt();
            parseInt();
            newIntValue();
            // return ret;
            return;
        }
        _env = _evalenv;
        // Get variable value from the environment
        do {
            if (
                _str == _env->varname
                // eqstr(_env->varname)
                ) {
                // return env->value;
                _value = _env->value;
                return;
            }
        } while ((_env = _env->next));
        // return NULL;
        _value = NULL;
        return;
    }

    // Is an int or a lambda
    if (node->type == INT || node->type == LAMBDA) {
        // return node;
        _value = node;
        return;
    }

    // Is a list

    // Is ()
    if (!(node->list)) {
        // return NULL;
        _value = NULL;
        return;
    }

    // The head of the list is an atom
    if (node->list->value->type == ATOM) {
        #define headstr _str
        headstr = node->list->value->str;
        if (!(node->list->next)) {
            goto eval_lambda;
        }
        arg1 = node->list->next->value;
        arg2list = node->list->next->next;

        if (_str == define_str) {
            #define ret _value
            // ret = eval(arg2list->value, env);
            eval(arg2list->value);
            _env = _evalenv;
            do {
                if (
                    _env->varname == arg1->str
                    // _str = _env->varname, eqstr(arg1->str)
                    ){
                    _env->value = ret;
                    // _value = ret;
                    return;
                    // return ret;
                }
            } while(_env->next && (_env = _env->next));
            _env3 = _env;

            // Append to the global environment
            _str = arg1->str;
            _value = ret;
            _env = NULL;
            _env3->next = newEnv();
            // return ret;
            return;
            #undef ret
        }
        if (_str == if_str) {
            #define condition _value
            // condition = eval(arg1, env);
            eval(arg1);
            eval(condition ? arg2list->value : arg2list->next->value);
            return;
            // return (condition ?
            // eval(arg2list->value, env) :
            // eval(arg2list->next->value, env));
            #undef condition
        }
        if (_str == quote_str) {
            _value = arg1;
            return;
            // return arg1;
        }
        if (_str == car_str) {
            // _value = eval(arg1, env);
            eval(arg1);
            // if (!_value) {
            //     return NULL;
            // }
            if (_value) {
                _value = _value->list->value;
            }
            return;
        }
        if (_str == cdr_str) {
            // _value = eval(arg1, env);
            eval(arg1);
            // if (!_value) {
            //     return NULL;
            // }
            if (_value) {
                _list = _value->list->next;
                // return !_list ? NULL : newListNode();
                _value = _list ? newListNode() : NULL;
            }
            return;
        }
        if (_str == cons_str) {
            #define car node
            // cdr = eval(arg2list->value, env);
            eval(arg1);
            car = _value;
            eval(arg2list->value);
            // cdr = _value;
            _list = newList(car, _value ? _value->list : NULL);
            // return newListNode();
            _value = newListNode();
            return;
            #undef car
        }
        if (_str == atom_str) {
            // _value = eval(arg1, env);
            eval(arg1);
            _str = "t";
            // return (
            //     !_value ? newAtomNode() :
            //     (_value->type == ATOM) || (_value->type == INT) ? _value : NULL
            // );
            _value = (
                !_value ? newAtomNode() :
                (_value->type == ATOM) || (_value->type == INT) ? _value : NULL
            );
            return;
        }
        if (_str == print_str) {
            // _value = eval(arg1, env);
            eval(arg1);
            node = _value;
            printValue();
            if(arg2list) {
                putchar('\n');
            }
            _value = node;
            // return _value;
            return;
        }
        if (_str == progn_str) {
            #define curlist _list_eval
            curlist = node->list->next;
            _value = NULL;
            while (curlist) {
                // _value = eval(curlist->value, env);
                eval(curlist->value);
                curlist = curlist->next;
            }
            // return _value;
            return;
            #undef curlist
        }
        if (_str == while_str) {
            _value = NULL;
            while (eval(arg1), _value) {
                // _value = eval(arg2list->value, env);
                eval(arg2list->value);
            }
            // return _value;
            return;
        }
        if (_str == lambda_str || _str == macro_str) {
            _lambda = malloc(sizeof(Lambda));
            debug("malloc at lambda\n");
            _lambda->argnames = arg1->list;
            _lambda->body = arg2list->value;
            _lambda->env = _evalenv;
            _lambda->type = headstr[0] == 'l' ? L_LAMBDA : L_MACRO;

            _value = malloc(sizeof(Value));
            debug("malloc at newLambdaValue\n");
            _value->type = LAMBDA;
            _value->lambda = _lambda;
            // return _value;
            return;
            // return newLambdaValue();
        }
        if (_str == eval_str) {
            eval(arg1);
            eval(_value);
            return;
            // return eval(_value, env);
        }
        if (_str == eq_str) {
            // node = eval(arg1, env);
            eval(arg1);
            node = _value;
            eval(arg2list->value);
            #define n1 node
            #define n2 _value
            // Integer equality
            if (n1->type == INT && n2->type == INT && n1->n == n2->n) {
                // return n1;
                _value = n1;
            }
            // Atom equality
            else if (n1->type == ATOM && n2->type == ATOM &&
                // (_str = n1->str, eqstr(n2->str))
                (n1->str == n2->str)
                ) {
                // return n1;
                _value = n1;
            } else {
                _value = NULL;
            }
            // return NULL;
            return;
            #undef n1
            #undef n2
            // return eqAtom(eval(arg1, env), eval(arg2list->value, env));
        }
        if (_str == plus_str || _str ==minus_str || _str == ast_str || _str == slash_str || _str == mod_str
        //  eqstr("+") || eqstr("-") || eqstr("*")
        //     || eqstr("/") || eqstr("mod")
            ) {
            c_eval = headstr[0];
        // c = headstr[0];
        // if (((c == '+' || c == '-' || c == '*' || c == '/') && headstr[1] == '\0') || eqstr("mod")) {

            // #define nextlist _list_eval
            // nextlist = node->list->next;
            evalAsInt(node->list->next->value);
            n_ = i;
            // for (nextlist = nextlist->next; nextlist; nextlist = nextlist->next) {
                evalAsInt(node->list->next->next->value);
                // int nextint = i;
                n_ = (
                    c_eval == '+' ? (n_ + i) :
                    c_eval == '-' ? (n_ - i) :
                    c_eval == '*' ? (n_ * i) :
                    c_eval == '/' ? (n_ / i) :
                    (n_%i)
                );
            // }
            // #define ret_ _value
            i = n_;
            newIntValue();
            // return ret_;
            return;
            // #undef ret_
            // #undef nextlist
        }
        if (_str == lt_str || _str == gt_str
            // eqstr("<") || eqstr(">")
            ) {
            c_eval = headstr[0];
            #define ret _value
            evalAsInt(arg2list->value);
            n_ = i;
            // ret = eval(arg1, env);
            eval(arg1);
            // n_ = ret->n;
            if (c_eval == '<') {
                _value = ret->n < n_ ? ret : NULL;
                // return n < m ? ret : NULL;
            } else {
                _value = ret->n > n_ ? ret : NULL;
            }
            // return n > m ? ret : NULL;
            return;
            #undef ret
        }
        #undef headstr
    }

eval_lambda:;
    #define curargname _list_eval
    #define isMacro node
    // If the head of the list is a list or an atom not any of the above,
    // it is expected for it to evaluate to a lambda.
    // Lambda* lambda = eval(node->list->value, env)->lambda;
    eval(node->list->value);
    Lambda* lambda = _value->lambda;
    // _lambda = eval(node->list->value, env)->lambda;
    curargname = lambda->argnames;
    List* curarg = node->list->next;
    // Macros should be evaluated in the environment they are called in
    isMacro = (Value*) (lambda->type == L_MACRO);
    Env* curenv = ((int)isMacro) ? _evalenv : lambda->env;

    while (curargname) {
        // char* argname = curargname->value->str;
        // For macros, simply pass the arguments without evaluating them, as nodes
        // _value = ((int)isMacro) ? eval(curarg->value, env) : curarg->value;
        if ((int)isMacro) {
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
    // _value = eval(lambda->body, curenv);
    Env* e = _evalenv;
    _evalenv = curenv;
    eval(lambda->body);
    // return ((int)isMacro) ? _value : eval(_value, curenv);
    // _value = ((int)isMacro) ? _value : eval(_value, curenv);
    if ((int)isMacro) {
        _evalenv = curenv;
        eval(_value);
    }
    _evalenv = e;
    #undef curargname
    #undef isMacro
}
#undef _list_eval
#undef arg1
#undef arg2list
#undef c_eval
#undef m
#undef n


#ifdef ELVM
char* init_stdin = QFTASM_RAMSTDIN_BUF_STARTPOSITION;
#endif
// Env* globalEnv;
List* initlist;
List* curlist;
Value* parsed;
int main (void) {
    // _str = "hey,i'm_nil";
    // _list = newList(newAtomNode(), NULL);
    _list = NULL;
    nil = newListNode();

#ifndef ELVM
    char* opstr_list_[num_ops] = {define_str, if_str, quote_str, car_str, cdr_str, cons_str, atom_str, print_str, progn_str, while_str, lambda_str, macro_str, eval_str, eq_str, plus_str, minus_str, ast_str, slash_str, mod_str, gt_str, lt_str, t_str};
    for(i=0;i<num_ops;i++){
        opstr_list[i] = opstr_list_[i];
    }    
#endif

    for(i=0;i<num_ops;i++){
        _str = opstr_list[i];
        appendStringTable();
        // printStr("aa");
        // printStr(_str);
    }

    // _str = (char*) progn_str;
    // appendStringTable();
    // _str = (char*) define_str;
    // appendStringTable();

    _str = "";
    _value = NULL;
    _env = NULL;
    _evalenv = newEnv();
    _str = (char*) progn_str;
    initlist = newList(newAtomNode(), NULL);
    curlist = initlist;
    // Value* parsed;
    while((parseExpr(), _value)) {
        curlist->next = newList(_value, NULL);
        curlist = curlist->next;
    }
    // printValue(newListNode(initlist));
#ifdef ELVM
    *init_stdin = 0;
    // for (;*init_stdin;init_stdin++) {
    //     *init_stdin = 0;
    // }
#endif
    // nil->list->value->str = "hi!";
    _list = initlist;
    // _value = newListNode();
    // printValue();
    eval(newListNode());
}
