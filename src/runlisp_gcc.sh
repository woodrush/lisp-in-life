#!/bin/bash
set -e

function lisp_interpreter () {
    ./lisp < /dev/stdin
    echo ""
}

echo "(print (< -5 0))" | lisp_interpreter
echo "(print (< -5 -2))" | lisp_interpreter
echo "(print (< -2 -5))" | lisp_interpreter
echo "(print (< -2 0))" | lisp_interpreter
echo "(print (< 0 -2))" | lisp_interpreter
echo "(print (> -5 0))" | lisp_interpreter
echo "(print (> -5 -2))" | lisp_interpreter
echo "(print (> -2 -5))" | lisp_interpreter
echo "(print (> -2 0))" | lisp_interpreter
echo "(print (> 0 -2))" | lisp_interpreter
echo "(print (< -5 1))" | lisp_interpreter
echo "(print (< -5 -2))" | lisp_interpreter
echo "(print (< -2 -5))" | lisp_interpreter
echo "(print (< -2 1))" | lisp_interpreter
echo "(print (< 1 -2))" | lisp_interpreter
echo "(print (> -5 1))" | lisp_interpreter
echo "(print (> -5 -2))" | lisp_interpreter
echo "(print (> -2 -5))" | lisp_interpreter
echo "(print (> -2 1))" | lisp_interpreter
echo "(print (> 1 -2))" | lisp_interpreter


echo "(print -5)" | lisp_interpreter
echo "(print -10)" | lisp_interpreter
echo "(print -200)" | lisp_interpreter
echo "(print (- -200))" | lisp_interpreter
echo "(print (- 190 150))" | lisp_interpreter
echo "(print -2000)" | lisp_interpreter
echo "(print -4000)" | lisp_interpreter
echo "(print -8000)" | lisp_interpreter
echo "(print 2000)" | lisp_interpreter
echo "(print 4000)" | lisp_interpreter
echo "(print 8000)" | lisp_interpreter
echo "(print (+ 8191))" | lisp_interpreter
echo "(print (+ 8191 1))" | lisp_interpreter
echo "(print (+ 8191 2))" | lisp_interpreter
echo "(print (- 8191))" | lisp_interpreter
echo "(print (- -8191 1))" | lisp_interpreter
echo "(print (- -8191 2))" | lisp_interpreter


echo 1 | lisp_interpreter
echo "(print 1)" | lisp_interpreter
echo "(print (atom 1))" | lisp_interpreter
echo "(print (atom (+ 1 2)))" | lisp_interpreter
echo "(print (atom ()))" | lisp_interpreter
echo "(print (atom (quote a)))" | lisp_interpreter
echo "(print (atom (quote 1)))" | lisp_interpreter
echo "(print (atom (quote (1 2 3))))" | lisp_interpreter
echo "(print (define n 17))" | lisp_interpreter
echo "(define n 15)(print n)" | lisp_interpreter
echo "(print (- (+ 1 17) 1))" | lisp_interpreter
echo "(print (- 27 1 1)" | lisp_interpreter
echo "(print (* 27 2 1)" | lisp_interpreter
echo "(define n 158)(print n)" | lisp_interpreter
echo "(define n 15)(print (+ 1 n))" | lisp_interpreter
echo "(print ((lambda (n) (+ 3 n)) 5))" | lisp_interpreter
echo "(print (< 1 5))" | lisp_interpreter
echo "(print (> 1 5))" | lisp_interpreter
echo "(print (lambda () 1))" | lisp_interpreter
echo "(print ((lambda () 1)))" | lisp_interpreter
echo "(print ())" | lisp_interpreter
echo "(print (quote ()))" | lisp_interpreter
echo "(print (atom (quote (1 2 3))))" | lisp_interpreter
echo "(print t)" | lisp_interpreter
echo "(print (eval t))" | lisp_interpreter
echo "(print (eval (eval t)))" | lisp_interpreter

cat print.lisp | lisp_interpreter
cat fact.lisp | lisp_interpreter
cat primes.lisp | lisp_interpreter
cat backquote.lisp | lisp_interpreter
cat object-oriented-like.lisp | lisp_interpreter
