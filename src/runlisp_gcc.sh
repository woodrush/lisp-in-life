gcc src/lisp.c -Isrc -DGCC && cat print.lisp | ./a.out
gcc src/lisp.c -Isrc -DGCC && cat fact.lisp | ./a.out
gcc src/lisp.c -Isrc -DGCC && cat primes.lisp | ./a.out
gcc src/lisp.c -Isrc -DGCC && cat objects.lisp | ./a.out