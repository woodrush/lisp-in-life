../elvm/out/8cc -S -DELVM -I. -I../elvm/libc -Iout -Isrc -o tmp_.eir src/lisp.c
cat ./src/memheader.eir > tmp.eir
echo "" >> tmp.eir
cat tmp_.eir >> tmp.eir
../elvm/out/elc -qftasm tmp.eir > tmp.qftasmpp   # elc outputs code that requires post-processing

wc -l tmp.qftasmpp

python ../elvm/tools/qftasm/qftasm_pp.py tmp.qftasmpp > lisp.qftasm


wc -l lisp.qftasm

# echo "(print (< -5 0))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (< -5 0))"
# echo "(print (< -5 -2))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (< -5 -2))"
# echo "(print (< -2 -5))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (< -2 -5))"
# echo "(print (< -2 0))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (< -2 0))"
# echo "(print (< 0 -2))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (< 0 -2))"
# echo "(print (> -5 0))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (> -5 0))"
# echo "(print (> -5 -2))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (> -5 -2))"
# echo "(print (> -2 -5))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (> -2 -5))"
# echo "(print (> -2 0))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (> -2 0))"
# echo "(print (> 0 -2))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (> 0 -2))"
# echo "(print (< -5 1))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (< -5 1))"
# echo "(print (< -5 -2))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (< -5 -2))"
# echo "(print (< -2 -5))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (< -2 -5))"
# echo "(print (< -2 1))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (< -2 1))"
# echo "(print (< 1 -2))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (< 1 -2))"
# echo "(print (> -5 1))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (> -5 1))"
# echo "(print (> -5 -2))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (> -5 -2))"
# echo "(print (> -2 -5))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (> -2 -5))"
# echo "(print (> -2 1))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (> -2 1))"
# echo "(print (> 1 -2))"  | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm && hy -c "(print (> 1 -2))"
# echo "===="
# echo "(print (lambda (n) 1))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print ((lambda (n) (+ 1 n)) 5))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print -5)" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print -10)" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print -200)" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print (- -200))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print (- 190 150))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print -2000)" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print -4000)" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print -8000)" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print 2000)" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print 4000)" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print 8000)" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print (+ 8191))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print (+ 8191 1))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print (+ 8191 2))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print (- 8191))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print (- -8191 1))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print (- -8191 2))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm

# echo "(print ())" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print (quote ()))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# echo "(print (atom (quote (1 2 3))))" | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm

# cat print.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# cat fact.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
# cat objects.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
cat primes.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py lisp.qftasm
