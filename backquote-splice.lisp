(define ` (macro* (body)
  ((lambda (`-helper appendlist)
    (define appendlist
      (quote ((lambda* (appendlist)
        (define appendlist (lambda* (l item)
          (if l
            (cons (car l) (appendlist (cdr l) item))
            item)))))))
    (define `-helper (lambda (body)
      (if (atom body)
        (list (quote quote) body)
      (if (eq (quote ~) (car body))
        (car (cdr body))
      (if (if (eq () (atom (car body))) (eq (quote ~@) (car (car body))) ())
        (list appendlist (car (cdr (car body))) (`-helper (cdr body)))
        (list
          (quote cons)
          (`-helper (car body))
          (`-helper (cdr body))))))))
    (list (quote eval) (list (quote quote) (`-helper body)))))))

(print (` (1 2 (~@ (quote (3 4))) 5 6)))
;; => (1 2 3 4 5 6)

(define lambdasplice (macro (args blist)
  (` (lambda (~ args) (~@ blist)))))

((lambdasplice (n)
   ((print n) (print (+ 1 n))))
 7)
;; => 78