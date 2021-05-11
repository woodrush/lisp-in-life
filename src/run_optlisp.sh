ramdump_csv=./src/ramdump.csv
lisp_opt_qftasm=./src/lisp_opt.qftasm

#================================================================
# Run the final code
#================================================================
echo "(print (lambda (n) 1))" | python ../elvm/tools/qftasm/qftasm_interpreter.py \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos 350 \
  --stdout-pos 799 \
  --stack-size 223 \
  --debug-ramdump \
  --debug-plot-memdist \
  -i $lisp_opt_qftasm \

# TODO: Use XOR for not



cat print.lisp     | python ../elvm/tools/qftasm/qftasm_interpreter.py -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --debug-ramdump \
  --suppress-address-overflow-warning \

cat objects.lisp   | python ../elvm/tools/qftasm/qftasm_interpreter.py -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --debug-ramdump \
  --suppress-address-overflow-warning \

cat backquote.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --debug-ramdump \
  --suppress-address-overflow-warning \

cat fact.lisp      | python ../elvm/tools/qftasm/qftasm_interpreter.py -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --debug-ramdump \
  --suppress-address-overflow-warning \

cat primes.lisp    | python ../elvm/tools/qftasm/qftasm_interpreter.py -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --debug-ramdump \
  --suppress-address-overflow-warning \
