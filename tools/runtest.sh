#!/bin/bash
set -e

QFTASM_INTERPRETER=./tools/qftasmi.sh
lisp_qftasm="./out/lisp.qftasm"
ramdump_csv="./out/ramdump.csv"
qftasm_executable=./out/lisp

if [ ! -f $lisp_qftasm ] || [ ! -f $ramdump_csv ]; then
    echo "File(s) ${lisp_qftasm} or ${ramdump_csv} not found. Run \`make all\` to obtain these files."
    exit 1
fi

if [ "$1" = "--test-executable" ]; then
    function runlisp () {
        echo "$1" | $qftasm_executable
    }
    engine_name="Executable"
else
    function runlisp () {
        $QFTASM_INTERPRETER -i $lisp_qftasm -m $ramdump_csv -u "$1"
    }
    engine_name="QFTASM    "
fi

function runlisp_expect () {
    if [[ $1 == *$'\n'* ]] || [[ $1 == *$'\r'* ]]; then
       case="$(printf "$1" | head -n 1) ..."
    else
       case=$1
    fi

    expected=$2

    echo "Case ${case}:"
    result_qft=$(runlisp "$1")
    echo "${engine_name}: ${result_qft}"
    echo "Expected  : ${expected}"

    if [[ $result_qft == $expected ]]; then
        echo "Passed."
    else
        echo "Test failed!"
        exit 1
    fi   
}

function run_and_compare_with_hy () {
    echo "Case ${1}:"
    result_qft=$(runlisp "$1")
    echo "${engine_name}: ${result_qft}"

    result_hy=$(hy -c "$1")
    echo "Hy        : ${result_hy}"

    if [[ $2 == "-notest" ]]; then
        return;
    fi
    if [[ $result_qft == "t" ]] && [[ $result_hy == "True" ]]; then
        echo "Passed."
    elif [[ $result_qft == "()" ]] && [[ $result_hy == "False" ]]; then
        echo "Passed."
    elif [[ $result_qft == $result_hy ]]; then
        echo "Passed."
    else
        echo "Test failed!"
        exit 1
    fi
}



runlisp_expect "(define a (quote (1 2 3))) (print a)" "(1 2 3)"
runlisp_expect "(print (quote ()))" "()"

runlisp_expect "(print (if (atom 1) (quote true) (quote false)))" "true"

runlisp_expect "(print (car (quote (1 2 3))))" "1"
runlisp_expect "(print (cdr (quote (1 2 3))))" "(2 3)"

runlisp_expect "(print (cons 1 (cons (cons 2 (cons 3 ())) (cons 4 ()))))" "(1 (2 3) 4)"

runlisp_expect "(print (list 1 (list 2 3) 4)" "(1 (2 3) 4)"

runlisp_expect "(print (atom ())"  "t"
runlisp_expect "(print (atom 1)"  "t"
runlisp_expect "(print (atom hello!)"  "t"
runlisp_expect "(print (atom (quote (1 2 3))))"  "()"

runlisp_expect "(print ())" "()"
runlisp_expect "(print 1 t) (print 2 t)" "1
2"

runlisp_expect "(if (atom 1) (progn (print 1) (print 2)) ())" "12"

runlisp_expect "(define n 3) (while (< 0 n) (print n) (define n (- n 1)))" "321"

runlisp_expect "(print (lambda (n) 1))" "#<Closure>"
runlisp_expect "(print (macro (x) x))" "#<Macro>"
runlisp_expect "(print ((lambda (n) (+ 1 n)) 5))" "6"
runlisp_expect "(print ((macro (x) (list (quote quote) x)) Hi!))" "Hi!"

runlisp_expect "(print (eq 1 1))" "t"
runlisp_expect "(print (eq 1 2))" "()"
runlisp_expect "(print (eq 1 ()))" "()"
runlisp_expect "(print (eq () ()))" "t"
runlisp_expect "(print (eq 1 (lambda () ())))" "()"
runlisp_expect "(print (eq () (lambda () ())))" "()"
runlisp_expect "(print (eq 1 (macro () ())))" "()"
runlisp_expect "(print (eq () (macro () ())))" "()"
runlisp_expect "(define l (lambda () ())) (print (eq l l))" "t"
runlisp_expect "(define l (macro () ())) (print (eq l l))" "t"
runlisp_expect "(print (eq (lambda () ()) (lambda () ())))" "()"
runlisp_expect "(print (eq (macro () ()) (macro () ())))" "()"

