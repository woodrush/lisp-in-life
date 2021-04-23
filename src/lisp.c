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


Value* _value;
Value* newAtomNode(char* str) {
#define ret _value
    ret = (Value*)malloc(sizeof(Value));
    debug("malloc at newAtomNode\n");
    ret->type = ATOM;
    ret->str = str;
    return ret;
#undef ret
}

Value* newListNode(List* list) {
#define ret _value
    ret = (Value*)malloc(sizeof(Value));
    debug("malloc at newListNode\n");
    ret->type = LIST;
    ret->list = list;
    return ret;
#undef ret
}

List* _list;
List* newList(Value* node, List* next) {
#define ret _list
    ret = (List*)malloc(sizeof(List));
    debug("malloc at newList\n");
    ret->value = node;
    ret->next = next;
    return ret;
#undef ret
}

Value* parseAtom();
Value* parseList();
Value* parseExpr();

char c;
char buf[32];
int i;
char* _str;
Value* parseAtom() {
    c = curchar();
    if (c == ')' || c == '(' || !c || c == EOF) {
        return NULL;
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

    _str = malloc(sizeof(char)*(i+1));
    debug("malloc at parseAtom\n");
    for (int j=0; j<i; j++) {
        _str[j] = buf[j];
    }

    return newAtomNode(_str);
}

List* parseListLoop() {
    Value* parsednode = parseExpr();
    return parsednode ? newList(parsednode, parseListLoop()) : NULL;
}

Value* parseList() {
    if (curchar() != '('){
        return NULL;
    }
    popchar(); // '('

    _list = parseListLoop();

    popchar(); // ')'
    return newListNode(_list);
}

Value* parseExpr() {
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
    _value = parseAtom();
    return _value ? _value : parseList();
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


Value* newIntValue(int n){
#define ret _value
    ret = malloc(sizeof(Value));
    debug("malloc at newIntValue\n");
    ret->type = INT;
    ret->n = n;
    return ret;
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

Env* newEnv(char* varname, Value* value, Env* next) {
    Env* env = malloc(sizeof(Env));
    debug("malloc at newEnv\n");
    env->varname = varname;
    env->value = value;
    env->next = next;
    return env;
}

Value* eqAtom(Value* n1, Value* n2) {
    // Integer equality
    if (n1->type == INT && n2->type == INT && n1->n == n2->n) {
        return n1;
    }
    // Atom equality
    if (n1->type == ATOM && n2->type == ATOM &&
        eqstr(n1->str, n2->str)) {
        return n1;
    }
    return NULL;
}

void printValue(Value* v);
Value* eval(Value* node, Env* env);

int parseInt(char* str) {
    int sign = 1;
    i = 0;
    if (str[0] == '-') {
        sign = 0;
        str++;
    }
    while (*str) {
        i = 10*i + (*str - '0');
        str++;
    }
    return sign ? i : -i;
}

int evalAsInt(Value* node, Env* env) {
    c = node->str[0];
    if (c == '-' || ('0' <= c && c <= '9')) {
        return parseInt(node->str);
    }
    return eval(node, env)->n;
}

Value* eval(Value* node, Env* env) {
    List* _list_eval;
    // Is an atom
    if (node->type == ATOM) {
        _str = node->str;
        c = node->str[0];
        if (('0' <= c && c <= '9') || c == '-') {
            return newIntValue(parseInt(_str));
        }
        // Get variable value from the environment
        do {
            if (eqstr(_str, env->varname)) {
                return env->value;
            }
        } while ((env = env->next));
        return NULL;
    }

    // Is an int or a lambda
    if (node->type == INT || node->type == LAMBDA) {
        return node;
    }

    // Is a list

    // Is ()
    if (!(node->list)) {
        return NULL;
    }

    // The head of the list is an atom
    if (node->list->value->type == ATOM) {
        char* headstr = node->list->value->str;
        if (!(node->list->next)) {
            goto eval_lambda;
        }
        Value* arg1 = node->list->next->value;
        List* arg2list = node->list->next->next;

        if (eqstr(headstr, "define")) {
            #define ret _value
            ret = eval(arg2list->value, env);
            do {
                if (eqstr(env->varname, arg1->str)){
                    env->value = ret;
                    return ret;
                }
            } while(env->next && (env = env->next));
            // Append to the global environment
            env->next = newEnv(arg1->str, ret, NULL);
            return ret;
            #undef ret
        }
        if (eqstr(headstr, "if")) {
            #define condition _value
            condition = eval(arg1, env);
            return (condition ?
            eval(arg2list->value, env) :
            eval(arg2list->next->value, env));
            #undef condition
        }
        if (eqstr(headstr, "quote")) {
            return arg1;
        }
        if (eqstr(headstr, "car") || eqstr(headstr, "cdr")) {
            _value = eval(arg1, env);
            if (!_value) {
                return NULL;
            }
            if (headstr[1] == 'a') {
                return _value->list->value;
            } else {
                _list = _value->list->next;
                return !_list ? NULL : newListNode(_list);
            }
        }
        if (eqstr(headstr, "cons")) {
            #define cdr node
            cdr = eval(arg2list->value, env);
            return newListNode(newList(eval(arg1, env), cdr ? cdr->list : NULL));
            #undef cdr
        }
        if (eqstr(headstr, "atom")) {
            _value = eval(arg1, env);
            return (
                !_value ? newAtomNode("t") :
                (_value->type == ATOM) || (_value->type == INT) ? _value : NULL
            );
        }
        if (eqstr(headstr, "print")) {
            _value = eval(arg1, env);
            printValue(_value);
            if(arg2list) {
                putchar('\n');
            }
            return _value;
        }
        if (eqstr(headstr, "progn")) {
            #define curlist _list_eval
            curlist = node->list->next;
            _value = NULL;
            while (curlist) {
                _value = eval(curlist->value, env);
                curlist = curlist->next;
            }
            return _value;
            #undef curlist
        }
        if (eqstr(headstr, "while")) {
            _value = NULL;
            while (eval(arg1, env)) {
                _value = eval(arg2list->value, env);
            }
            return _value;
        }
        if (eqstr(headstr, "lambda") || eqstr(headstr, "macro")) {
            _lambda = malloc(sizeof(Lambda));
            debug("malloc at lambda\n");
            _lambda->type = headstr[0] == 'l' ? L_LAMBDA : L_MACRO;
            _lambda->argnames = arg1->list;
            _lambda->body = arg2list->value;
            _lambda->env = env;

            _value = malloc(sizeof(Value));
            debug("malloc at newLambdaValue\n");
            _value->type = LAMBDA;
            _value->lambda = _lambda;
            return _value;
            // return newLambdaValue();
        }
        if (eqstr(headstr, "eval")) {
            return eval(eval(arg1, env), env);
        }
        if (eqstr(headstr, "eq")) {
            return eqAtom(eval(arg1, env), eval(arg2list->value, env));
        }
        if (eqstr(headstr, "+") || eqstr(headstr, "-") || eqstr(headstr, "*")
            || eqstr(headstr, "/") || eqstr(headstr, "mod")) {
            #define nextlist _list_eval
            nextlist = node->list->next;
            int ret = evalAsInt(nextlist->value, env);
            for (nextlist = nextlist->next; nextlist; nextlist = nextlist->next) {
                int nextint = evalAsInt(nextlist->value, env);
                ret = (
                    headstr[0] == '+' ? (ret + nextint) :
                    headstr[0] == '-' ? (ret - nextint) :
                    headstr[0] == '*' ? (ret * nextint) :
                    headstr[0] == '/' ? (ret / nextint) :
                    (ret%evalAsInt(nextlist->value, env))
                );
            }
            return newIntValue(ret);
            #undef nextlist
        }
        if (eqstr(headstr, "<") || eqstr(headstr, ">")) {
            #define ret _value
            int m = evalAsInt(arg2list->value, env);
            ret = eval(arg1, env);
            int n = ret->n;
            if (headstr[0] == '<') {
                return n < m ? ret : NULL;
            }
            return n > m ? ret : NULL;
            #undef ret
        }
    }

eval_lambda:;
    #define curargname _list_eval
    // If the head of the list is a list or an atom not any of the above,
    // it is expected for it to evaluate to a lambda.
    _lambda = eval(node->list->value, env)->lambda;
    curargname = _lambda->argnames;
    List* curarg = node->list->next;
    // Macros should be evaluated in the environment they are called in
    Env* curenv = (_lambda->type == L_LAMBDA) ? _lambda->env : env;

    while (curargname) {
        char* argname = curargname->value->str;
        // For macros, simply pass the arguments without evaluating them, as nodes
        curenv = newEnv(
            curargname->value->str,
            (_lambda->type == L_LAMBDA) ? eval(curarg->value, env) : curarg->value,
            curenv
        );
        curargname = curargname->next;
        curarg = curarg->next;
    }
    // For macros, evaluate the result before returning it
    _value = eval(_lambda->body, curenv);
    return (_lambda->type == L_LAMBDA) ? _value : eval(_value, curenv);
    #undef curargname
}

void printValue(Value* v) {
    if (!v) {
        printStr("()");
    } else if (v->type == INT) {
        printInt(v->n);
    } else if (v->type == LAMBDA) {
        printStr(v->lambda->type == L_LAMBDA ? "#<Closure>" : "#<Macro>");
    } else if (v->type == ATOM) {
        printStr(v->str);
    } else if (v->type == LIST){
        putchar('(');
        List* list = v->list;
        while(list) {
            printValue(list->value);
            list = list->next;
            if (list) {
                putchar(' ');
            }
        }
        putchar(')');
    }
}

#ifdef ELVM
char* init_stdin = QFTASM_RAMSTDIN_BUF_STARTPOSITION;
#endif
Env* globalEnv;
List* initlist;
List* curlist;
Value* parsed;
int main (void) {
    globalEnv = newEnv("", NULL, NULL);
    initlist = newList(newAtomNode("progn"), NULL);
    curlist = initlist;
    // Value* parsed;
    while((parsed = parseExpr())) {
        curlist->next = newList(parsed, NULL);
        curlist = curlist->next;
    }
    // printValue(newListNode(initlist));
#ifdef ELVM
    *init_stdin = 0;
    // for (;*init_stdin;init_stdin++) {
    //     *init_stdin = 0;
    // }
#endif
    eval(newListNode(initlist), globalEnv);
}
