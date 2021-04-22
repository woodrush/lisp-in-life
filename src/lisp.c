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


Value* node;
Value* newAtomNode(char* str) {
    node = (Value*)malloc(sizeof(Value));
    debug("malloc at newAtomNode\n");
    node->type = ATOM;
    node->str = str;
    return node;
}

Value* newListNode(List* list) {
    node = (Value*)malloc(sizeof(Value));
    debug("malloc at newListNode\n");
    node->type = LIST;
    node->list = list;
    return node;
}

List* list;
List* newList(Value* node, List* next) {
    list = (List*)malloc(sizeof(List));
    debug("malloc at newList\n");
    list->value = node;
    list->next = next;
    return list;
}

Value* parseAtom();
Value* parseList();
Value* parseExpr();

char c;
char buf[32];
int i;
char* str;
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

    str = malloc(sizeof(char)*(i+1));
    debug("malloc at parseAtom\n");
    for (int j=0; j<i; j++) {
        str[j] = buf[j];
    }

    return newAtomNode(str);
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

    list = parseListLoop();

    popchar(); // ')'
    return newListNode(list);
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
    node = parseAtom();
    return node ? node : parseList();
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
    node = malloc(sizeof(Value));
    debug("malloc at newIntValue\n");
    node->type = INT;
    node->n = n;
    return node;
}

Value* newLambdaValue(Lambda* l){
    node = malloc(sizeof(Value));
    debug("malloc at newLambdaValue\n");
    node->type = LAMBDA;
    node->lambda = l;
    return node;
}

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
    int ret = 0;
    if (str[0] == '-') {
        sign = 0;
        str++;
    }
    while (*str) {
        ret = 10*ret + (*str - '0');
        str++;
    }
    return sign ? ret : -ret;
}

int evalAsInt(Value* node, Env* env) {
    if (node->str[0] == '-' || ('0' <= node->str[0] && node->str[0] <= '9')) {
        return parseInt(node->str);
    }
    return eval(node, env)->n;
}

Value* eval(Value* node, Env* env) {
    // Is an atom
    if (node->type == ATOM) {
        if (('0' <= node->str[0] && node->str[0] <= '9') || node->str[0] == '-') {
            return newIntValue(parseInt(node->str));
        }
        // Get variable value from the environment
        do {
            if (eqstr(node->str, env->varname)) {
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
        if (eqstr(headstr, "define")) {
            Value* value = eval(node->list->next->next->value, env);
            do {
                if (eqstr(env->varname, node->list->next->value->str)){
                    env->value = value;
                    return value;
                }
            } while(env->next && (env = env->next));
            // Append to the global environment
            env->next = newEnv(node->list->next->value->str, value, NULL);
            return value;
        }
        if (eqstr(headstr, "if")) {
            Value* condition = eval(node->list->next->value, env);
            return (condition ?
            eval(node->list->next->next->value, env) :
            eval(node->list->next->next->next->value, env));
        }
        if (eqstr(headstr, "quote")) {
            return node->list->next->value;
        }
        if (eqstr(headstr, "car") || eqstr(headstr, "cdr")) {
            Value* value = eval(node->list->next->value, env);
            if (!value) {
                return NULL;
            }
            if (headstr[1] == 'a') {
                return value->list->value;
            } else {
                List* nextlist = value->list->next;
                return !nextlist ? NULL : newListNode(nextlist);
            }
        }
        if (eqstr(headstr, "cons")) {
            Value* cdr = eval(node->list->next->next->value, env);
            return newListNode(newList(eval(node->list->next->value, env), cdr ? cdr->list : NULL));
        }
        if (eqstr(headstr, "atom")) {
            Value* ret = eval(node->list->next->value, env);
            return (
                !ret ? newAtomNode("t") :
                (ret->type == ATOM) || (ret->type == INT) ? ret :
                NULL
            );
        }
        if (eqstr(headstr, "print")) {
            Value* value = eval(node->list->next->value, env);
            printValue(value);
            if(node->list->next->next) {
                putchar('\n');
            }
            return value;
        }
        if (eqstr(headstr, "progn")) {
            List* curlist = node->list->next;
            Value* value = NULL;
            while (curlist) {
                value = eval(curlist->value, env);
                curlist = curlist->next;
            }
            return value;
        }
        if (eqstr(headstr, "while")) {
            Value* value = NULL;
            while (eval(node->list->next->value, env)) {
                value = eval(node->list->next->next->value, env);
            }
            return value;
        }
        if (eqstr(headstr, "lambda") || eqstr(headstr, "macro")) {
            Lambda* lambda = malloc(sizeof(Lambda));
            debug("malloc at lambda\n");
            lambda->type = headstr[0] == 'l' ? L_LAMBDA : L_MACRO;
            lambda->argnames = node->list->next->value->list;
            lambda->body = node->list->next->next->value;
            lambda->env = env;
            return newLambdaValue(lambda);
        }
        if (eqstr(headstr, "eval")) {
            return eval(eval(node->list->next->value, env), env);
        }
        if (eqstr(headstr, "eq")) {
            return eqAtom(eval(node->list->next->value, env), eval(node->list->next->next->value, env));
        }
        if (eqstr(headstr, "+") || eqstr(headstr, "-") || eqstr(headstr, "*")
            || eqstr(headstr, "/") || eqstr(headstr, "mod")) {
            List* next = node->list->next;
            int ret = evalAsInt(next->value, env);
            for (next = next->next; next; next = next->next) {
                int nextint = evalAsInt(next->value, env);
                ret = (
                    headstr[0] == '+' ? (ret + nextint) :
                    headstr[0] == '-' ? (ret - nextint) :
                    headstr[0] == '*' ? (ret * nextint) :
                    headstr[0] == '/' ? (ret / nextint) :
                    (ret%evalAsInt(next->value, env))
                );
            }
            return newIntValue(ret);
        }
        if (eqstr(headstr, "<") || eqstr(headstr, ">")) {
            Value* v = eval(node->list->next->value, env);
            int n = v->n;
            int m = evalAsInt(node->list->next->next->value, env);
            if (headstr[0] == '<') {
                return n < m ? v : NULL;
            }
            return n > m ? v : NULL;
        }
    }

    // If the head of the list is a list or an atom not any of the above,
    // it is expected for it to evaluate to a lambda.
    Lambda* lambda = eval(node->list->value, env)->lambda;
    List* curargname = lambda->argnames;
    List* curarg = node->list->next;
    // Macros should be evaluated in the environment they are called in
    Env* curenv = (lambda->type == L_LAMBDA) ? lambda->env : env;

    while (curargname) {
        char* argname = curargname->value->str;
        // For macros, simply pass the arguments without evaluating them, as nodes
        curenv = newEnv(
            curargname->value->str,
            (lambda->type == L_LAMBDA) ? eval(curarg->value, env) : curarg->value,
            curenv
        );
        curargname = curargname->next;
        curarg = curarg->next;
    }
    // For macros, evaluate the result before returning it
    Value* ret = eval(lambda->body, curenv);
    return (lambda->type == L_LAMBDA) ? ret : eval(ret, curenv);
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
#ifdef ELVM
    for (;*init_stdin;init_stdin++) {
        *init_stdin = 0;
    }
#endif
    // printValue(newListNode(initlist));
    eval(newListNode(initlist), globalEnv);
}
