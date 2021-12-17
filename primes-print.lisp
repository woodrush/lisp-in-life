(define n_max 20)

(define n 2)
(define primelist ())

(while (< n n_max)
  (define iter primelist)
  (define isprime t)

  (while iter
    (if (eq 0 (mod n (car iter)))
      (progn
        (define iter ())
        (define isprime ()))
      ())
    (define iter (cdr iter)))

  (if isprime
    (progn
      (define primelist (cons n primelist))
      (print n t))
    ())

  (define n (+ 1 n)))
