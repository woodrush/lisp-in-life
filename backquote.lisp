(define ` (macro* (body)
  ;; Create a local variable named `-helper inside the closure
  ((lambda (`-helper)
    ;; The value of `-helper remains persistent between calls
    (define `-helper (lambda (body)
      (if (atom body)
        (list (quote quote) body)
        (if (eq (quote ~) (car body))
          (car (cdr body))
          (list
            (quote cons)
            (`-helper (car body))
            (`-helper (cdr body)))))))
    ;; When arguments are skipped in lambda calls, they are initialized with ()
    (list (quote eval) (list (quote quote) (`-helper body)))))))

;; Since we don't have reader macros, the syntax for ` and ~ is slightly different
(define printquote (macro (body)
  (` (print (quote (~ body))))))

(printquote Hi!)     ;; => Hi!
(printquote (1 2 3)) ;; => (1 2 3)

(define c 5)
(print (` (a b (~ c))))
;; => (a b 5)

(printquote (` (a b (~ c))))
;; => (` (a b (~ c)))