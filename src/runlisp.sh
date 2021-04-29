../elvm/out/8cc -S -DELVM -I. -I../elvm/libc -Iout -Isrc -o tmp_.eir src/lisp.c
echo "" > tmp.eir
cat ./src/memheader.eir >> tmp.eir
cat tmp_.eir >> tmp.eir
../elvm/out/elc -qftasm tmp.eir > tmp.qftasmpp   # elc outputs code that requires post-processing

# wc -l tmp.qftasmpp

python ../elvm/tools/qftasm/qftasm_pp.py tmp.qftasmpp > lisp.qftasm

wc -l lisp.qftasm

cat print.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
cat fact.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
cat primes.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
cat objects.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
