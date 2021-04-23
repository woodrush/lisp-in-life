(progn
    (define defun (macro (fname varlist body)
    (cons (quote define) (cons fname (cons (cons (quote lambda) (cons varlist (cons body))))))))
    (defun append (l item) (if l (cons (car l) (append (cdr l) item)) (cons item)))
    (defun isprime (n) ((lambda (primelist p ret) (progn (while primelist (progn (define p (car primelist)) (define primelist (cdr primelist)) (if (eq 0 (mod n p)) (progn (define primelist) (define ret))))) ret)) primelist) 1)
    )