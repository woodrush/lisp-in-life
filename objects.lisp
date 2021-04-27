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

(define defmethod (macro (methodname arglist body)
  (list (quote define) (quote methodlist)
        (list (quote cons)
              (list (quote list)
                    (list (quote quote) methodname)
                    (list (quote lambda) arglist body))
              (quote methodlist)))))
(define counter (lambda ()
  ((closure (methodlist)
  
    (progn
      ((closure (n)
        (progn
          (defmethod inc () (define n (+ 1 n)))
          (defmethod get () n)
          (defmethod set (m) (define n m))

        )) ())

      (lambda (callname)
      
        ((lambda (methodlist curitem curname ret)
          (progn
            (while methodlist
              (progn
                (define curitem (car methodlist))
                (define curname (car curitem))
                (if (eq curname callname)
                  (progn
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
(define counter2 (counter))
((counter1 (quote set)) 0)
((counter2 (quote set)) 12)
(print ((counter1 (quote get))) ())
(print ((counter2 (quote get))) ())
;; ((counter1 (quote inc)))
;; ((counter1 (quote inc)))
;; ((counter1 (quote inc)))
;; ((counter2 (quote inc)))
;; ((counter1 (quote inc)))
;; ((counter1 (quote inc)))
;; (print ((counter1 (quote get))) ())
;; (print ((counter2 (quote get))) ())

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
(print methodlist)