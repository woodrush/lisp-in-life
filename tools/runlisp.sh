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

QFTASM_INTERPRETER=./tools/qftasmi.sh
ramdump_csv=./out/ramdump.csv

function lisp_interpreter () {
    if [ "$plot_memdist" = "" ]; then
        memdist_opt=""
    else
        mkdir -p memdist
        memdist_opt=$1
    fi
    $QFTASM_INTERPRETER \
        -i $lisp_opt_qftasm \
        -m $ramdump_csv \
        -u "$(< /dev/stdin)" \
        -p "--debug-ramdump $memdist_opt"

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
cat primes-print.lisp | lisp_interpreter --debug-plot-memdist primes
