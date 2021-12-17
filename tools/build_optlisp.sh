#!/bin/bash
set -e

lisp_src=./src/lisp.c
ramdump_stack_csv_headlines=180
memheader_eir=./src/memheader.eir
lisp_opt_qftasm=./out/lisp.qftasm

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790

EIGHTCC=./elvm/out/8cc
ELC=./elvm/out/elc
QFTASM_PP=./elvm/tools/qftasm/qftasm_pp.py
QFTASM_INTERPRETER=./tools/qftasmi.sh
OPTIMIZE_QFTASMPP=./tools/optimize_qftasmpp.sh
QFTASMC=./tools/qftasmc.sh

#================================================================
ramdump_heap_csv=./build/ramdump_heap.csv
ramdump_stack_csv=./build/ramdump_stack.csv
ramdump_csv=./out/ramdump.csv

tmp_eir=./build/tmp.eir
tmp2_eir=./build/tmp2.eir
tmp_qftasm=./build/tmp.qftasm
tmp_qftasmpp=./build/tmp.qftasmpp
opt_qftasmpp=./build/opt.qftasmpp

target=./build/lisp_opt_tmp.qftasm

#================================================================
# Step 1
#================================================================
mkdir -p build
mkdir -p out

echo "Step 1: Obtain the ramdump for the precalculations"

$QFTASMC \
    -i $lisp_src \
    -o $tmp_qftasm \
    -8 "-DQFT -Dprecalculation_run -Isrc" \
    -e "" \
    -h ./src/memheader.eir

echo "Running the compiled qftasm to obtain the ramdump for the string hash table precalculations..."
$QFTASM_INTERPRETER -i $tmp_qftasm -u "" -m "" -p "\
  --debug-ramdump-verbose \
  --suppress-stdout \
  --suppress-address-overflow-warning \
  --debug-ramdump-verbose" > $ramdump_stack_csv
echo "Done."

echo "Created ${ramdump_stack_csv}."
echo ""

#================================================================
# Step 2
#================================================================
echo "Step 2: Obtain the optimized ROM, with the memory initializations at the footer"

$QFTASMC \
    -i $lisp_src \
    -o $target \
    -8 "-DQFT -Dskip_precalculation -Isrc" \
    -e "--qftasm-memory-at-footer" \
    -h ./src/memheader.eir \
    -p

echo "Created ${target}."
wc -l $target
echo ""

#================================================================
# Step 3
#================================================================
echo "Step 3: Obtain the heap memory and register initialization settings"
initline=$(grep -E '^[0-9]+\..*Register initialization \(stdin buffer pointer\)' $target | grep -oE '^[0-9]+')
initline=$(expr 1 + $initline)
tail -n +$initline $target | sed -E 's/^[0-9]+\. MNZ [0-9]+ ([0-9]+) ([0-9]+);.*/\2,\1/g' > $ramdump_heap_csv

echo "Created ${ramdump_heap_csv}."

# Omit the function call stack
cat $ramdump_heap_csv <(echo "") <(head -n $ramdump_stack_csv_headlines $ramdump_stack_csv) > $ramdump_csv
sed '/^$/d' $ramdump_csv > ${ramdump_csv}.tmp
mv ${ramdump_csv}.tmp $ramdump_csv

echo "Created ${ramdump_csv}."
echo ""

#================================================================
# Step 4
#================================================================
echo "Step 4: Omit the heap and register initialization settings and create $lisp_opt_qftasm"
head -n $(expr $initline - 1) $target > $lisp_opt_qftasm

echo "Created $lisp_opt_qftasm."
