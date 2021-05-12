if [ $# -eq 0 ]; then
    input=./src/lisp_opt.qftasm
else
    input=$1
fi

ramdump_csv=./src/ramdump.csv

QFTASM_RAMSTDIN_BUF_STARTPOSITION=290
QFTASM_RAMSTDOUT_BUF_STARTPOSITION=790
QFTASM_STACK_SIZE=233

function runlisp () {
    echo "$1" | python ../elvm/tools/qftasm/qftasm_interpreter.py \
    -i $input \
    --stdin-pos $QFTASM_RAMSTDIN_BUF_STARTPOSITION \
    --stdout-pos $QFTASM_RAMSTDOUT_BUF_STARTPOSITION \
    --stack-size $QFTASM_STACK_SIZE \
    --initial-ramvalues $ramdump_csv

    if [[ $2 != "-nonewline" ]]; then
        echo ""
    fi
}

function run_and_compare_with_hy () {
    echo "Case ${1}:"
    result_qft=$(runlisp "$1" -nonewline)
    echo $result_qft

    result_hy=$(hy -c "$1")
    echo $result_hy

    if [[ $2 == "-notest" ]]; then
        return;
    fi
    if [[ $result_qft == "t" ]] && [[ $result_hy == "True" ]]; then
        echo "Passed."
    elif [[ $result_qft == "()" ]] && [[ $result_hy == "False" ]]; then
        echo "Passed."
    else
        echo "Test failed!"
        exit
    fi
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

runlisp "(print ())"
runlisp "(print (quote ()))"
runlisp "(print (atom (quote (1 2 3))))"
runlisp "(print (lambda (n) 1))"
runlisp "(print ((lambda (n) (+ 1 n)) 5))"
