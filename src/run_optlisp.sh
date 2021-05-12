ramdump_csv=./src/ramdump.csv
lisp_opt_qftasm=./src/lisp_opt.qftasm

QFTASM_RAMSTDIN_BUF_STARTPOSITION=340
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790
QFTASM_STACK_SIZE=233

#================================================================
# Run the final code
#================================================================
echo "(print (lambda (n) 1))" | python ../elvm/tools/qftasm/qftasm_interpreter.py \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  -i $lisp_opt_qftasm \
#   --suppress-address-overflow-warning \


cat print.lisp     | python ../elvm/tools/qftasm/qftasm_interpreter.py \
  -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  --debug-plot-memdist \

mv memdist.png memdist_print.png


cat objects.lisp   | python ../elvm/tools/qftasm/qftasm_interpreter.py \
  -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  --debug-plot-memdist \

mv memdist.png memdist_objects.png

cat backquote.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py \
  -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  --debug-plot-memdist \

mv memdist.png memdist_backquote.png

cat fact.lisp      | python ../elvm/tools/qftasm/qftasm_interpreter.py \
  -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  --debug-plot-memdist \

mv memdist.png memdist_fact.png

cat primes.lisp    | python ../elvm/tools/qftasm/qftasm_interpreter.py \
  -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --stack-size $QFTASM_STACK_SIZE \
  --debug-ramdump \
  --debug-plot-memdist \

mv memdist.png memdist_primes.png
