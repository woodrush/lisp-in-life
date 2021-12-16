#!/bin/bash
set -e

if [ $# -eq 0 ]; then
    lisp_opt_qftasm=./src/lisp_opt.qftasm
else
    lisp_opt_qftasm=$1
fi

if [ "$2" = "--plot-memdist" ]; then
    plot_memdist=1
else
    plot_memdist=""
fi

QFTASM_INTERPRETER=./elvm/tools/qftasm/qftasm_interpreter.py

ramdump_csv=./src/ramdump.csv

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790
QFTASM_STACK_SIZE=233

function lisp_interpreter () {
    if [ "$plot_memdist" = "" ]; then
        memdist_opt=""
    else
        mkdir -p memdist
        memdist_opt=$1
    fi
    python $QFTASM_INTERPRETER \
    --initial-ramvalues $ramdump_csv \
    --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
    --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
    --stack-size $QFTASM_STACK_SIZE \
    --debug-ramdump \
    -i $lisp_opt_qftasm \
    $memdist_opt < /dev/stdin

    if [ ! $plot_memdist = "" ] && [ ! -z "$2" ]; then
        mv memdist.png $2
    fi
}

#================================================================
# Run the lisp programs
#================================================================
echo "(print (lambda (n) 1))" | lisp_interpreter

cat print.lisp | lisp_interpreter --debug-plot-memdist ./memdist/memdist_print.png
cat object-oriented-like.lisp | lisp_interpreter --debug-plot-memdist ./memdist/memdist_object-oriented-like.png
cat backquote.lisp | lisp_interpreter --debug-plot-memdist ./memdist/memdist_backquote.png
cat backquote-splice.lisp | lisp_interpreter --debug-plot-memdist ./memdist/memdist_backquote-splice.png
cat z-combinator.lisp | lisp_interpreter --debug-plot-memdist ./memdist/memdist_fact.png
cat primes.lisp | lisp_interpreter --debug-plot-memdist ./memdist/memdist_primes.png
