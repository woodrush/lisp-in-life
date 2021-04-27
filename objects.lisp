;; (define defmethod (macro (methodname body nextmethod)
;;   (list (quote if) (list (quote eq) (quote methodname) (list (quote quote) methodname)) body nextmethod)))
;; (define callmethod (macro (object methodname arg)
;;   (list object (list (quote quote) methodname) arg)))

;; (define newcounter (lambda (m)
;;   ((closure (n)
;;     (lambda (methodname arg)
;;       (if (eq methodname (quote inc))
;;         (define n (+ n 1))
;;       (if (eq methodname (quote dec))
;;         (define n (- n 1))
;;       (if (eq methodname (quote print))
;;         (print n ())
;;       (if (eq methodname (quote set))
;;         (define n arg)
;;       (if (eq methodname (quote get))
;;         n
;;         n)))))))
;;     (if m m 0))))

(define defmethod (macro (methodname body nextmethod)
  (list (quote if)
        (list (quote eq) (quote methodname) (list (quote quote) methodname))
        body nextmethod)))
(define callmethod (macro (object methodname arg)
  (list object (list (quote quote) methodname) arg)))

(define newcounter (lambda (m)
  ((closure (n)
    (lambda (methodname arg)
      (defmethod inc
        (define n (+ n 1))
      (defmethod dec
        (define n (- n 1))
      (defmethod print
        (print n ())
      (defmethod set
        (define n arg)
      (defmethod get
        n
        n)))))))
    (if m m 0))))

(print (list 1 2 3))
(define counter1 (newcounter))
(define counter2 (newcounter 5))
(callmethod counter1 print)
(callmethod counter1 inc)
(callmethod counter1 inc)
(callmethod counter1 inc)
(callmethod counter1 dec)
(callmethod counter1 print)


(callmethod counter2 print)
(callmethod counter2 inc)
(callmethod counter2 inc)
(callmethod counter2 inc)
(callmethod counter1 inc)
(callmethod counter1 inc)
(callmethod counter2 dec)

(callmethod counter2 print)
(callmethod counter1 print)

