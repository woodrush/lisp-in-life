(define ` (macro (body)
  ;; Create a local variable named `-helper.
  ;; Calling a lambda without a corresponding argument will initialize the variable with ().
  ((lambda* (`-helper)
    (progn
      ;; Define `-helper to its designated definition afterwards.
      ;; Although the variable name "body" is shadowed, it will resolve to the correct variable.
      (define `-helper (lambda (body)
        (if (atom body)
          (list (quote quote) body)
          (if (eq (quote ~) (car body))
            (car (cdr body))
            (cons
              (quote cons)
              (cons
                (`-helper (car body))
                (list (`-helper (cdr body)))))))))
      (list (quote eval) (list (quote quote) (`-helper body))))))))

;; Since we don't have reader macros, we need parentheses to call `.
(define printquote (macro (body)
  (` (print (quote (~ body))))))

(printquote Hi!)