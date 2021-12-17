#!/bin/bash
set -e

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790
QFTASM_STACK_SIZE=233

lisp_qftasm="./out/lisp.qftasm"
ramdump_csv="./out/ramdump.csv"

# if [ ! -f $lisp_qftasm ] || [ ! -f $ramdump_csv ]; then
#     echo "File ${lisp_qftasm} or ${ramdump_csv} not found. Run \`make all\` to obtain these files."
#     exit 1
# fi

code=$1

shift;

echo "${code}" | python ./elvm/tools/qftasm/qftasm_interpreter.py \
    -i $lisp_qftasm \
    --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
    --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
    --stack-size $QFTASM_STACK_SIZE \
    --initial-ramvalues $ramdump_csv \
    $@
