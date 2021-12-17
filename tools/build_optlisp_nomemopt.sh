lisp_opt_qftasm=./out/lisp.qftasm

target=./src/lisp_no_precalculations.qftasm

ramdump_stack_csv_headlines=180

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790

tmp_eir=./build/tmp.eir
tmp_qftasmpp=./build/tmp.qftasmpp

#================================================================
# Pass 1
#================================================================
echo "Pass 1: Obtain the optimized ROM"

../elvm/out/8cc -S -DQFT -I. -Isrc -o tmp_.eir src/lisp.c

cat ./src/memheader.eir > $tmp_eir
echo "" >> $tmp_eir
cat tmp_.eir >> $tmp_eir
../elvm/out/elc -qftasm \
  --qftasm-stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --qftasm-stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  $tmp_eir > $tmp_qftasmpp

echo "Running compiler optimizations on ${tmp_qftasmpp}..."

$OPTIMIZE_QFTASMPP $tmp_qftasmpp $opt_qftasmpp

echo "Running the qftasm preprocessor.."
python $QFTASM_PP $opt_qftasmpp > $target
echo "Done."
echo "Created ${target}."
wc -l $target
echo ""
