../elvm/out/8cc -S -DELVM -I. -Ilibc -Iout -Isrc -o tmp.eir src/lisp.c
../elvm/out/elc -qftasm tmp.eir > tmp.qftasmpp   # elc outputs code that requires post-processing

wc -l tmp.qftasmpp
python ./src/qftasmopt.py tmp.qftasmpp > opt.qftasmpp    && wc -l opt.qftasmpp
python ./src/qftasmopt.py opt.qftasmpp > opt2.qftasmpp   && wc -l opt2.qftasmpp
python ./src/qftasmopt.py opt2.qftasmpp > opt3.qftasmpp  && wc -l opt3.qftasmpp
python ./src/qftasmopt.py opt3.qftasmpp > opt4.qftasmpp  && wc -l opt4.qftasmpp
python ./src/qftasmopt.py opt4.qftasmpp > opt5.qftasmpp  && wc -l opt5.qftasmpp
python ./src/qftasmopt.py opt5.qftasmpp > opt6.qftasmpp  && wc -l opt6.qftasmpp

python ../elvm/tools/qftasm/qftasm_pp.py opt6.qftasmpp > lisp_opt.qftasm

cat print.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp_opt.qftasm
cat primes.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp_opt.qftasm
cat fact.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp_opt.qftasm



# python qftasmopt.py ../elvm/tmp.qftasmpp > opt.qftasmpp
# python qftasmopt.py opt.qftasmpp > opt2.qftasmpp
# python qftasmopt.py opt2.qftasmpp > opt3.qftasmpp
# python ../elvm/tools/qftasm/qftasm_pp.py opt3.qftasmpp > lisp_opt.qftasm
