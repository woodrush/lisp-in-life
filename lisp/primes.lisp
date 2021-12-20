(define n_max 20)

(define n 3)
(define primelist (list 2))

(define append (lambda (l item)
  (if l
    (cons (car l) (append (cdr l) item))
    (cons item ()))))

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
    (define primelist (append primelist n))
    ())

  (define n (+ 1 n)))

(print primelist)
