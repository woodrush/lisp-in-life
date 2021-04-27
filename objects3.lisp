(define new (lambda (classname) (classname)))
(define . (macro (object methodname) (list object (list (quote quote) methodname))))
(define defmethod (macro (methodname arglist body) (list (quote define) (quote methodlist) (list (quote cons) (list (quote list) (list (quote quote) methodname) (list (quote lambda) arglist body)) (quote methodlist)))))
(define _selector (lambda (methodlist targetmethod) ((lambda (methodlist targetmethod curitem curname ret) (progn (while methodlist (progn (define curitem (car methodlist)) (define curname (car curitem)) (if (eq curname targetmethod) (progn (define methodlist ()) (define ret (car (cdr curitem)))) ()) (define methodlist (cdr methodlist)))) ret) ) methodlist targetmethod)))

(define counter (lambda ()
  ((closure (methodlist)
    (progn
      ((closure (n)
        (progn
          (define n 0)
          (defmethod inc ()
            (define n (+ n 1)))
          (defmethod get ()
            n)
          (defmethod set (m)
            (define n m))
        )))
      (lambda (targetmethod)
          (_selector methodlist targetmethod)))))))

(define counter1 (new counter))
(define counter2 (new counter))
((. counter2 set) 12)

(print ((. counter1 get)) ())
(print ((. counter2 get)) ())

((. counter1 inc))
((. counter2 inc))
((. counter2 inc))

(print ((. counter1 get)) ())
(print ((. counter2 get)) ())
