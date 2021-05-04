gcc src/lisp.c -Isrc -DGCC -Wall && echo 1 | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print 1)" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print (define n 17))" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(define n 15)(print n)" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print (- (+ 1 17) 1))" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print (- 27 1 1)" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print (* 27 2 1)" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(define n 158)(print n)" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(define n 15)(print (+ 1 n))" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print ((lambda (n) (+ 3 n)) 5))" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print (< 1 5))" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print (> 1 5))" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && cat print.lisp | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && cat fact.lisp | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && cat primes.lisp | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && cat objects.lisp | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print (lambda () 1))" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print ((lambda () 1)))" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print ())" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print (quote ()))" | ./a.out
echo ""
gcc src/lisp.c -Isrc -DGCC && echo "(print (atom (quote (1 2 3))))" | ./a.out