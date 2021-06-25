LISP_OPT=./build/lisp_opt.qftasm

all:
	make $(LISP_OPT)

run:
	make run_gcc
	make run_qft

$(LISP_OPT):
	bash src/build_optlisp.sh

run_qft:
	bash src/runlisp.sh $(LISP_OPT)

build_gcc:
	gcc src/lisp.c -Isrc

run_gcc:
	bash src/runlisp_gcc.sh
