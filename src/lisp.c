#ifdef QFT
    #define DEFLOCATION extern
    #define BITSIZE 16
#else
    #define DEFLOCATION
    #define BITSIZE 64
#endif

DEFLOCATION char* _str;
DEFLOCATION int q;
DEFLOCATION int r;
DEFLOCATION unsigned long long i;
DEFLOCATION int j;
DEFLOCATION unsigned long long k;
DEFLOCATION int _malloc_bytes;
DEFLOCATION void* _malloc_result;


#ifdef QFT
    #include "qft.h"
    #define debug(x)
    #define debug1(x,y)
    #define debug1_2(x,y)
    #define debug2(x,y,z)
    #define debug_malloc(x)
#else
    #include "lisp.h"
    #define debug(x)      // printf(x)
    #define debug1(x,y)   // printf(x,y)
    #define debug1_2(x,y) // printf(x,y)
    #define debug2(x,y,z) // printf(x,y,z)
    #define debug_malloc(x) // printf(x)
#endif


typedef void* Value;

typedef struct List {
    struct List* next;
    Value value;
} List;


typedef struct StringTable {
    char* varname;
    struct StringTable* lesser;
    struct StringTable* greater;
} StringTable;

typedef struct Env {
    char* varname;
    struct Env* prev;
    Value value;
    struct Env* next;
} Env;

typedef struct Lambda {
    struct List* definition;
    struct Env* env;
} Lambda;


DEFLOCATION char c;

DEFLOCATION char* s1;
DEFLOCATION char* s2;
DEFLOCATION char* s3;


typedef enum {
    ATOM   = (unsigned long long)1<<(BITSIZE-2),
    INT    = (unsigned long long)2<<(BITSIZE-2),
    LAMBDA = (unsigned long long)3<<(BITSIZE-2),
} Valuetype;


#ifdef QFT
    int stdin_startpos = QFTASM_RAMSTDIN_BUF_STARTPOSITION;

    #define typemaskinv (0b0011111111111111)
    // #define buf ((char*)65352)
    // #define stringTableHeadList ((StringTable**)65336)

    // TODO: hand-fold these to constants
    #define stack_head (QFTASM_RAM_SIZE - QFTASM_STACK_SIZE)
    #define buf ((char*)stack_head)
    #define stringTableHeadList ((StringTable**)(stack_head+32))

    int* _edata_stack_ptr = stack_head + 48;

    // #define _edata_stack ((int*)65385)

    // #define _edata_stack (*((int*)stack_head+48))

    #define _edata_stack (*_edata_stack_ptr)

    // DEFLOCATION int* _edata_stack;
    DEFLOCATION List initlist;


    Value true_value = t_str ^ ATOM;
    Env initialenv = {
        .varname = t_str,
        .prev = (Env*)1,
        .value = t_str ^ ATOM,
        .next = NULL,
    };
    DEFLOCATION Env* _evalenv;
    DEFLOCATION List* curlist;

#else
    #define typemaskinv (~LAMBDA)
    #define valuemask_14 (((unsigned long long)1<<14)-1)
    char buf[32];
    List initlist = { .next = NULL, .value = NULL };
    StringTable* stringTableHeadList[16];

    Value true_value;
    Env initialenv = {
        .varname = t_str,
        .prev = (Env*)1,
        .value = NULL,
        .next = NULL,
    };
    DEFLOCATION Env* _evalenv = &initialenv;
    DEFLOCATION List* curlist = &initlist;

#endif

#define typemask LAMBDA

#define isIntValue(x) ((((unsigned long long)(x)) &~ typemaskinv) == INT)
#define isAtomValue(x) ((((unsigned long long)(x)) &~ typemaskinv) == ATOM)
#define isLambdaValue(x) ((((unsigned long long)(x)) &~ typemaskinv) == LAMBDA)

typedef enum {
    L_LAMBDA     = LAMBDA,
    L_MACRO      = INT,
    L_TEMPLAMBDA = ATOM,
    L_TEMPMACRO  = 0,
} Lambdatype;

