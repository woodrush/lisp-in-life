LISP_OPT=./out/lisp.qftasm

all:
	$(MAKE) $(LISP_OPT)

run:
	$(MAKE) run_gcc
	$(MAKE) run_qft

$(LISP_OPT):
	cd elvm && $(MAKE)
	./tools/build_optlisp.sh

run_qft:
	./tools/runlisp.sh $(LISP_OPT)

run_qft_memdist:
	./tools/runlisp.sh $(LISP_OPT) --plot-memdist

./out/lisp:
	gcc src/lisp.c -Isrc -o ./out/lisp

run_gcc: ./out/lisp
	./tools/runlisp_gcc.sh
