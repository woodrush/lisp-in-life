if [ $# -eq 0 ]; then
    lisp_opt_qftasm=./src/lisp_opt.qftasm
else
    lisp_opt_qftasm=$1
fi

ramdump_csv=./src/ramdump.csv

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790
QFTASM_STACK_SIZE=233


../elvm/out/8cc -S -DQFT -Isrc -o tmp_.eir src/lisp.c

cat ./src/memheader.eir > tmp.eir
echo "" >> tmp.eir
cat tmp_.eir >> tmp.eir
../elvm/out/elc -qftasm \
  --qftasm-stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
  --qftasm-stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
  tmp.eir > tmp.qftasmpp   # elc outputs code that requires post-processing

wc -l tmp.qftasmpp

python ../elvm/tools/qftasm/qftasm_pp.py tmp.qftasmpp > lisp.qftasm

wc -l lisp.qftasm

function run_and_compare_with_hy () {
    echo "$1"
    echo "$1" | python ../elvm/tools/qftasm/qftasm_interpreter.py \
    -i lisp.qftasm \
    --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
    --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
    --stack-size $QFTASM_STACK_SIZE
    echo ""

    hy -c "$1"
}

run_and_compare_with_hy "(print (< -5 0))" 
run_and_compare_with_hy "(print (< -5 -2))"
run_and_compare_with_hy "(print (< -2 -5))"
run_and_compare_with_hy "(print (< -2 0))" 
run_and_compare_with_hy "(print (< 0 -2))" 
run_and_compare_with_hy "(print (> -5 0))" 
run_and_compare_with_hy "(print (> -5 -2))"
run_and_compare_with_hy "(print (> -2 -5))"
run_and_compare_with_hy "(print (> -2 0))" 
run_and_compare_with_hy "(print (> 0 -2))" 
run_and_compare_with_hy "(print (< -5 1))" 
run_and_compare_with_hy "(print (< -5 -2))"
run_and_compare_with_hy "(print (< -2 -5))"
run_and_compare_with_hy "(print (< -2 1))" 
run_and_compare_with_hy "(print (< 1 -2))" 
run_and_compare_with_hy "(print (> -5 1))" 
run_and_compare_with_hy "(print (> -5 -2))"
run_and_compare_with_hy "(print (> -2 -5))"
run_and_compare_with_hy "(print (> -2 1))" 
run_and_compare_with_hy "(print (> 1 -2))" 

echo "===="

run_and_compare_with_hy "(print (lambda (n) 1))"          
run_and_compare_with_hy "(print ((lambda (n) (+ 1 n)) 5))"
run_and_compare_with_hy "(print -10)"        
run_and_compare_with_hy "(print -5)"         
run_and_compare_with_hy "(print -200)"       
run_and_compare_with_hy "(print (- -200))"   
run_and_compare_with_hy "(print (- 190 150))"
run_and_compare_with_hy "(print -2000)"      
run_and_compare_with_hy "(print -4000)"      
run_and_compare_with_hy "(print -8000)"      
run_and_compare_with_hy "(print 2000)"       
run_and_compare_with_hy "(print 4000)"       
run_and_compare_with_hy "(print 8000)"       
run_and_compare_with_hy "(print (+ 8191))"   
run_and_compare_with_hy "(print (+ 8191 1))" 
run_and_compare_with_hy "(print (+ 8191 2))" 
run_and_compare_with_hy "(print (- 8191))"   
run_and_compare_with_hy "(print (- -8191 1))"
run_and_compare_with_hy "(print (- -8191 2))"

run_and_compare_with_hy "(print ())"                    
run_and_compare_with_hy "(print (quote ()))"            
run_and_compare_with_hy "(print (atom (quote (1 2 3))))"


cat print.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py --debug-ramdump -i lisp.qftasm \
  --stdin-pos 350 \
  --stdout-pos 799 \
  --stack-size 223 \

cat fact.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py --debug-ramdump -i lisp.qftasm \
  --stdin-pos 350 \
  --stdout-pos 799 \
  --stack-size 223 \

cat objects.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py --debug-ramdump -i lisp.qftasm \
  --stdin-pos 350 \
  --stdout-pos 799 \
  --stack-size 223 \
#   --debug-plot-memdist \


cat backquote.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py \
  --stdin-pos 350 \
  --stdout-pos 799 \
  --stack-size 223 \
  --debug-ramdump \
  -i lisp.qftasm \
  --debug-plot-memdist \

cat primes.lisp | python ../elvm/tools/qftasm/qftasm_interpreter.py \
  --stdin-pos 350 \
  --stdout-pos 799 \
  --stack-size 223 \
  --debug-ramdump \
  -i lisp.qftasm \
  --debug-plot-memdist \

# echo "(print t)"              | python ../elvm/tools/qftasm/qftasm_interpreter.py -i lisp.qftasm
# echo "(print (eval t))"       | python ../elvm/tools/qftasm/qftasm_interpreter.py -i lisp.qftasm
# echo "(print (eval (eval t)))"       | python ../elvm/tools/qftasm/qftasm_interpreter.py -i lisp.qftasm
