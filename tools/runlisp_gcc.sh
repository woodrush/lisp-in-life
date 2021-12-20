#!/bin/bash
set -e

function lisp_interpreter () {
    ./out/lisp < /dev/stdin
    echo ""
}

cat ./lisp/print.lisp | lisp_interpreter
cat ./lisp/object-oriented-like.lisp | lisp_interpreter
cat ./lisp/backquote.lisp | lisp_interpreter
cat ./lisp/backquote-splice.lisp | lisp_interpreter
cat ./lisp/z-combinator.lisp | lisp_interpreter
cat ./lisp/primes.lisp | lisp_interpreter
cat ./lisp/primes-print.lisp | lisp_interpreter
