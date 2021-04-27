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
