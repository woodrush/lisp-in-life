#!/bin/bash
set -e

if [ $# -eq 0 ]; then
    input=./out/lisp.qftasm
else
    input=$1
fi

ramdump_csv=./out/ramdump.csv

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790
QFTASM_STACK_SIZE=233

function runlisp () {
    echo "$1" | python ./elvm/tools/qftasm/qftasm_interpreter.py \
    -i $input \
    --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
    --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
    --stack-size $QFTASM_STACK_SIZE \
    --initial-ramvalues $ramdump_csv
}

function runlisp_expect () {
    if [[ $1 == *$'\n'* ]] || [[ $1 == *$'\r'* ]]; then
       case="$(printf "$1" | head -n 1) ..."
    else
       case=$1
    fi

    expected=$2

    echo "Case ${case}:"
    result_qft=$(runlisp "$1")
    echo "QFT     : ${result_qft}"
    echo "Expected: ${expected}"

    if [[ $result_qft == $expected ]]; then
        echo "Passed."
    else
        echo "Test failed!"
        exit
    fi   
}

function run_and_compare_with_hy () {
    echo "Case ${1}:"
    result_qft=$(runlisp "$1")
    echo "QFT: ${result_qft}"

    result_hy=$(hy -c "$1")
    echo "Hy : ${result_hy}"

    if [[ $2 == "-notest" ]]; then
        return;
    fi
    if [[ $result_qft == "t" ]] && [[ $result_hy == "True" ]]; then
        echo "Passed."
    elif [[ $result_qft == "()" ]] && [[ $result_hy == "False" ]]; then
        echo "Passed."
    elif [[ $result_qft == $result_hy ]]; then
        echo "Passed."
    elif [[ $2 == "--exfail" ]]; then
        echo "The test failed, but it was expected to fail."
    else
        echo "Test failed!"
        exit
    fi
}

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
run_and_compare_with_hy "(print (+ 8191 1))" --exfail
run_and_compare_with_hy "(print (+ 8191 2))" --exfail
run_and_compare_with_hy "(print (- 8191))"
run_and_compare_with_hy "(print (- -8191 1))" --exfail
run_and_compare_with_hy "(print (- -8191 2))" --exfail

echo "===="

runlisp_expect "(print ())" "()"
runlisp_expect "(print (quote ()))" "()"
runlisp_expect "(print (atom ())"  "t"
runlisp_expect "(print (atom 1)"  "t"
runlisp_expect "(print (atom hello!)"  "t"
runlisp_expect "(print (atom (quote (1 2 3))))"  "()"
runlisp_expect "(print (lambda (n) 1))" "#<Closure>"
runlisp_expect "(print ((lambda (n) (+ 1 n)) 5))" "6"

echo "===="

runlisp_expect "$(< print.lisp)" '42'
runlisp_expect "$(< object-oriented-like.lisp)" $'1\n9\n2\n8\n2'
runlisp_expect "$(< backquote.lisp)" 'Hi!(a b 5)(` (a b (~ c)))'
runlisp_expect "$(< backquote-splice.lisp)" '(1 2 3 4)'
runlisp_expect "$(< z-combinator.lisp)" '120'
runlisp_expect "$(< primes.lisp)" '(2 3 5 7 11 13 17 19)'