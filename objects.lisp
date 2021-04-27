;; (define class (macro (fieldlist body)
;;   (list (quote lambda) (quote ())
;;         (list (list (quote closure) fieldlist
;;               (list (quote lambda) (list (quote methodname))
;;                     (list (list (quote lambda)
;;                                 (list (quote ret))
;;                                 (list (quote progn) body (quote ret)))
;;                           (quote ()))))))))
;; (define defclass (macro (classname fieldlist body)
;;   (list (quote define) classname
;;     (list (quote class) fieldlist body))))
;; (define defmethod (macro (methodname arglist body)
;;   (list (quote if)
;;         (list (quote eq) (quote methodname) (list (quote quote) methodname))
;;         (list (quote define) (quote ret) (list (quote lambda) arglist body)) (quote ()))))
;; (define . (macro (object methodname)
;;   (list object (list (quote quote) methodname))))

;; (defclass counter (n)
;;   (progn
;;     (defmethod inc ()
;;       (define n (+ n 1)))
;;     (defmethod set (arg)
;;       (define n arg))
;;     (defmethod get ()
;;       n)))

;; (define append (lambda (l item)
;;   (if l
;;     (cons (car l) (append (cdr l) item))
;;     (cons item ()))))

(define counter (lambda ()
  ((closure (methodlist)
  
    (progn
      ((closure (n)
        (progn
          (define methodlist
            (cons
              (list (quote inc)
                (lambda () (define n (+ n 1))))
              methodlist))
          (define methodlist
            (cons
              (list (quote get)
                (lambda () n))
              methodlist))
          (define methodlist
            (cons
              (list (quote set)
                (lambda (m) (define n m)))
              methodlist))
          
          
        )) ())

      (lambda (callname)
      
        ((lambda (methodlist curitem curname ret)
          (progn
            (while methodlist
              (progn
                (print methodlist ())
                (define curitem (car methodlist))
                (define curname (car curitem))
                (print (quote curitem) ())
                (print curitem ())
                (print (car (cdr curitem)) ())
                (if (eq curname callname)
                  (progn
                    (print (quote matched!) ())
                    (define methodlist ())
                    (define ret (car (cdr curitem))))
                  ())
                (define methodlist (cdr methodlist))
                  ))
            ret)
        ) methodlist)
      )

      )
  ) () ) 
))

(define counter1 (counter))
(print counter1 ())
(print (quote aa))
(print (counter1 (quote get)) ())
((counter1 (quote set)) 12)
(print ((counter1 (quote get))) ())

;; (print 8 ())
;; (define counter2 (counter))

;; ((. counter1 set) 0)
;; ((. counter2 set) 5)
;; (print ((. counter1 get)) ())
;; ((. counter1 inc))
;; ((. counter1 inc))
;; ((. counter2 inc))
;; (print ((. counter1 get)) ())
;; ((. counter1 inc))
;; (print ((. counter1 get)) ())
;; (print ((. counter2 get)) ())
