if [ $# -eq 0 ]; then
    lisp_opt_qftasm=./src/lisp_opt.qftasm
else
    lisp_opt_qftasm=$1
fi

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790
QFTASM_STACK_SIZE=233

../elvm/out/8cc -S -DQFT -Isrc -o tmp_.eir src/lisp.c

cat ./src/memheader.eir > tmp.eir
echo "" >> tmp.eir
cat tmp_.eir >> tmp.eir
../elvm/out/elc -qftasm \
  --qftasm-stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --qftasm-stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  tmp.eir > tmp.qftasmpp

wc -l tmp.qftasmpp

python ../elvm/tools/qftasm/qftasm_pp.py tmp.qftasmpp > lisp.qftasm

wc -l lisp.qftasm
