lisp_qftasm=./out/lisp.qftasm
executable=./out/lisp


all:
	$(MAKE) $(lisp_qftasm)

$(lisp_qftasm):
	cd elvm && $(MAKE)
	./tools/build_optlisp.sh

test: $(lisp_qftasm)
	./tools/runtest.sh

run_qft:
	./tools/runlisp.sh $(lisp_qftasm)

run_qft_memdist:
	./tools/runlisp.sh $(lisp_qftasm) --plot-memdist


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
		-u "standard_input"

run_hello_debug:
	@./tools/qftasmi.sh \
		-i ./out/hello.qftasm \
		-u "standard_input" \
		-p "--debug-ramdump --debug-plot-memdist"
