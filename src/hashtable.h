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
    .value = &slash_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_if = {
    .value = &if_value,
    .lesser = &stringtable_slash,
    .greater = NULL,
};
StringTable stringtable_gt = {
    .value = &gt_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_quote = {
    .value = &quote_value,
    .lesser = &stringtable_gt,
    .greater = NULL,
};
StringTable stringtable_minus = {
    .value = &minus_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_print = {
    .value = &print_value,
    .lesser = &stringtable_minus,
    .greater = NULL,
};
StringTable stringtable_lt = {
    .value = &lt_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_list = {
    .value = &list_value,
    .lesser = &stringtable_lt,
    .greater = NULL,
};
StringTable stringtable_lambdaast = {
    .value = &lambdaast_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_plus = {
    .value = &plus_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_define = {
    .value = &define_value,
    .lesser = &stringtable_plus,
    .greater = &stringtable_lambdaast,
};
StringTable stringtable_ast = {
    .value = &ast_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_while = {
    .value = &while_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_cdr = {
    .value = &cdr_value,
    .lesser = NULL,
    .greater = &stringtable_while,
};
StringTable stringtable_eval = {
    .value = &eval_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_progn = {
    .value = &progn_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_car = {
    .value = &car_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_eq = {
    .value = &eq_value,
    .lesser = &stringtable_car,
    .greater = &stringtable_progn,
};
StringTable stringtable_t = {
    .value = &t_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_cons = {
    .value = &cons_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_macro = {
    .value = &macro_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_atom = {
    .value = &atom_value,
    .lesser = NULL,
    .greater = NULL,
};
StringTable stringtable_lambda = {
    .value = &lambda_value,
    .lesser = &stringtable_atom,
    .greater = NULL,
};
StringTable stringtable_mod = {
    .value = &mod_value,
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

// LIST, since ->type and ->next are inside the same union
Value nil_value = { .type = LIST, .value = NULL };
#define nil (&nil_value)
#define true_value (&t_value)
