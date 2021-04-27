(define new (lambda (classname) (classname)))
(define defclass (macro (classname fieldlist body)
  (list (quote define) classname
    (list (quote class) fieldlist body))))
(define . (macro (object methodname)
  (list object (list (quote quote) methodname))))
(define defmethod (macro (methodname arglist body)
  (list (quote define) (quote methodlist)
        (list (quote cons)
              (list (quote list)
                    (list (quote quote) methodname)
                    (list (quote lambda) arglist body))
              (quote methodlist)))))
(define class (macro (fieldlist body)
  (list (quote lambda) (quote ())
    (list (list (quote closure) (list (quote methodlist))
      (list (quote progn)
        (list (list (quote closure) fieldlist body))
        (quote
          (lambda (targetmethod)
            ((lambda (methodlist curitem curname ret)
              (progn
                (while methodlist
                  (progn
                    (define curitem (car methodlist))
                    (define curname (car curitem))
                    (if (eq curname targetmethod)
                      (progn
                        (define methodlist ())
                        (define ret (car (cdr curitem))))
                      ())
                    (define methodlist (cdr methodlist))))
                ret)
            ) methodlist) ))
            
              ))) )))

(defclass counter (n)
  (progn
    (define n 0)
    (defmethod inc ()
      (define n (+ 1 n)))
    (defmethod get ()
      n)
    (defmethod set (m)
      (define n m))))

(define counter1 (new counter))
(define counter2 (new counter))

((. counter2 set) 12)

(print ((. counter1 get)) ())
(print ((. counter2 get)) ())

(while (< ((. counter1 get)) 2)
  (progn
    ((. counter1 inc))
    ((. counter2 inc))
    ((. counter2 inc))))

(print ((. counter1 get)) ())
(print ((. counter2 get)) ())