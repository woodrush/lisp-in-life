(define ` (macro (body)
  ((lambda (`-helper)
    (define `-helper (lambda (body)
      (if (atom body)
        (list (quote quote) body)
      (if (eq (quote ~) (car body))
        (car (cdr body))
      (if (if (atom (car body)) () (eq (quote ~@) (car (car body))))
        (list append (car (cdr (car body))) (`-helper (cdr body)))
        (list
          (quote cons)
          (`-helper (car body))
          (`-helper (cdr body))))))))
    (`-helper body)))))

(define append (lambda (l1 l2)
  (if l1
    (cons (car l1) (append (cdr l1) l2))
    l2)))

(define splicequote (macro (l x)
  (` (quote ((~@ l) (~ x) 4)))))

(print (splicequote (1 2) 3))
;; => (1 2 3 4)
