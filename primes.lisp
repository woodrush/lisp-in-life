(define i (macro (b) b))

(define defun
  (macro (fname varlist body)
    (list
      (quote define) (i fname)
      (list (quote lambda*) varlist body))))

(defun append (l item)
  (if l
    (cons (car l) (append (cdr l) item))
    (cons item ())))

(defun isprime (n)
  ((lambda* (primelist p ret)
      (while primelist
        (progn
          (define p (car primelist))
          (define primelist (cdr primelist))
          (if (eq 0 (mod n p))
            (progn
              (define primelist ())
              (define ret ()))
            ())))
      ret)
   primelist () 1))

(define n 2)
(define nmax 20)
(define primelist (cons 2 ()))

(while (< n nmax)
  (progn
    (define n (+ 1 n))
    (if (isprime n)
      (define primelist (append primelist n))
      ())))

(print primelist)