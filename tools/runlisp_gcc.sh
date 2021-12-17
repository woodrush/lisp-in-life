#!/bin/bash
set -e

function lisp_interpreter () {
    ./out/lisp < /dev/stdin
    echo ""
}

cat print.lisp | lisp_interpreter
cat object-oriented-like.lisp | lisp_interpreter
cat backquote.lisp | lisp_interpreter
cat backquote-splice.lisp | lisp_interpreter
cat z-combinator.lisp | lisp_interpreter
cat primes.lisp | lisp_interpreter