runlisp_expect "(print (eval (quote ((lambda (n) (+ 1 n)) 7))))" "8"
runlisp_expect "(print (eval (quote (lambda (n) (+ 1 n)))))" "#<Closure>"
runlisp_expect "(print (eval 5))" "5"
runlisp_expect "(print (eval ()))" "()"

echo "===="

run_and_compare_with_hy "(print (+ -2 -3))"
run_and_compare_with_hy "(print (+ 2 -3))"
run_and_compare_with_hy "(print (+ -2 3))"
run_and_compare_with_hy "(print (+ 2 3))"

run_and_compare_with_hy "(print (- -2 -3))"
run_and_compare_with_hy "(print (- 2 -3))"
run_and_compare_with_hy "(print (- -2 3))"
run_and_compare_with_hy "(print (- 2 3))"

run_and_compare_with_hy "(print (* -2 -3))"
run_and_compare_with_hy "(print (* 2 -3))"
run_and_compare_with_hy "(print (* -2 3))"
run_and_compare_with_hy "(print (* 2 3))"

runlisp_expect "(print (mod 17 3))" "2"
runlisp_expect "(print (mod 15 3))" "0"

run_and_compare_with_hy "(print -10)"
run_and_compare_with_hy "(print -5)"
run_and_compare_with_hy "(print -200)"
run_and_compare_with_hy "(print (- -200))"
run_and_compare_with_hy "(print (- 190 150))"
run_and_compare_with_hy "(print -2000)"
run_and_compare_with_hy "(print -4000)"
run_and_compare_with_hy "(print -8000)"
run_and_compare_with_hy "(print 2000)"
run_and_compare_with_hy "(print 4000)"
run_and_compare_with_hy "(print 8000)"
run_and_compare_with_hy "(print (+ 8191))"
run_and_compare_with_hy "(print (- 8191))"
run_and_compare_with_hy "(print (- -8191 1))"

runlisp_expect "(print (+ 8191 1))" "-8192"
runlisp_expect "(print (+ 8191 2))" "-8191"
runlisp_expect "(print (- -8191 2))" "8191"

echo "===="

run_and_compare_with_hy "(print (< -5 0))"
run_and_compare_with_hy "(print (< -5 -2))"
run_and_compare_with_hy "(print (< -2 -5))"
run_and_compare_with_hy "(print (< -2 0))"
run_and_compare_with_hy "(print (< 0 -2))"
run_and_compare_with_hy "(print (> -5 0))"
run_and_compare_with_hy "(print (> -5 -2))"
run_and_compare_with_hy "(print (> -2 -5))"
run_and_compare_with_hy "(print (> -2 0))"
run_and_compare_with_hy "(print (> 0 -2))"
run_and_compare_with_hy "(print (< -5 1))"
run_and_compare_with_hy "(print (< -5 -2))"
run_and_compare_with_hy "(print (< -2 -5))"
run_and_compare_with_hy "(print (< -2 1))"
run_and_compare_with_hy "(print (< 1 -2))"
run_and_compare_with_hy "(print (> -5 1))"
run_and_compare_with_hy "(print (> -5 -2))"
run_and_compare_with_hy "(print (> -2 -5))"
run_and_compare_with_hy "(print (> -2 1))"
run_and_compare_with_hy "(print (> 1 -2))"

echo "===="

runlisp_expect "$(< ./lisp/print.lisp)" '42'
runlisp_expect "$(< ./lisp/lambda.lisp)" '42'
runlisp_expect "$(< ./lisp/printquote.lisp)" 'Hi!'
runlisp_expect "$(< ./lisp/factorial.lisp)" '120'
runlisp_expect "$(< ./lisp/z-combinator.lisp)" '120'
runlisp_expect "$(< ./lisp/backquote-splice.lisp)" '(1 2 3 4)'
runlisp_expect "$(< ./lisp/backquote.lisp)" 'Hi!(a b 5)(` (a b (~ c)))'
runlisp_expect "$(< ./lisp/object-oriented-like.lisp)" $'1\n9\n2\n8\n2'
runlisp_expect "$(< ./lisp/primes-print.lisp)" $'2\n3\n5\n7\n11\n13\n17\n19'
runlisp_expect "$(< ./lisp/primes.lisp)" '(2 3 5 7 11 13 17 19)'

echo "All tests have passed."
