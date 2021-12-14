#!/usr/local/bin/bash
set -e

lisp_src=./src/lisp.c
ramdump_stack_csv_headlines=180
memheader_eir=./src/memheader.eir
lisp_opt_qftasm=./out/lisp_opt.qftasm

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790

EIGHTCC=./elvm/out/8cc
ELC=./elvm/out/elc
QFTASM_PP=./elvm/tools/qftasm/qftasm_pp.py
QFTASM_INTERPRETER=./elvm/tools/qftasm/qftasm_interpreter.py


#================================================================
ramdump_heap_csv=./build/ramdump_heap.csv
ramdump_stack_csv=./build/ramdump_stack.csv
ramdump_csv=./build/ramdump.csv

tmp_eir=./build/tmp.eir
tmp2_eir=./build/tmp2.eir
tmp_qftasmpp=./build/tmp.qftasmpp

final1=./build/opt8.qftasmpp
final2=./build/opt9.qftasmpp
final=./build/opt9.qftasmpp
target=./build/lisp_opt_tmp.qftasm

#================================================================
# Pass 1
#================================================================
mkdir -p build
mkdir -p out

echo "Pass 1: Obtain the ramdump for the precalculations"
$EIGHTCC -S -DQFT -Dprecalculation_run -Isrc -o $tmp2_eir $lisp_src

cat ./src/memheader.eir > $tmp_eir
echo "" >> $tmp_eir
cat $tmp2_eir >> $tmp_eir
$ELC -qftasm \
  --qftasm-stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --qftasm-stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  $tmp_eir > $tmp_qftasmpp

python $QFTASM_PP $tmp_qftasmpp > tmp.qftasm

echo "" | python $QFTASM_INTERPRETER -i tmp.qftasm \
  --debug-ramdump-verbose \
  --suppress-stdout \
  --suppress-address-overflow-warning \
  --debug-ramdump-verbose > $ramdump_stack_csv

echo "Created ${ramdump_stack_csv}."
echo ""

#================================================================
# Pass 2
#================================================================
echo "Pass 2: Obtain the optimized ROM, with the memory initializations at the footer"

$EIGHTCC -S -DQFT -Dskip_precalculation -Isrc -o $tmp2_eir $lisp_src

cat ./src/memheader.eir > $tmp_eir
echo "" >> $tmp_eir
cat $tmp2_eir >> $tmp_eir
$ELC -qftasm \
  --qftasm-stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --qftasm-stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --qftasm-memory-at-footer \
  $tmp_eir > $tmp_qftasmpp


echo "Running compiler optimizations on ${tmp_qftasmpp}..."

wc -l $tmp_qftasmpp
python ./src/qftasmopt.py $tmp_qftasmpp         > ./build/opt.qftasmpp   && wc -l ./build/opt.qftasmpp
python ./src/qftasmopt.py ./build/opt.qftasmpp  > ./build/opt2.qftasmpp  && wc -l ./build/opt2.qftasmpp
python ./src/qftasmopt.py ./build/opt2.qftasmpp > ./build/opt3.qftasmpp  && wc -l ./build/opt3.qftasmpp
python ./src/qftasmopt.py ./build/opt3.qftasmpp > ./build/opt4.qftasmpp  && wc -l ./build/opt4.qftasmpp
python ./src/qftasmopt.py ./build/opt4.qftasmpp > ./build/opt5.qftasmpp  && wc -l ./build/opt5.qftasmpp
python ./src/qftasmopt.py ./build/opt5.qftasmpp > ./build/opt6.qftasmpp  && wc -l ./build/opt6.qftasmpp
python ./src/qftasmopt.py ./build/opt6.qftasmpp > ./build/opt7.qftasmpp  && wc -l ./build/opt7.qftasmpp
python ./src/qftasmopt.py ./build/opt7.qftasmpp > ./build/opt8.qftasmpp  && wc -l ./build/opt8.qftasmpp
python ./src/qftasmopt.py ./build/opt8.qftasmpp > ./build/opt9.qftasmpp  && wc -l ./build/opt9.qftasmpp

if [ $(diff $final1 $final2 | wc -l) -ne 0 ]; then
    echo "Files ${final1} and ${final2} do not match!"
    exit
else
    echo "Files ${final1} and ${final2} match."
fi
echo "Done."

echo "Running the qftasm preprocessor.."
python $QFTASM_PP $final > $target
echo "Done."
echo "Created ${target}."
wc -l $target
echo ""

#================================================================
# Pass 3
#================================================================
echo "Pass 3: Obtain the heap memory and register initialization settings"
initline=$(grep -E '^[0-9]+\..*Register initialization \(stdin buffer pointer\)' $target | grep -oE '^[0-9]+')
initline=$(expr 1 + $initline)
tail -n +$initline $target | sed -E 's/^[0-9]+\. MNZ [0-9]+ ([0-9]+) ([0-9]+);.*/\2,\1/g' > $ramdump_heap_csv

echo "Created ${ramdump_stack_csv}."

# Omit the function call stack
cat $ramdump_heap_csv <(head -n $ramdump_stack_csv_headlines $ramdump_stack_csv) > $ramdump_csv

echo "Created ${ramdump_csv}."
echo ""

#================================================================
# Pass 4
#================================================================
echo "Pass 4: Omit the heap and register initialization settings and create $lisp_opt_qftasm"
head -n $(expr $initline - 1) $target > $lisp_opt_qftasm

echo "Created $lisp_opt_qftasm."
echo ""
