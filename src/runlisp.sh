../elvm/out/8cc -S -DELVM -I. -I../elvm/libc -Iout -Isrc -o tmp_.eir src/lisp.c

# ../elvm/out/8cc -S -DELVM -Dmemdumpopt1 -I. -I../elvm/libc -Iout -Isrc -o tmp_.eir src/lisp.c
# ../elvm/out/8cc -S -DELVM -Dmemdumpopt2 -I. -I../elvm/libc -Iout -Isrc -o tmp_.eir src/lisp.c
cat ./src/memheader.eir > tmp.eir
echo "" >> tmp.eir
cat tmp_.eir >> tmp.eir
../elvm/out/elc -qftasm tmp.eir > tmp.qftasmpp   # elc outputs code that requires post-processing

wc -l tmp.qftasmpp

python ../elvm/tools/qftasm/qftasm_pp.py tmp.qftasmpp > lisp.qftasm


wc -l lisp.qftasm

echo 1 | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
cat print.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm

# echo 1 | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm > ramdump_1.txt
# cat print.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm > ramdump_print.txt

# echo 1 | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm > ramdump_1_2.txt
# cat print.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm > ramdump_print_2.txt

# echo 1 | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm > ramdump_1_3.txt
# cat print.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm > ramdump_print_3.txt

cat fact.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# cat primes.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# cat objects.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