#define lambdaType(lambda) (((unsigned long long)((lambda)->env)) &~ typemaskinv)
#define lambdaEnv(lambda) ((Env*)(((unsigned long long)((lambda)->env)) &~ typemask))




DEFLOCATION StringTable* _stringtable;

DEFLOCATION Lambda* _lambda;

DEFLOCATION Env* _env;
DEFLOCATION Env* _env2;
DEFLOCATION Env* _env3;



DEFLOCATION Value _value;
DEFLOCATION List* _list;

DEFLOCATION int sthash;

DEFLOCATION StringTable* stringtable;
DEFLOCATION StringTable** branch;


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

#define str2Atom(__str) {                                  \
    debug("str2Atom\n");                                   \
    _value = (Value) (((unsigned long long)__str) ^ ATOM); \
}

#define atom2Str(__value) {                                       \
    debug("atom2Str\n");                                          \
    _str = (char*) (((unsigned long long)__value) & (~typemask)); \
}

#define newLambdaData(__target, __definition, __env, __type) {   \
    malloc_k(sizeof(Lambda), __target);                          \
    debug_malloc("lambda 1\n");                                  \
    _lambda->definition = __definition;                          \
    _lambda->env = (Env*)(((unsigned long long)__env) ^ __type); \
}

#define lambda2Value(__lambda) {                                 \
    debug("lambda2Value\n");                                     \
    _value = (Value) (((unsigned long long)__lambda) ^ LAMBDA);  \
}

#define value2Lambda(__value, __outvalue) {                              \
    debug("value2Lambda\n");                                             \
    __outvalue = (Value) (((unsigned long long)__value) & (~typemask));  \
}

List* newList(Value node, List* next) {
    // _malloc_bytes = sizeof(Value);
    malloc_k(sizeof(List), _list);
    // ret = (Value) _malloc_result;
    debug_malloc("newList\n");
    _list->value = node;
    _list->next = next;
    return _list;
}

#define newIntValue() {                                             \
    debug("newIntValue\n");                                         \
    _value = (Value) (((unsigned long long)(i &~ typemask)) ^ INT); \
}

#define newStringTable_(__stringtable, __str) {                      \
    malloc_k_pos(sizeof(StringTable), __stringtable, _edata_stack);  \
    debug_malloc("newStringTable_\n");                               \
    __stringtable->varname = __str;                                  \
    __stringtable->lesser = NULL;                                    \
    __stringtable->greater = NULL;                                   \
}

#ifndef skip_precalculation
    StringTable* newStringTable(char* varname, StringTable* lesser, StringTable* greater) {
        StringTable* ret;
        malloc_k_pos(sizeof(StringTable), ret, _edata_stack);
        debug_malloc("newStringTable\n");
        ret->varname = varname;
        ret->lesser = lesser;
        ret->greater = greater;
        return ret;
    }

    void buildStringTable () {
#ifdef QFT
        _edata_stack = stack_head + 49;
#endif

        stringTableHeadList[0] = newStringTable(mod_str, NULL, NULL);
        stringTableHeadList[1] = newStringTable(lambda_str, newStringTable(atom_str, NULL, NULL), NULL);
        stringTableHeadList[2] = newStringTable(macro_str, NULL, NULL);
        stringTableHeadList[3] = newStringTable(cons_str, NULL, NULL);
        stringTableHeadList[4] = newStringTable(t_str, NULL, NULL);
        stringTableHeadList[5] = NULL;
        stringTableHeadList[6] = newStringTable(
            eq_str,
            newStringTable(car_str, NULL, NULL),
            newStringTable(progn_str, NULL, NULL)
        );
        stringTableHeadList[7] = NULL;
        stringTableHeadList[8] = newStringTable(eval_str, NULL, NULL);
        stringTableHeadList[9] = newStringTable(cdr_str, NULL, newStringTable(while_str, NULL, NULL));
        stringTableHeadList[10] = newStringTable(ast_str, NULL, NULL);
        stringTableHeadList[11] = newStringTable(
            define_str,
            newStringTable(plus_str, NULL, NULL),
            newStringTable(lambdaast_str, NULL, NULL)
        );
        stringTableHeadList[12] = newStringTable(list_str,
            newStringTable(lt_str, NULL, NULL),
            newStringTable(macroast_str, NULL, NULL)
        );
        stringTableHeadList[13] = newStringTable(print_str, newStringTable(minus_str, NULL, NULL), NULL);
        stringTableHeadList[14] = newStringTable(quote_str, newStringTable(gt_str, NULL, NULL), NULL);
        stringTableHeadList[15] = newStringTable(if_str, NULL, NULL);
    }
