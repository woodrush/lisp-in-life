#!/bin/bash
set -e

if [ $# -eq 0 ]; then
    lisp_opt_qftasm=./out/lisp.qftasm
else
    lisp_opt_qftasm=$1
fi

if [ "$2" = "--plot-memdist" ]; then
    plot_memdist=1
else
    plot_memdist=""
fi

QFTASM_INTERPRETER=./elvm/tools/qftasm/qftasm_interpreter.py

ramdump_csv=./out/ramdump.csv

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
        mv memdist.png "./memdist/memdist_${2}.png"
        mv memdist2.png "./memdist/memdist2_${2}.png"
        mv memdist_negative.png "./memdist/memdist-negative_${2}.png"
        mv memvaluedist.png "./memdist/memvaluedist_${2}.png"
        mv romdist.png "./memdist/romdist_${2}.png"
        mv stackdist.png "./memdist/stackdist_${2}.png"
    fi
}

#================================================================
# Run the lisp programs
#================================================================
cat print.lisp | lisp_interpreter --debug-plot-memdist print
cat object-oriented-like.lisp | lisp_interpreter --debug-plot-memdist object-oriented-like
cat backquote.lisp | lisp_interpreter --debug-plot-memdist backquote
cat backquote-splice.lisp | lisp_interpreter --debug-plot-memdist backquote-splice
cat z-combinator.lisp | lisp_interpreter --debug-plot-memdist z-combinator
cat primes.lisp | lisp_interpreter --debug-plot-memdist primes
