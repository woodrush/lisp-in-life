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
run_and_compare_with_hy "(print (- 8191))"
run_and_compare_with_hy "(print (- -8191 1))"

echo "===="
runlisp_expect "(print (+ 8191 1))" "-8192"
runlisp_expect "(print (+ 8191 2))" "-8191"
runlisp_expect "(print (- -8191 2))" "8191"

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
runlisp_expect "$(< primes-print.lisp)" $'2\n3\n5\n7\n11\n13\n17\n19'

echo "All tests have passed."
