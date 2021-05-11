ramdump_heap_csv=./src/ramdump_heap.csv
ramdump_stack_csv=./src/ramdump_stack.csv
ramdump_csv=./src/ramdump.csv
lisp_opt_qftasm=./src/lisp_opt.qftasm

#================================================================
# Pass 1
#================================================================
echo "Pass 1: Obtain the ramdump for the precalculations"
../elvm/out/8cc -S -DQFT -Dprecalculation_run -I. -Isrc -o tmp_.eir src/lisp.c

cat ./src/memheader.eir > tmp.eir
echo "" >> tmp.eir
cat tmp_.eir >> tmp.eir
../elvm/out/elc -qftasm \
  --qftasm-stdin-pos 350 \
  --qftasm-stdout-pos 799 \
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
  --qftasm-stdin-pos 350 \
  --qftasm-stdout-pos 799 \
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

echo "Running the qftasm preprocessor.."
python ../elvm/tools/qftasm/qftasm_pp.py $final > lisp_opt_tmp.qftasm
echo "Done."
echo "Created lisp_opt_tmp.qftasm."
wc -l lisp_opt_tmp.qftasm


#================================================================
# Pass 3
#================================================================
echo "Pass 3: Obtain the heap memory and register initialization settings"
target=lisp_opt_tmp.qftasm
initline=$(grep -E '^[0-9]+\..*Register initialization \(stdin buffer pointer\)' $target | grep -oE '^[0-9]+')
initline=$(expr 1 + $initline)
tail -n +$initline $target | sed -E 's/^[0-9]+\. MNZ [0-9]+ ([0-9]+) ([0-9]+);.*/\2,\1/g' > $ramdump_heap_csv

echo "Created ${ramdump_stack_csv}."

# Omit the function call stack
cat $ramdump_heap_csv <(head -n 180 $ramdump_stack_csv) > $ramdump_csv

echo "Created ${ramdump_csv}."


#================================================================
# Pass 4
#================================================================
echo "Pass 4: Omit the heap and register initialization settings and create $lisp_opt_qftasm"
head -n $initline lisp_opt_tmp.qftasm > $lisp_opt_qftasm

echo "Created $lisp_opt_qftasm."


#================================================================
# Run the final code
#================================================================
echo "(print (lambda (n) 1))" | python ../elvm/tools/qftasm/qftasm_interpreter.py \
  --initial-ramvalues $ramdump_csv \
  --stdin-pos 350 \
  --stdout-pos 799 \
  --stack-size 223 \
  --debug-ramdump \
  --debug-plot-memdist \
  -i $lisp_opt_qftasm \

# TODO: Use XOR for not



cat print.lisp     | python ../elvm/tools/qftasm/qftasm_interpreter.py -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --debug-ramdump \
  --suppress-address-overflow-warning \

cat objects.lisp   | python ../elvm/tools/qftasm/qftasm_interpreter.py -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --debug-ramdump \
  --suppress-address-overflow-warning \

cat backquote.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --debug-ramdump \
  --suppress-address-overflow-warning \

cat fact.lisp      | python ../elvm/tools/qftasm/qftasm_interpreter.py -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --debug-ramdump \
  --suppress-address-overflow-warning \

cat primes.lisp    | python ../elvm/tools/qftasm/qftasm_interpreter.py -i $lisp_opt_qftasm \
  --initial-ramvalues $ramdump_csv \
  --debug-ramdump \
  --suppress-address-overflow-warning \
