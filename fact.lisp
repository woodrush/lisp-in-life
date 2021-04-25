(define Z (closure (f) (
  (closure (x) (f (closure (v) ((x x) v))))
  (closure (x) (f (closure (v) ((x x) v)))))))
 
(define fact (Z (closure (fact_)
  (closure (n) (if (eq n 0) 1 (* n (fact_ (- n 1))))))))
 
(print (fact 5))