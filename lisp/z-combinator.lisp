(print
  (((lambda (f)
      ((lambda (x) (f (lambda (v) ((x x) v))))
       (lambda (x) (f (lambda (v) ((x x) v))))))
    (lambda (fact)
      (lambda (n)
        (if (eq n 0) 1 (* n (fact (- n 1)))))))
   5))
