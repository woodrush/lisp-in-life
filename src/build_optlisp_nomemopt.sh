lisp_opt_qftasm=./src/lisp_opt.qftasm

target=./src/lisp_opt_nomemopt.qftasm

ramdump_stack_csv_headlines=180

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790


#================================================================
# Pass 1
#================================================================
echo "Pass 1: Obtain the optimized ROM"

../elvm/out/8cc -S -DQFT -I. -Isrc -o tmp_.eir src/lisp.c

cat ./src/memheader.eir > tmp.eir
echo "" >> tmp.eir
cat tmp_.eir >> tmp.eir
../elvm/out/elc -qftasm \
  --qftasm-stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --qftasm-stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
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
python ../elvm/tools/qftasm/qftasm_pp.py $final > $target
echo "Done."
echo "Created ${target}."
wc -l $target
