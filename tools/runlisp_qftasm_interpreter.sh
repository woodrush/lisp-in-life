#!/bin/bash
set -e

function show_usage_exit() {
    echo "Usage: ./runlisp_qftasm_interpreter.sh -i input-file.qftasm [-m ramdump.csv] [-p \"interpreter options\"]
    -i: Input qftasm file
    -m: Memory initialization values to be used before execution
    -p: Interpreter options to pass to ./elvm/tools/qftasm/qftasm_interpreter.py (wrap in \"\" if there are multiple options)" 1>&2;
    exit 1;
}

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790
QFTASM_STACK_SIZE=233

lisp_qftasm="./out/lisp.qftasm"
ramdump_csv="./out/ramdump.csv"
interpreter_options=""

while getopts ":i:m:p:" o; do
    case "${o}" in
        i)
            lisp_qftasm=${OPTARG}
            ;;
        m)
            ramdump_csv=${OPTARG}
            if [ -f "$ramdump_csv" ]; then
                ramdump_args="--initial-ramvalues ${ramdump_csv}"
            fi
            ;;
        p)
            interpreter_options=${OPTARG}
            ;;
        *)
            show_usage_exit
            ;;
    esac
done
shift $((OPTIND-1))


python ./elvm/tools/qftasm/qftasm_interpreter.py \
    -i $lisp_qftasm \
    --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
    --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
    --stack-size $QFTASM_STACK_SIZE \
    $interpreter_options \
    $ramdump_args \
    $@ < /dev/stdin
