(define ` (macro* (body)
  ;; Create a local variable named `-helper.
  ;; Calling a lambda without a corresponding argument will initialize the variable with ().
  ((lambda (`-helper)
    ;; Provide the definition of `-helper afterwards
    (define `-helper (lambda (body)
      (if (atom body)
        (list (quote quote) body)
        (if (eq (quote ~) (car body))
          (car (cdr body))
          (list
            (quote cons)
            (`-helper (car body))
            (`-helper (cdr body)))))))
    (list (quote eval) (list (quote quote) (`-helper body)))))))

;; Since we don't have reader macros, the syntax for ` is slightly different
(define printquote (macro (body)
  (` (print (quote (~ body))))))

(printquote Hi!)
(printquote (1 2 3))

;; Prints (a b 5)
(define c 5)
(print (` (a b (~ c))))

;; Prints (` (a b (~ c))
(printquote (` (a b (~ c))))