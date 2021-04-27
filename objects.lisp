(define defmethod (macro (methodname body)
  (list (quote if)
        (list (quote eq) (quote methodname) (list (quote quote) methodname))
        body ())))
(define callmethod (macro (object methodname arg)
  (list object (list (quote quote) methodname) arg)))

(define newcounter (lambda (m)
  ((closure (n)
    (lambda (methodname arg)
      (progn
      (defmethod inc
        (define n (+ n 1)))
      (defmethod dec
        (define n (- n 1)))
      (defmethod print
        (print n ()))
      (defmethod set
        (define n arg))
      (defmethod get
        n)) ))
    (if m m 0))))
(print ())
(print (quote ()))
(define counter1 (newcounter))
(define counter2 (newcounter 5))
(callmethod counter1 print)
(callmethod counter1 inc)
(callmethod counter1 inc)
(callmethod counter1 inc)
(callmethod counter1 dec)
(callmethod counter1 print)
