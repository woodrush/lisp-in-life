(define printline (macro (body)
  (` (print (~ body) ()))))

(define ` (macro (body)
  (list (quote eval) (list (quote quote) (`-helper body)))))

(define `-helper 
  (lambda (body)
    (if (atom body)
      (list (quote quote) body)
      (if (eq (quote ~) (car body))
        (car (cdr body))
        (cons
          (quote cons)
          (cons
            (`-helper (car body))
            (list (`-helper (cdr body)))))))))

;; (define printline (lambda* (body) (print body ())))

;; (print (` (print (~ (lambda () ())))))
(printline 5)

;; (define c 5)
;; (print (` (a b (~ c))) ())
