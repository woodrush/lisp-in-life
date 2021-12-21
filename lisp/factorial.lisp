(define fact (lambda (n)
  (if (eq n 0) 1 (* n (fact (- n 1))))))

(print (fact 5))
