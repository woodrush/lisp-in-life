#!/bin/bash
set -e

# gcc src/lisp.c -Isrc && echo "(print (< -5 0))" | ./a.out && hy -c "(print (< -5 0))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (< -5 -2))" | ./a.out && hy -c "(print (< -5 -2))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (< -2 -5))" | ./a.out && hy -c "(print (< -2 -5))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (< -2 0))" | ./a.out && hy -c "(print (< -2 0))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (< 0 -2))" | ./a.out && hy -c "(print (< 0 -2))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (> -5 0))" | ./a.out && hy -c "(print (> -5 0))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (> -5 -2))" | ./a.out && hy -c "(print (> -5 -2))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (> -2 -5))" | ./a.out && hy -c "(print (> -2 -5))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (> -2 0))" | ./a.out && hy -c "(print (> -2 0))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (> 0 -2))" | ./a.out && hy -c "(print (> 0 -2))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (< -5 1))" | ./a.out && hy -c "(print (< -5 1))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (< -5 -2))" | ./a.out && hy -c "(print (< -5 -2))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (< -2 -5))" | ./a.out && hy -c "(print (< -2 -5))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (< -2 1))" | ./a.out && hy -c "(print (< -2 1))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (< 1 -2))" | ./a.out && hy -c "(print (< 1 -2))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (> -5 1))" | ./a.out && hy -c "(print (> -5 1))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (> -5 -2))" | ./a.out && hy -c "(print (> -5 -2))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (> -2 -5))" | ./a.out && hy -c "(print (> -2 -5))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (> -2 1))" | ./a.out && hy -c "(print (> -2 1))"
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (> 1 -2))" | ./a.out && hy -c "(print (> 1 -2))"
# echo ""
# echo "===="


# gcc src/lisp.c -Isrc && echo "(print -5)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print -10)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print -200)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (- -200))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (- 190 150))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print -2000)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print -4000)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print -8000)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print 2000)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print 4000)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print 8000)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (+ 8191))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (+ 8191 1))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (+ 8191 2))" | ./a.out
# echo ""

# gcc src/lisp.c -Isrc && echo "(print (- 8191))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (- -8191 1))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (- -8191 2))" | ./a.out
# echo ""


# gcc src/lisp.c -Isrc -Wall && echo 1 | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print 1)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (atom 1))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (atom (+ 1 2)))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (atom ()))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (atom (quote a)))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (atom (quote 1)))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (atom (quote (1 2 3))))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (define n 17))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(define n 15)(print n)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (- (+ 1 17) 1))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (- 27 1 1)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (* 27 2 1)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(define n 158)(print n)" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(define n 15)(print (+ 1 n))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print ((lambda (n) (+ 3 n)) 5))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (< 1 5))" | ./a.out
# echo ""
# gcc src/lisp.c -Isrc && echo "(print (> 1 5))" | ./a.out
# echo ""
gcc src/lisp.c -Isrc && cat print.lisp | ./a.out #> print.gcc.out
echo ""
gcc src/lisp.c -Isrc && cat fact.lisp | ./a.out #> fact.gcc.out
echo ""
gcc src/lisp.c -Isrc && cat primes.lisp | ./a.out #> primes.gcc.out
echo ""
gcc src/lisp.c -Isrc && cat backquote.lisp | ./a.out #> primes.gcc.out
echo ""
gcc src/lisp.c -Isrc && cat object-oriented-like.lisp | ./a.out #> objects.gcc.out
echo ""
gcc src/lisp.c -Isrc && echo "(print (lambda () 1))" | ./a.out
echo ""
gcc src/lisp.c -Isrc && echo "(print ((lambda () 1)))" | ./a.out
echo ""
gcc src/lisp.c -Isrc && echo "(print ())" | ./a.out
echo ""
gcc src/lisp.c -Isrc && echo "(print (quote ()))" | ./a.out
echo ""
gcc src/lisp.c -Isrc && echo "(print (atom (quote (1 2 3))))" | ./a.out
echo ""
gcc src/lisp.c -Isrc && echo "(print t)" | ./a.out
echo ""
gcc src/lisp.c -Isrc && echo "(print (eval t))" | ./a.out
echo ""
gcc src/lisp.c -Isrc && echo "(print (eval (eval t)))" | ./a.out
