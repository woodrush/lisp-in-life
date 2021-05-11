(define counter
  (lambda (n)
    (lambda (methodname)
      (if (eq methodname (quote inc))
        (lambda () (define n (+ n 1)))
      (if (eq methodname (quote dec))
        (lambda () (define n (- n 1)))
      (if (eq methodname (quote get))
        (lambda () n)
      (if (eq methodname (quote set))
        (lambda (m) (define n m))
        ())))))))

(define . (macro* (object methodname) (list object (list (quote quote) methodname))))
(define new (lambda (x) (x)))

(define counter1 (new counter))
(define counter2 (new counter))

((. counter1 set) 0)
((. counter2 set) 8)

;; `(print x t)` prints the value with a newline
(print ((. counter1 inc)) t)
(print ((. counter1 inc)) t)
(print ((. counter1 inc)) t)
(print ((. counter2 inc)) t)
(print ((. counter2 dec)) t)
(print ((. counter1 inc)) t)
(print ((. counter2 inc)) t)