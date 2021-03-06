#!/bin/bash
set -e

function show_usage_exit() {
    echo "Usage: ./qftasmi.sh -i input-file.qftasm [-u string] [-m ramdump.csv] [-n int] [-t int] [-s int] -p \"interpreter options\"]
    -i: Input qftasm file
    -u: Standard input to provide to the QFTASM program (optional)
    -m: Memory initialization values to be used before execution (optional)
    -n: QFTASM_RAMSTDIN_BUF_STARTPOSITION (memory address of the beginning of the standard input)
    -t: QFTASM_RAMSTDOUT_BUF_STARTPOSITION (memory address of the beginning of the standard output)
    -s: QFTASM_STACK_SIZE (the size of the stack region)
    -p: Interpreter options to pass to ./elvm/tools/qftasm/qftasm_interpreter.py (wrap in \"\" if there are multiple options)" 1>&2;
    exit 1;
}

input_qftasm=""
qftasm_stdin=""
ramdump_csv=""
ramdump_args=""
QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790
QFTASM_STACK_SIZE=233
interpreter_options=""

while getopts ":i:u:m:n:t:s:p:" o; do
    case "${o}" in
        i)
            input_qftasm=${OPTARG}
            ;;
        u)
            qftasm_stdin=${OPTARG}
            ;;
        m)
            ramdump_csv=${OPTARG}
            ;;
        n)
            QFTASM_RAMSTDIN_BUF_STARTPOSITION=${OPTARG}
            ;;
        t)
            QFTASM_RAMSTDOUT_BUF_STARTPOSITION=${OPTARG}
            ;;
        s)
            QFTASM_STACK_SIZE=${OPTARG}
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

if [ ! -f "$input_qftasm" ]; then
    show_usage_exit
fi

if [ -f "$ramdump_csv" ]; then
    ramdump_args="--initial-ramvalues ${ramdump_csv}"
fi


echo "${qftasm_stdin}" | python ./elvm/tools/qftasm/qftasm_interpreter.py \
    -i $input_qftasm \
    --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
    --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
    --stack-size $QFTASM_STACK_SIZE \
    $ramdump_args \
    $interpreter_options
