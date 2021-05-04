Value lambda_value = { .type = ATOM, .str = lambda_str };
Value print_value = { .type = ATOM, .str = print_str };
Value define_value = { .type = ATOM, .str = define_str };
Value quote_value = { .type = ATOM, .str = quote_str };
Value list_value = { .type = ATOM, .str = list_str };
Value if_value = { .type = ATOM, .str = if_str };
Value car_value = { .type = ATOM, .str = car_str };
Value while_value = { .type = ATOM, .str = while_str };
Value progn_value = { .type = ATOM, .str = progn_str };
Value macro_value = { .type = ATOM, .str = macro_str };
Value lambdaast_value = { .type = ATOM, .str = lambdaast_str };
Value eq_value = { .type = ATOM, .str = eq_str };
Value cons_value = { .type = ATOM, .str = cons_str };
Value plus_value = { .type = ATOM, .str = plus_str };
Value t_value = { .type = ATOM, .str = t_str };
Value mod_value = { .type = ATOM, .str = mod_str };
Value eval_value = { .type = ATOM, .str = eval_str };
Value cdr_value = { .type = ATOM, .str = cdr_str };
Value minus_value = { .type = ATOM, .str = minus_str };
Value ast_value = { .type = ATOM, .str = ast_str };
Value lt_value = { .type = ATOM, .str = lt_str };
Value gt_value = { .type = ATOM, .str = gt_str };
Value slash_value = { .type = ATOM, .str = slash_str };
Value atom_value = { .type = ATOM, .str = atom_str };


StringTable stringtable_slash = {
    .varname = slash_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_if = {
    .varname = if_str,
    .lesser = &stringtable_slash,
    .greater = NULL,
};
StringTable stringtable_gt = {
    .varname = gt_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_quote = {
    .varname = quote_str,
    .lesser = &stringtable_gt,
    .greater = NULL,
};
StringTable stringtable_minus = {
    .varname = minus_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_print = {
    .varname = print_str,
    .lesser = &stringtable_minus,
    .greater = NULL,
};
StringTable stringtable_lt = {
    .varname = lt_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_list = {
    .varname = list_str,
    .lesser = &stringtable_lt,
    .greater = NULL,
};
StringTable stringtable_lambdaast = {
    .varname = lambdaast_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_plus = {
    .varname = plus_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_define = {
    .varname = define_str,
    .lesser = &stringtable_plus,
    .greater = &stringtable_lambdaast,
};
StringTable stringtable_ast = {
    .varname = ast_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_while = {
    .varname = while_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_cdr = {
    .varname = cdr_str,
    .lesser = NULL,
    .greater = &stringtable_while,
};
StringTable stringtable_eval = {
    .varname = eval_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_progn = {
    .varname = progn_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_car = {
    .varname = car_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_eq = {
    .varname = eq_str,
    .lesser = &stringtable_car,
    .greater = &stringtable_progn,
};
StringTable stringtable_t = {
    .varname = t_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_cons = {
    .varname = cons_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_macro = {
    .varname = macro_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_atom = {
    .varname = atom_str,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_lambda = {
    .varname = lambda_str,
    .lesser = &stringtable_atom,
    .greater = NULL,
};
StringTable stringtable_mod = {
    .varname = mod_str,
    .lesser = NULL,
    .greater = NULL,
};

// eval : 8
// lambda* : 11
// atom : 1
// quote : 14
// macro : 2
// define : 11
// while : 9
// progn : 6
// lambda : 1
// > : 14
// < : 12
// + : 11
// - : 13
// * : 10
// / : 15
// t : 4
// mod : 0
// print : 13
// cons : 3
// cdr : 9
// car : 6
// eq : 6
// if : 15
// list : 12

StringTable* stringTableHeadList[16] = {
    &stringtable_mod,        // 0
    &stringtable_lambda,     // 1 stringtable_atom
    &stringtable_macro,      // 2
    &stringtable_cons,       // 3
    &stringtable_t,          // 4
    NULL,                    // 5
    &stringtable_eq,         // 6  stringtable_car, stringtable_progn
    NULL,                    // 7
    &stringtable_eval,       // 8
    &stringtable_cdr,        // 9  stringtable_while
    &stringtable_ast,        // 10
    &stringtable_define,     // 11 stringtable_plus, stringtable_lambdaast
    &stringtable_list,       // 12 stringtable_lt
    &stringtable_print,      // 13 stringtable_minus
    &stringtable_quote,      // 14 stringtable_gt
    &stringtable_if,         // 15
};

Env initialenv = {
    .varname = NULL,
    .value = NULL,
    .next = NULL,
    .prev = (Env*)1,
};

// .type = 1, since ->type and ->next are inside the same union
List nil_value = { .type = 1, .value = NULL };
