#!/bin/bash
set -e

function show_usage_exit() {
    echo "Usage: ./qftasm.sh -i input-file.c -o output-file.qftasm [-h memheader-file.eir] [-8 \"8cc options\"] [-e \"elc options\"] [-p]
    -i: Input C source file
    -o: Output qftasm file
    -h: Memory header EIR file (optional)
    -8: 8cc options (wrap in \"\" if there are multiple options)
    -e: elc options (wrap in \"\" if there are multiple options)
    -p: Use the QFTASM optimizer (./tools/optimize_qftasmpp.sh)" 1>&2;
    exit 1;
}

c_src=""
eightcc_opts=""
elc_opts=""
memheader_eir=""
target="a.qftasm"
qftasm_optimize=""

while getopts ":i:o:h:8:e:p" o; do
    case "${o}" in
        i)
            c_src=${OPTARG}
            ;;
        o)
            target=${OPTARG}
            ;;
        h)
            memheader_eir=${OPTARG}
            ;;
        8)
            eightcc_opts=${OPTARG}
            ;;
        e)
            elc_opts=${OPTARG}
            ;;
        p)
            qftasm_optimize=true
            ;;
        *)
            show_usage_exit
            ;;
    esac
done
shift $((OPTIND-1))

echo "qftasmc options:"
echo "i: $c_src"
echo "o: $target"
echo "h: $memheader_eir"
echo "8: $eightcc_opts"
echo "e: $elc_opts"
echo "p: $qftasm_optimize"
echo ""

if [ ! -f "$c_src" ]; then
    show_usage_exit
fi


compiled_eir=./build/compiled.eir
linked_eir=./build/linked.eir
linked_eir_qftasmpp=./build/linked.eir.qftasmpp
optimized_qftasmpp=./build/optimized.qftasmpp


QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790


EIGHTCC=./elvm/out/8cc
ELC=./elvm/out/elc
QFTASM_PP=./elvm/tools/qftasm/qftasm_pp.py
QFTASM_INTERPRETER=./tools/runlisp_qftasm_interpreter.sh
OPTIMIZE_QFTASMPP=./tools/optimize_qftasmpp.sh


echo "Compiling the C source to EIR..."
$EIGHTCC -S $eightcc_opts -o $compiled_eir $c_src
echo "Done."

echo "Prepending EIR memory headers..."
echo ".data
	.string \"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\"
" > $linked_eir

if [ -f "$memheader_eir" ]; then
    cat $memheader_eir >> $linked_eir
    echo "" >> $linked_eir
fi
echo "Done."

echo "Compiling the EIR file to qftasmpp..."
cat $compiled_eir >> $linked_eir
$ELC -qftasm \
  --qftasm-stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --qftasm-stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  $elc_opts \
  $linked_eir > $linked_eir_qftasmpp
echo "Done."

if [ "$qftasm_optimize" != "" ]; then
    echo "Running compiler optimizations on ${linked_eir_qftasmpp}..."
    $OPTIMIZE_QFTASMPP $linked_eir_qftasmpp $optimized_qftasmpp

    qftasmpp_source=$optimized_qftasmpp
else
    qftasmpp_source=$linked_eir_qftasmpp
fi

echo "Running the qftasm preprocessor.."
python $QFTASM_PP $qftasmpp_source > $target
echo "Done."

echo "Created ${target}."
