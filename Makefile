LISP_QFTASM=./out/lisp.qftasm
executable=./out/lisp

all:
	$(MAKE) $(LISP_QFTASM)

$(LISP_QFTASM):
	cd elvm && $(MAKE)
	./tools/build_optlisp.sh

test: $(LISP_QFTASM)
	./tools/runtest.sh

run_qft:
	./tools/runlisp.sh $(LISP_QFTASM)

run_qft_memdist:
	./tools/runlisp.sh $(LISP_QFTASM) --plot-memdist

$(executable):
	gcc src/lisp.c -Isrc -o ./out/lisp

test_executable: $(executable)
	./tools/runtest.sh --test-executable

run_executable: ./out/lisp
	./tools/runlisp_gcc.sh
