;; (define Z (lambda (f) (
;;   (lambda (x) (f (lambda (v) ((x x) v))))
;;   (lambda (x) (f (lambda (v) ((x x) v)))))) )
 
;; (define fact (Z (closure (fact_)
;;   (lambda (n) (if (eq n 0) 1 (* n (fact_ (- n 1)))))) ))
 
;; (print (fact 5))


(print
  (((lambda (f)
      ((lambda (x) (f (lambda (v) ((x x) v))))
       (lambda (x) (f (lambda (v) ((x x) v))))))
    (lambda (fact)
      (lambda (n)
        (if (eq n 0) 1 (* n (fact (- n 1)))))))
   5))