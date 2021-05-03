gcc src/lisp.c -Isrc -DGCC -Wall && echo 1 | ./a.out
gcc src/lisp.c -Isrc -DGCC && cat print.lisp | ./a.out
gcc src/lisp.c -Isrc -DGCC && cat fact.lisp | ./a.out
gcc src/lisp.c -Isrc -DGCC && cat primes.lisp | ./a.out
gcc src/lisp.c -Isrc -DGCC && cat objects.lisp | ./a.out
gcc src/lisp.c -Isrc -DGCC && echo "(print (lambda () 1))" | ./a.out
gcc src/lisp.c -Isrc -DGCC && echo "(print ())" | ./a.out
gcc src/lisp.c -Isrc -DGCC && echo "(print (quote ()))" | ./a.out
gcc src/lisp.c -Isrc -DGCC && echo "(print (atom (quote (1 2 3))))" | ./a.out