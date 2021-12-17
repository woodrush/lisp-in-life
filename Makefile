LISP_QFTASM=./out/lisp.qftasm

all:
	$(MAKE) $(LISP_QFTASM)

$(LISP_QFTASM):
	cd elvm && $(MAKE)
	./tools/build_optlisp.sh

test: $(LISP_QFTASM)
	./tools/runtest.sh

run:
	$(MAKE) run_gcc
	$(MAKE) run_qft

run_qft:
	./tools/runlisp.sh $(LISP_QFTASM)

run_qft_memdist:
	./tools/runlisp.sh $(LISP_QFTASM) --plot-memdist

./out/lisp:
	gcc src/lisp.c -Isrc -o ./out/lisp

run_gcc: ./out/lisp
	./tools/runlisp_gcc.sh

