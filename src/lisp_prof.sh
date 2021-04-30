for code in print fact primes objects; do
    gcc src/lisp.c -Isrc -DGCC -pg
    cat $code.lisp | ./a.out
    gprof a.out gmon.out > prof_output_$code
done
