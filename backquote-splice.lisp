(define ` (macro* (body)
  ;; Create a local variable named `-helper.
  ;; Calling a lambda without a corresponding argument will initialize the variable with ().
  ((lambda (`-helper appendlist)
    ;; Provide the definition of `-helper afterwards
    (define appendlist (quote ((lambda (appendlist)
      (define appendlist (lambda (l item)
            (if l
              (cons (car l) (appendlist (cdr l) item))
              item)))))))
    (define `-helper (lambda (body)
      (if (atom body)
        (list (quote quote) body)
        (if (eq (quote ~) (car body))
          (car (cdr body))
          (if (if (eq () (atom (car body))) (eq (quote ~@) (car (car body))) f)
            (list appendlist (car (cdr (car body))) (`-helper (cdr body)))
            ;; (list (quote quote) (quote a))
            ;; (car (cdr body))
            ;; (appendlist (car (cdr body)) (`-helper (cdr (cdr body))))
            ;; (list (quote quote) (quote (a b c)))

            ;; (progn (print body)
            (list
              (quote cons)
              (`-helper (car body))
              (`-helper (cdr body)))
              ;; )
          
          )
          ) )))
    (list (quote eval) (list (quote quote) (`-helper body)))))))

;; ;; Since we don't have reader macros, the syntax for ` is slightly different
;; (define printquote (macro (body)
;;   (` (print (quote (~ body))))))

;; (printquote Hi!)
;; (printquote (1 2 3))

;; ;; Prints (a b 5)
;; (define c 5)
;; (print (` (a b (~ c))))
;; (define appendlist (lambda (l item)
;;   (if l
;;     (cons (car l) (appendlist (cdr l) item))
;;     item)))
;; (print (appendlist (quote (1 2)) (quote (3 4))))
;; ;; Prints (` (a b (~ c))
;; (printquote (` (a b (~ c))))


(print (` (a b (~@ (list 1 2)) e f)))

(define lambdatest (macro (args blist)
  (` (lambda (~ args) (~@ blist)))))

(print (lambdatest (n) ((print n) (print n))))
((lambdatest (n) ((print n) (print n))) 5)