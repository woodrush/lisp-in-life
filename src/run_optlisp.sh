../elvm/out/8cc -S -DELVM -I. -Ilibc -Iout -Isrc -o tmp_.eir src/lisp.c
echo "" > tmp.eir
cat ./src/memheader.eir >> tmp.eir
cat tmp_.eir >> tmp.eir
../elvm/out/elc -qftasm tmp.eir > tmp.qftasmpp   # elc outputs code that requires post-processing

# wc -l tmp.qftasmpp
python ./src/qftasmopt.py tmp.qftasmpp > opt.qftasmpp    && wc -l opt.qftasmpp
python ./src/qftasmopt.py opt.qftasmpp > opt2.qftasmpp   && wc -l opt2.qftasmpp
python ./src/qftasmopt.py opt2.qftasmpp > opt3.qftasmpp  && wc -l opt3.qftasmpp
python ./src/qftasmopt.py opt3.qftasmpp > opt4.qftasmpp  && wc -l opt4.qftasmpp
python ./src/qftasmopt.py opt4.qftasmpp > opt5.qftasmpp  && wc -l opt5.qftasmpp
python ./src/qftasmopt.py opt5.qftasmpp > opt6.qftasmpp  && wc -l opt6.qftasmpp
python ./src/qftasmopt.py opt6.qftasmpp > opt7.qftasmpp  && wc -l opt7.qftasmpp
python ./src/qftasmopt.py opt7.qftasmpp > opt8.qftasmpp  && wc -l opt8.qftasmpp
python ./src/qftasmopt.py opt8.qftasmpp > opt9.qftasmpp  && wc -l opt9.qftasmpp
diff opt8.qftasmpp opt9.qftasmpp | wc -l

# python ./src/qftasmopt.py opt6.qftasmpp > opt7.qftasmpp  && wc -l opt7.qftasmpp
# diff opt6.qftasmpp opt7.qftasmpp | wc -l
# python ./src/qftasmopt.py opt7.qftasmpp > opt8.qftasmpp  && wc -l opt8.qftasmpp
# python ./src/qftasmopt.py opt8.qftasmpp > opt9.qftasmpp  && wc -l opt9.qftasmpp

python ../elvm/tools/qftasm/qftasm_pp.py opt7.qftasmpp > lisp_opt.qftasm

# # # wc -l lisp_opt.qftasm

cat print.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp_opt.qftasm
cat fact.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp_opt.qftasm
cat objects.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp_opt.qftasm
cat primes.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp_opt.qftasm
