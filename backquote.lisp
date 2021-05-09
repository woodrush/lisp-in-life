(define ` (macro (body)
  (progn
    (define backquote-helper 
      (lambda (body)
        (if (atom body)
          (if (eq () body)
            (quote ())
            (list (quote quote) body))
          (if (eq (quote ~) (car body))
            (car (cdr body))
            (cons
              (quote cons)
              (cons
                (backquote-helper (car body))
                (list (backquote-helper (cdr body)))))))))
    (list (quote eval) (list (quote quote) (backquote-helper body))))))

(define printline (macro (body)
  (` (print (~ body) ())))  )

(print (` (print (~ 5))))
(printline 5)

(define c 5)
(print (` (a b (~ c))) ())
