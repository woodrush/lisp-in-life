(define printquote (macro (x)
  (list (quote print) (list (quote quote) x))))

(printquote Hi!)
