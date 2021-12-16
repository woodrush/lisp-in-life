LISP_OPT=./out/lisp_opt.qftasm

all:
	$(MAKE) $(LISP_OPT)

run:
	$(MAKE) run_gcc
	$(MAKE) run_qft

$(LISP_OPT):
	cd elvm && $(MAKE)
	./src/build_optlisp.sh

run_qft:
	./src/runlisp.sh $(LISP_OPT)

run_qft_memdist:
	./src/runlisp.sh $(LISP_OPT) --plot-memdist

./out/lisp:
	gcc src/lisp.c -Isrc -o ./out/lisp

run_gcc: ./out/lisp
	./src/runlisp_gcc.sh