#endif


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

#define pushTailList(__value) {      \
    _list = newList(__value, NULL);  \
    listTail->next = _list;          \
    listTail = _list;                \
}

void parseExpr(List* listTail) {
parseExprHead:
    // Remove whitespace
    while (c == ' ' || c == '\n') {
        c = getchar();
    }
    if (c == ';') {
        do {
            c = getchar();
            if (isEOF(c)) {
                return;
            }
        } while(c != '\n');
        goto parseExprHead;
    }
    if (isEOF(c)) {
        return;
    }
    // Parse as a list
    if (c == '(') {
        c = getchar();

        debug("pushing list...\n");
        parseExpr(listTail);

        _list = listTail->next;
        pushTailList(_list ? (Value)_list : NULL);
        goto parseExprHead;
    }

    if (c == ')') {
        debug1("popping list...\n%s", "");

        c = getchar();
        return;
    }

    // Parse as an atom
    i = 0;
    sthash = 0;
    while (isNotEOF(c) && c != ' ' && c != '\n' && c != ')' && c != '(' && c != ';') {
        // putchar(c);
        buf[i] = c;
        sthash += c;
        ++i;
        c = getchar();
    }
    buf[i] = '\0';


    // If the expression is an integer literal, evaluate it
    j = buf[0];
    if (('0' <= j && j <= '9') || (j == '-' && ('0' <= buf[1] && buf[1] <= '9'))) {
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
            debug_malloc("parseAtom (new string)\n");
            for(; (*s1 = *s2); ++s1, ++s2) {}
            newStringTable_(_stringtable, _str);
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
    debug_malloc("newEnv\n");
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

void eval(Value node);
#define evalAsInt() {                                                         \
    if (!isIntValue(_value)) {                                                \
        eval(_value);                                                         \
    }                                                                         \
    i = (unsigned long long)(((unsigned long long)_value) & (~typemask));     \
}

typedef struct {
    union {
        char c_eval_;
        List* list;
        Value _value_eval;
        int* prev_edata;
    };
    union {
        Value arg1_;
        List* _list_eval_2;
        Env* env2;
    };
    union {
        List* arg2list_;
        int n_;
        Env* e;
    };
} EvalStack;

void progn (List* list) {
    _value = NULL;
    while (list) {
        eval(list->value);
        list = list->next;
    }
}

void printValue();

void eval(Value node) {
    EvalStack evalstack;
    debug("entering eval...\n");

#define _value_eval (evalstack._value_eval)
#define _list_eval (evalstack.list)
#define _list_eval_2 (evalstack._list_eval_2)
#define arg1 (evalstack.arg1_)
#define arg2list (evalstack.arg2list_)
#define c_eval (evalstack.c_eval_)
#define n_ (evalstack.n_)
#define evalstack_env (evalstack.e)

    if (!node || isIntValue(node) || isLambdaValue(node)) {
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

    // The head of the list is an atom
    if (isAtomValue(((List*)node)->value)) {
        #define headstr _str
        // headstr = node->value->str;
        atom2Str(((List*)node)->value);

#ifdef QFT
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

#ifdef QFT
        goto *((void*)*((int*)((int)&evalhash + (((int)_str) >> 1))));
#else
             if (_str == define_str) goto eval_define;
        else if (_str == if_str) goto eval_if;
        else if (_str == quote_str) goto eval_quote;
        else if (_str == car_str) goto eval_car;
        else if (_str == cdr_str) goto eval_cdr;
        else if (_str == cons_str) goto eval_cons;
        else if (_str == atom_str) goto eval_atom;
        else if (_str == print_str) goto eval_print;
        else if (_str == progn_str) goto eval_progn;
        else if (_str == while_str) goto eval_while;
        else if (_str == lambda_str || _str == macro_str || _str == lambdaast_str || _str == macroast_str) goto eval_createlambda;
        else if (_str == eval_str) goto eval_eval;
        else if (_str == eq_str) goto eval_eq;
        else if (_str == plus_str || _str == minus_str || _str == ast_str || _str == slash_str || _str == mod_str) goto eval_arith;
        else if (_str == lt_str || _str == gt_str) goto eval_cmp;
        else if (_str == list_str) goto eval_list;
        else goto eval_lambda_call;
#endif

eval_define:
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
            debug("appending to the global environment...\n");

            // Append to the global environment
            _env = NULL;
            _env3->next = newEnv();
            debug("appended to the global environment.\n");
            return;

eval_if:
            #define condition _value
            eval(arg1);
            eval(condition ? arg2list->value : arg2list->next->value);
            return;
            #undef condition

eval_car:
            eval(arg1);
            if (_value) {
                _value = ((List*)_value)->value;
            }
            return;

eval_cdr:
            eval(arg1);
            if (_value) {
                _value = (Value)((List*)_value)->next;
            }
            return;

eval_list:
            #define initlist _list_eval
            #define curlist node
            if (!arg1) {
                _value = NULL;
            } else {
                eval(arg1);
                initlist = newList(_value, NULL);
                arg1 = (Value)((List*)node)->next;
                curlist = (Value)initlist;
                while ((arg1 = (Value)((List*)arg1)->next)) {
                    eval(((List*)arg1)->value);
                    ((List*)curlist)->next = newList(_value, NULL);
                    curlist = (Value)((List*)curlist)->next;
                }
                _value = (Value)initlist;
            }
            return;
            #undef initlist
            #undef curlist

eval_cons:
            #define car node
            eval(arg1);
            car = _value;
            eval(arg2list->value);
            _value = (Value)newList(car, (List*)_value);
            return;
            #undef car

eval_print:
            eval(arg1);
            _list_eval = (List*)_value;
            printValue();
            if( ((List*)((List*)node)->next)->next) {
                putchar('\n');
            }
            _value = (Value)_list_eval;
            return;

eval_progn:
            progn(((List*)node)->next);
            return;

eval_while:
            _value_eval = NULL;
            while (eval(arg1), _value) {
                progn(arg2list);
                _value_eval = _value;
            }
            _value = _value_eval;
            return;

eval_createlambda:
            newLambdaData(
                _lambda,
                ((List*)node)->next,
                _evalenv,
                (headstr[0] == 'm' ? (headstr[5] == '*' ? L_TEMPMACRO :  L_MACRO) : headstr[6] == '*' ? L_TEMPLAMBDA : L_LAMBDA)
            );
            lambda2Value(_lambda);
            return;

eval_eq:
            eval(arg1);
            node = _value;
            eval(arg2list->value);
            #define n1 node
            #define n2 _value
            // Works for nil, atoms, integers, and lambdas.
            // Since atoms with the same string always carry the same string pointers, this works for atoms as well.
            _value = ((unsigned long long)n1 == (unsigned long long)n2) ? true_value : NULL;
            return;
            #undef n1
            #undef n2

eval_arith:
            c_eval = headstr[0];

            #define nextlist _list_eval_2
            nextlist = ((List*)node)->next;
            _value = nextlist->value;
            evalAsInt();

            if (c_eval == '-' && !(nextlist->next)) {
                i = -i;
            } else {
                n_ = i;
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

eval_cmp:
            c_eval = headstr[0];
            _value = arg2list->value;
            evalAsInt();
#ifndef QFT
            i &= valuemask_14;
#endif
            debug1_2("[arg2:%lld]\n", i);
            if (i > 8191) {
                i = i - 16384;
            }
            debug1_2("[arg2:%lld]\n", i);
            n_ = i;
            _value = arg1;
            evalAsInt();
#ifndef QFT
            i &= valuemask_14;
#endif
            debug1_2("[arg1:%lld]\n", i);
            if (i > 8191) {
                i = i - 16384;
            }
            debug1_2("[arg1:%lld]\n", i);
            j = (int)i < (int)n_;
            _value = (c_eval == '<' ? j : !j) ? true_value : NULL;
            return;

eval_quote:
            _value = arg1;
            return;

eval_atom:
            eval(arg1);
            _value = !_value || (isIntValue(_value)) || (isAtomValue(_value)) ? true_value : NULL;
            return;

eval_eval:
            eval(arg1);
            eval(_value);
            return;
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
    eval(((List*)node)->value);
    value2Lambda(_value, node);
    curargname = curlambda->definition->value;


    // The body of the macro should be evaluated in the environment they are called in,
    // instead of the environment they were defined in
    k = lambdaType(curlambda);
    curenv = (k == L_MACRO || k == L_TEMPMACRO) ? _evalenv : lambdaEnv(curlambda);

    while (curargname) {
        if (curarg) {
            k = lambdaType(curlambda);
            if (k == L_MACRO || k == L_TEMPMACRO) {
                _value = curarg->value;
            } else {
                eval(curarg->value);
            }
        } else {
            _value = NULL;
        }

        atom2Str(curargname->value);
        _env = curenv;
        if (lambdaType(curlambda) == L_LAMBDA) {
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
    evalstack.env2 = _evalenv;
    _evalenv = curenv;

    k = lambdaType(curlambda);
    if (k == L_MACRO || k == L_TEMPMACRO) {
        if (k == L_TEMPMACRO) {
#ifdef QFT
                evalstack.prev_edata = _edata;
                // _edata = stack_head;
#endif
        } else {
            evalstack.prev_edata = NULL;
        }

        progn(curlambda->definition->next);

        if (evalstack.prev_edata) {
#ifdef QFT
                _edata = evalstack.prev_edata;
#endif
        }

        _evalenv = evalstack.env2;
        eval(_value);
    } else {
        progn(curlambda->definition->next);
        _evalenv = evalstack.env2;
    }
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


void printValue() {
    List* list;
    if (!_value) {
        debug("<nil>");
        goto printlist;
    }

    if (isIntValue(_value)) {
        debug("<int>");
        k = ((unsigned long long)_value) & (~typemask);
#ifndef QFT
        k &= valuemask_14;
#endif
        debug1_2("[%lld]", (unsigned long long)_value);
        debug1_2("[%lld]", k);
        if (k > 8191) {
            putchar('-');
            k = 16384-k;
        }
        _str = buf + 7;
        *_str = '\0';
        do {
            _div(k, 10);
            --_str;
            *_str = (r + '0');
            k = q;
        } while (k);
    } else if (isAtomValue(_value)){
        debug("<atom>");
        atom2Str(_value);
    } else if (isLambdaValue(_value)){
        debug("<lambda>");
        value2Lambda(_value, _value);
        k = lambdaType((Lambda*)_value);
        putchar('#');
        putchar('<');
        _str = (k == L_LAMBDA || k == L_TEMPLAMBDA) ? "Closure>" : "Macro>";
    } else {
        debug("<list>");
printlist:
        list = (List*)_value;
        putchar('(');
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
    for (; *_str; ++_str){
        putchar(*_str);
    }
}

int main (void) {
#ifndef QFT
    str2Atom(t_str);
    true_value = _value;
    initialenv.value = true_value;
#endif

#ifndef skip_precalculation
    buildStringTable();
#endif
#ifdef precalculation_run
    return 0;
#endif

    c = getchar();
    do {
        parseExpr(curlist);
    } while((curlist = curlist->next));

    debug_malloc("==== end of parsing phase ====\n");

    curlist = initlist.next;
    while (curlist) {
        eval(curlist->value);
        // _value = curlist->value;
        // printValue();
        curlist = curlist->next;
    }
#ifdef QFT
    // Clear out the stdin, for aesthetic reasons.
    // Since the stdin will be overwritten during runtime,
    // this will prevent the stdin viewer from viewing overwritten data.
    *((char*)(stdin_startpos)) = 0;

    // This is required - this inserts a terminal symbol to the end of the stdout buffer. 
    *(*((char**)QFTASM_STDOUT)) = 0;
#endif
}
