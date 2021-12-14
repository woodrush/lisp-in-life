LISP_OPT=./out/lisp_opt.qftasm

all: ./elvm/8cc/8cc
	$(MAKE) $(LISP_OPT)

run:
	$(MAKE) run_gcc
	$(MAKE) run_qft

$(LISP_OPT): ./elvm/8cc/8cc
	./src/build_optlisp.sh

run_qft:
	./src/runlisp.sh $(LISP_OPT)

build_gcc:
	gcc src/lisp.c -Isrc

run_gcc:
	./src/runlisp_gcc.sh

./elvm/8cc/8cc:
	git submodule update --init --recursive
	cd elvm && $(MAKE)
