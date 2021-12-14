#!/usr/local/bin/bash
set -e

if [ $# -eq 0 ]; then
    lisp_opt_qftasm=./src/lisp_opt.qftasm
else
    lisp_opt_qftasm=$1
fi


QFTASM_INTERPRETER=./elvm/tools/qftasm/qftasm_interpreter.py

ramdump_csv=./src/ramdump.csv

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790
QFTASM_STACK_SIZE=233

#================================================================
# Run the final code
#================================================================
echo "(print (lambda (n) 1))" | python $QFTASM_INTERPRETER \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  -i $lisp_opt_qftasm \


cat print.lisp     | python $QFTASM_INTERPRETER \
  -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  --debug-plot-memdist \

mv memdist.png memdist_print.png


cat object-oriented-like.lisp   | python $QFTASM_INTERPRETER \
  -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  --debug-plot-memdist \

mv memdist.png memdist_object-oriented-like.png

cat backquote.lisp | python $QFTASM_INTERPRETER \
  -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  --debug-plot-memdist \

mv memdist.png memdist_backquote.png

cat backquote-splice.lisp     | python $QFTASM_INTERPRETER \
  -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  --debug-plot-memdist \

mv memdist.png memdist_backquote-splice.png

cat fact.lisp      | python $QFTASM_INTERPRETER \
  -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
--debug-plot-memdist \

mv memdist.png memdist_fact.png

cat primes.lisp    | python $QFTASM_INTERPRETER \
  -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  --debug-plot-memdist \

mv memdist.png memdist_primes.png

