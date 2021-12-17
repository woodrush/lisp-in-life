#!/bin/bash
set -e

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790
QFTASM_STACK_SIZE=233

lisp_qftasm="./out/lisp.qftasm"
ramdump_csv="./out/ramdump.csv"

python ./elvm/tools/qftasm/qftasm_interpreter.py \
    -i $lisp_qftasm \
    --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
    --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
    --stack-size $QFTASM_STACK_SIZE \
    --initial-ramvalues $ramdump_csv \
    $@ < /dev/stdin
