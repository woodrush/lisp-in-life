ramdump_heap_csv=./src/ramdump_heap.csv
ramdump_stack_csv=./src/ramdump_stack.csv
ramdump_csv=./src/ramdump.csv
lisp_opt_qftasm=./src/lisp_opt.qftasm

ramdump_stack_csv_headlines=180

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790

#================================================================
# Pass 1
#================================================================
echo "Pass 1: Obtain the ramdump for the precalculations"
../elvm/out/8cc -S -DQFT -Dprecalculation_run -I. -Isrc -o tmp_.eir src/lisp.c

cat ./src/memheader.eir > tmp.eir
echo "" >> tmp.eir
cat tmp_.eir >> tmp.eir
../elvm/out/elc -qftasm \
  --qftasm-stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --qftasm-stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  tmp.eir > tmp.qftasmpp

python ../elvm/tools/qftasm/qftasm_pp.py tmp.qftasmpp > tmp.qftasm

echo "" | python ../elvm/tools/qftasm/qftasm_interpreter.py -i tmp.qftasm \
  --debug-ramdump-verbose \
  --suppress-stdout \
  --suppress-address-overflow-warning \
  --debug-ramdump-verbose > $ramdump_stack_csv

echo "Created ${ramdump_stack_csv}."


#================================================================
# Pass 2
#================================================================
echo "Pass 2: Obtain the optimized ROM, with the memory initializations at the footer"

../elvm/out/8cc -S -DQFT -Dskip_precalculation -I. -Isrc -o tmp_.eir src/lisp.c

cat ./src/memheader.eir > tmp.eir
echo "" >> tmp.eir
cat tmp_.eir >> tmp.eir
../elvm/out/elc -qftasm \
  --qftasm-stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --qftasm-stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  --qftasm-memory-at-footer \
  tmp.eir > tmp.qftasmpp


echo "Running compiler optimizations on tmp.qftasmpp..."

wc -l tmp.qftasmpp
python ./src/qftasmopt.py tmp.qftasmpp > opt.qftasmpp    && wc -l opt.qftasmpp
python ./src/qftasmopt.py opt.qftasmpp > opt2.qftasmpp   && wc -l opt2.qftasmpp
python ./src/qftasmopt.py opt2.qftasmpp > opt3.qftasmpp  && wc -l opt3.qftasmpp
python ./src/qftasmopt.py opt3.qftasmpp > opt4.qftasmpp  && wc -l opt4.qftasmpp
python ./src/qftasmopt.py opt4.qftasmpp > opt5.qftasmpp  && wc -l opt5.qftasmpp
python ./src/qftasmopt.py opt5.qftasmpp > opt6.qftasmpp  && wc -l opt6.qftasmpp
python ./src/qftasmopt.py opt6.qftasmpp > opt7.qftasmpp  && wc -l opt7.qftasmpp
python ./src/qftasmopt.py opt7.qftasmpp > opt8.qftasmpp  && wc -l opt8.qftasmpp
python ./src/qftasmopt.py opt8.qftasmpp > opt9.qftasmpp  && wc -l opt9.qftasmpp

final1=opt8.qftasmpp
final2=opt9.qftasmpp
if [ $(diff $final1 $final2 | wc -l) -ne 0 ]; then
    echo "Files ${final1} and ${final2} do not match!"
    exit
else
    echo "Files ${final1} and ${final2} match."
fi
echo "Done."

# final=tmp.qftasmpp
final=opt9.qftasmpp
# final=opt2.qftasmpp

target=lisp_opt_tmp.qftasm

echo "Running the qftasm preprocessor.."
python ../elvm/tools/qftasm/qftasm_pp.py $final > $target
echo "Done."
echo "Created ${target}."
wc -l $target


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


#================================================================
# Pass 4
#================================================================
echo "Pass 4: Omit the heap and register initialization settings and create $lisp_opt_qftasm"
head -n $(expr $initline - 1) $target > $lisp_opt_qftasm

echo "Created $lisp_opt_qftasm."
