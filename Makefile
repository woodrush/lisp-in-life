lisp_qftasm=./out/lisp.qftasm
lisp_qftasm_512=./out/lisp_512.qftasm
executable=./out/lisp


all:
	$(MAKE) $(lisp_qftasm)

$(lisp_qftasm): ./src/lisp.c ./src/lisp.h ./src/qft.h
	cd elvm && $(MAKE)
	./tools/build_optlisp.sh

$(lisp_qftasm_512): ./src/lisp.c ./src/lisp.h ./src/qft.h
	cd elvm && $(MAKE)
	./tools/build_optlisp_512.sh

lisp_512:
	$(MAKE) $(lisp_qftasm_512)

test: $(lisp_qftasm)
	./tools/runtest.sh

run_qft:
	./tools/runlisp.sh $(lisp_qftasm)

run_qft_memdist:
	./tools/runlisp.sh $(lisp_qftasm) --plot-memdist

run_qft_512:
	./tools/runlisp_512.sh $(lisp_qftasm_512)

run_qft_512_memdist:
	./tools/runlisp_512.sh $(lisp_qftasm_512) --plot-memdist


#================================================================
# Compiling and testing lisp.c with gcc
#================================================================
$(executable):
	gcc src/lisp.c -Isrc -o ./out/lisp

test_executable: $(executable)
	./tools/runtest.sh --test-executable

run_executable: ./out/lisp
	./tools/runlisp_gcc.sh


#================================================================
# Compiling the Hello World C program to QFTASM
#================================================================
hello:
	./tools/qftasmc.sh  \
		-i ./misc/hello/hello.c \
		-o ./out/hello.qftasm \
		-8 "-Imisc/hello" \
		-p

run_hello:
	@./tools/qftasmi.sh \
		-i ./out/hello.qftasm \
		-u "$$(cat ./misc/hello/hello_stdin.txt)"

run_hello_debug:
	@./tools/qftasmi.sh \
		-i ./out/hello.qftasm \
		-u "$$(cat ./misc/hello/hello_stdin.txt)" \
		-p "--debug-ramdump --debug-plot-memdist"
