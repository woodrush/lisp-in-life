for code in print fact primes objects; do
    gcc src/lisp.c -Isrc -DGCC -pg -g
    cat $code.lisp | ./a.out
    gprof a.out gmon.out -A > prof_output_$code
done
