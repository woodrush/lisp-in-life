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

build_gcc:
	gcc src/lisp.c -Isrc

run_gcc:
	./src/runlisp_gcc.sh
