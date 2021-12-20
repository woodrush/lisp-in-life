(define ` (macro (body)
  ((lambda (`-helper)
    (define `-helper (lambda (body)
      (if (atom body)
        (list (quote quote) body)
        (if (eq (quote ~) (car body))
          (car (cdr body))
          (list
            (quote cons)
            (`-helper (car body))
            (`-helper (cdr body)))))))
    (`-helper body)))))


(define printquote (macro (body)
  (` (print (quote (~ body))))))

(printquote Hi!)
;; => Hi!

(define c 5)
(print (`(a b (~ c))))
;; => (a b 5)

(printquote (` (a b (~ c))))
;; => (` (a b (~ c)))
