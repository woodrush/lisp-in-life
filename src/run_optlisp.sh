python qftasmopt.py ../elvm/tmp.qftasmpp > opt.qftasmpp
python qftasmopt.py opt.qftasmpp > opt2.qftasmpp
python qftasmopt.py opt2.qftasmpp > opt3.qftasmpp
python ../elvm/tools/qftasm/qftasm_pp.py opt3.qftasmpp > lisp_opt.qftasm
cat primes.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp_opt.qftasm