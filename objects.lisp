(define class (macro (fieldlist body)
  (list (quote lambda) (quote ())
    (list (quote progn)
      (list (list (quote closure) fieldlist body))
      (quote (lambda (callname)
      
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
        ) methodlist))) )
  )))
(define defmethod (macro (methodname arglist body)
  (list (quote define) (quote methodlist)
        (list (quote cons)
              (list (quote list)
                    (list (quote quote) methodname)
                    (list (quote lambda) arglist body))
              (quote methodlist)))))

(define . (macro (object methodname)
  (list object (list (quote quote) methodname))))

(define counter (class (n)
(progn
          (defmethod inc ()
            (define n (+ n 1)))
          (defmethod set (m)
            (define n m))
          (defmethod get ()
            n)
        )
))
;; (define counter (lambda ()
;;   ((closure (methodlist)
  
;;     (progn
;;       ((closure (n)
;;         (progn
;;           (defmethod inc ()
;;             (define n (+ n 1)))
;;           (defmethod set (m)
;;             (define n m))
;;           (defmethod get ()
;;             n)
;;         ) ))
 
;;       (lambda (callname)
      
;;         ((lambda (methodlist curitem curname ret)
;;           (progn
;;             (while methodlist
;;               (progn
;;                 (define curitem (car methodlist))
;;                 (define curname (car curitem))
;;                 (if (eq curname callname)
;;                   (progn
;;                     (define methodlist ())
;;                     (define ret (car (cdr curitem))))
;;                   ())
;;                 (define methodlist (cdr methodlist))
;;                   ))
;;             ret)
;;         ) methodlist)
;;       )

;;       )
;;   )) 
;; ))

(define counter1 (counter))
(print (. counter1 get) ())
((. counter1 set) 12)
((. counter1 inc))
((. counter1 inc))
((. counter1 inc))
(print ((. counter1 get)) ())

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
