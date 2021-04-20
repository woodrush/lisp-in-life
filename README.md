# Lisp in Conway's Game of Life

This repository is an ongoing project to make a Lisp interpreter run in Conway's Game of Life, based on the computer used in the [Quest For Tetris](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life) project.

## The Current Status

Done:

- I have made a Game-of-Life-like cellular automaton pattern which I have verified that successfully runs Lisp. The rule used in the pattern is Varlife, a Life-like rule defined in the [Quest For Tetris](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life) (QFT) project.
  - The Varlife pattern is available [here](./patterns/lisp_512B_ram_printstdin_QFT.mc).
  - The pattern took 1,308,622,784 generations to finish running, which took 7 minutes to run on a 32GB RAM machine. The final pattern after 1,308,622,784 generations is also available [here](./patterns/lisp_512B_ram_printstdin_results_QFT.mc).
  - A Varlife pattern with a blank stdin buffer for experimenting is available [here](./patterns/lisp_512B_ram_blankstdin_QFT.mc).

Ongoing:

- As explained in the [Stack Exchange post for Quest For Tetris](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life), any Varlife pattern can be systematically transformed into a Conway's Game of Life (GoL) pattern that has an isomorphic behavior.
  - Therefore, the fact that the Varlife pattern runs Lisp gives a strong support that the corresponding GoL pattern can also run Lisp.
  - However, since I felt that it's necessary to run the actual GoL pattern in the GoL rule for actual verification, I am currently taking some time to run the pattern in the GoL rule.
  - The transformed GoL pattern is available [here](./patterns/lisp_512B_ram_printstdin_GoL_QFT.mc).

Addendum:

- The computer in the Varlife-verified pattern has only 512 bytes (1 byte is 16 bits in the computer architecture used to run Lisp) of RAM space, so the class of programs that it can run is limited. Using the [QFT Devkit](https://github.com/woodrush/QFT-devkit), it is possible to expand the ram up to 65536 bytes, which gives room for larger programs to run on.
  - A 5000-byte RAM Varlife pattern that finds prime numbers up to 20 is available [here](./patterns/lisp_5000B_ram_primestdin_QFT.mc).
  - The GoL version of this pattern is available [here](./patterns/lisp_5000B_ram_primestdin_GoL_QFT.mc).
  - A Varlife version with a blank stdin buffer is available [here](./patterns/lisp_5000B_ram_blankstdin_QFT.mc).
  - However, expanding the RAM makes even Varlife patterns to run slow on Golly, so I haven't been able to verify patterns that have a significantly larger RAM space.

## Screenshots
![An animation of the RAM module of the QFT computer in the Varlife rule, while it is running.](./img/lisp_512B_ram_printstdin_QFT.mc.gif)

An animation of the RAM module of the QFT computer in the Varlife rule, while it is running. The colors of the cells represent the 8 distinct states of the Varlife rule.

![The QFT computer showing the results of the computation.](./img/lisp_512B_ram_printstdin_results_QFT.mc.gif)

The QFT computer showing the results of the computation of the following code:

```lisp
(print (* 3 14))
```

The result is `42`, shown in binary ascii format (0b110100, 0b110010), read in bottom-to-up order.

![The RAM module of the QFT computer converted to a Conway's Game of Life pattern while running.](./img/lisp_512B_ram_printstdin_QFT.mc.png)

The RAM module of the QFT computer converted to a Conway's Game of Life pattern while running. Each "cell" visible here is actually an [OTCA metapixel](https://www.conwaylife.com/wiki/OTCA_metapixel) (OTCAMP) zoomed far away.


## The Approach
![The toolchain used for this project](./img/toolchain.png)

Basically, I created a link between two existing works, [ELVM](https://github.com/shinh/elvm) and [The Quest For Tetris](https://github.com/QuestForTetris/QFT) (QFT). There was also lots of work done on the QFT side to actually create a working pattern, obtain the GoL pattern from the Varlife pattern, etc.


### ELVM
[ELVM](https://github.com/shinh/elvm), the Esoteric Language Virtual Machine, is a project inspired by LLVM, which can be used to port C code to [Esoteric Programming Languates](https://en.wikipedia.org/wiki/Esoteric_programming_language) such as [Unlambda](https://en.wikipedia.org/wiki/Unlambda) and [Piet](https://en.wikipedia.org/wiki/Esoteric_programming_language#Piet). It is done by first compiling C code to an assembly language targeted for a virtual RISC architecture, and then porting the assembly language to the target Esoteric Language. The latter step is called as the backend of ELVM. In this project, one of the main works I have done is to create a QFTASM backend for ELVM.

### The Quest For Tetris 
[The Quest For Tetris](https://github.com/QuestForTetris/QFT) (QFT) is a collaborative project that was aimed to run Tetris in the Game of Life. The project is explained in detail in this [Stack Exchange post](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life).

The method is as follows. First, the authors define a Life-like cellular automaton rule which they call as Varlife. Each Varlife cell can be assigned one rule among four different rules, and one state among two different states; therefore, each Varlife cell has eight distinct states. The important property of Varlife is that any given Varlife pattern can be systematically transformed into a GoL pattern that has an isomorphic behavior. This is done by replacing each Varlife cell with an [OTCA metapixel](https://www.conwaylife.com/wiki/OTCA_metapixel) (OTCAMP), i.e. metafying each cell. The trick is to tile OTCAMPs with different rules next to each other, corresponding to the rule assigned to each Varlife cell (I really like this clever way the OTCA metapixel is used). Therefore, by verifying a pattern's behavior on Varlife, one can automatically verify the behavior of the corresponding metafied GoL pattern.

Next, the authors create a programmable computer in the Varlife rule. They create an assembly language for the computer called QFTASM. The authors write a Tetris program on QFTASM, and port the written program to a Varlife pattern and integrate it into the computer. This pattern is capable of running Tetris in the Varilfe rule. In this writing, the computer here will be referred to as the QFT computer.

Finally, the Varlife pattern is metafied to obtain the final GoL pattern, to obtain a pattern that runs Tetris on the Game of Life.


### This Project
The main work done for this project is to create a [QFTASM backend for ELVM](https://github.com/woodrush/elvm/tree/add-qftasm/tools/qftasm). This allows one to use ELVM to compile C code to QFTASM, which can be ported to the QFT computer, and metafied into a GoL pattern.

I have also done the following on the QFT side as well:

- The original pattern for the QFT architecture on the QFT repo ([Tetris8.mc](https://github.com/QuestForTetris/QFT/blob/master/Tetris8.mc)) had a 9-bit ROM, 7-bit RAM demultiplexer address size, designed to fit the original Tetris program which had 290 instructions in the ROM, and used 71 bytes of RAM. Since the rest of the hardware is designed to be capable of having 16 bits of both ROM and RAM address size, I extended the Tetris8.mc pattern to a 15-bit ROM, 14-bit RAM address size architecture. The pattern available [here](https://github.com/woodrush/QFT-devkit/blob/main/QFT_15bit_rom_14bit_ram_blank.mc) in the QFT-devkit repository. This was done by reading QFT's Stack Exchange post, and carefully studying the contents of the QFT repo, which contained commented patterns of modules contained in the architecture.
- The next obstacle was extending the metafier to transform Varlife patterns to GoL patterns in the original QFT repository ([MetafierV2.py](https://github.com/QuestForTetris/QFT/blob/master/MetafierV2.py)) so that it could handle large patterns. For this, I studied the Macrocell format (*.mc) and the Hashlife algorithm. The example that ultimately led me to understanding the format was the pattern mentioned by Adam P. Goucher in the first post in [this thread](https://sourceforge.net/p/golly/mailman/golly-test/thread/983116CEB3464154AF26245F052EFB36%40adamspackard/). [Golly's page](http://golly.sourceforge.net/Help/formats.html#mc) and the [Life Lexicon's page](https://www.conwaylife.com/ref/lexicon/lex_m.htm#macrocell) was also informative. For Hashlife, [this writing](https://github.com/mafm/HashLife) let me understand the algorithm.

These tools are all summarized and available in the [QFT-devkit](https://github.com/woodrush/QFT-devkit) repository.


## The Source Code for the Lisp Interpreter
The original C code used to create the GoL pattern is available in this repository under `./src`. When writing `lisp.c`, I referenced some C implementation techniques from [lisp.c in the ELVM repository](https://github.com/shinh/elvm/blob/master/test/lisp.c). Some parts of `elvm.h` are copied from the header files of ELVM's library. Otherwise, the source code was written by the author of this repository from scratch.

### Compiling with GCC
The code can also be compiled with GCC to test on a usual computer. To do this, use gcc under the `./src` directory, or use the `-I` option to include `./src` to let gcc find `lisp.h` when compiling.

### Compiling on ELVM
Although compiling on ELVM can be done in the same way as GCC, switching the header file to `elvm.h` will allow the resulting QFTASM code to be smaller. This can be done by adding the following options:

- `-DELVM`, for 8cc (which is the C compiler used for ELVM) to switch the header file to `elvm.h`, and
- `-I[path to ./src]`, for including the header file `elvm.h`.

Also, the compiler configurations in `./target/qftasm.c` in the ELVM repository must be changed according to the following table:

| Architecture  | QFTASM_RAMSTDIN_BUF_STARTPOSITION | QFTASM_RAMSTDIN_BUF_STARTPOSITION | QFTASM_MEM_OFFSET | 
| ------------- | --------------------------------- | --------------------------------- | ----------------- | 
| 512-byte RAM  | 503                               | 511                               | 170               | 
| 5000-byte RAM | 4499                              | 4999                              | 1024              | 


## Details of the Lisp Interpreter
### Special Forms and Builtin Functions

- define
- if
- quote
- car, cdr
- cons
- atom
- print
- progn
- while
- lambda, macro
- eval
- eq
- +, -, *, /, mod, <, >

### Lexical Closures
This Lisp implementation supports lexical closures. Therefore, [fixed point combinators](https://en.wikipedia.org/wiki/Fixed-point_combinator) can be used for [anonymous recursion](https://en.wikipedia.org/wiki/Anonymous_recursion) as in the following program:

```lisp
(define Z (lambda (f) (
  (lambda (x) (f (lambda (v) ((x x) v))))
  (lambda (x) (f (lambda (v) ((x x) v)))))))
 
(define fact (Z (lambda (fact_)
  (lambda (n) (if (eq n 0) 1 (* n (fact_ (- n 1))))))))
 
(print (fact 5))
```

You can also use named recursion as follows:

```lisp 
(define fact
  (lambda (n) (if (eq n 0) 1 (* n (fact (- n 1))))))
 
(print (fact 5))
```

Another example of the use of lexical closures is that you can create environments allocated with distinct memory regions with the same variable name, similar to instantiating objects in OOP, as in the following example:

```lisp
(define createcounter (lambda (n) (lambda () (define n (+ 1 n)))))
(define counter1 (createcounter 0))
(define counter2 (createcounter 0))
(print (counter1) ())
(print (counter1) ())
(print (counter1) ())
(print (counter2) ())
(print (counter2) ())
(print (counter1) ())
```

This yields the following output:
```
1
2
3
1
2
4
3
```

As in this example, lambdas create and hold their own lexical environment when they are called. The variables in the environment can be overwritten by using `define` inside the lambda. In the same way, global variables can be shadowed as in `primes.lisp`.

Note that a new lexical variable can be introduced only through `lambda`, and not `define`. When a `define` encounters an unknown variable, it either writes or appends to the global environment instead of introducing the variable to the lexical environment.

Also note that `print` outputs a newline when a second argument is present, and it doesn't when it is called with only one argument.


### Macros
This Lisp implementation has a macro feature. Lisp macros can be thought as a function that receives code and returns code. Following this design, macros are treated exacly the same as lambdas, except for the following properties:

- The arguments of macros are passed as raw S-expressions, i.e. in quoted form, instead of being evaluated first before being passed to the functions as in lambdas.
- The macro is expected to be written as a lambda that takes S-expressions and returns S-expressions. Therefore, the return values of the macros are first passed to `eval`, and then returned to the environment.

An example of a macro in action can be seen in `primes.lisp`, where the macro `defun` is defined:

```lisp
(define defun
  (macro (fname varlist body)
    (cons (quote define)
    (cons fname
    (cons (cons (quote lambda)
          (cons varlist
          (cons body ()))) ())))))
```

Although this Lisp implementation doesn't have a quasiquote feature, this example can be rewritten using quasiquotes as follows (Here, unquote is denoted as `~`):

```lisp
(define defun
  (macro `(define ~fname (lambda ~varlist ~body))))
```

## Results

Here are the results for running the Lisp program on the QFTASM interpreter and on Golly. Since [Golly](https://en.wikipedia.org/wiki/Golly_(program)) can run [Hashlife](https://en.wikipedia.org/wiki/Hashlife) in the Varlife rule, the verification for Varlife has been done on Golly. Running patterns on the GoL rule is currently ongoing.

### On the QFTASM Interpreter
The following table shows the number of QFTASM steps required for running each Lisp program on the Lisp interpreter written in QFTASM. The QFTASM program was ran under the QFTASM interpreter in the [QFT Devkit](https://github.com/woodrush/QFT-devkit).

| Lisp Source | QFTASM Source         | QFTASM ROM Size | Number of QFTASM Steps | 
| ----------- | --------------------- | --------------- | ---------------------- | 
| print.lisp  | lisp_512B_ram.qftasm  | 9634            | 20340                  | 
| primes.lisp | lisp_5000B_ram.qftasm | 9634            | 2447497                | 

Here are the raw interpreter outputs (with `debug_ramdump = True`) for each of the Lisp programs:

print.lisp (using lisp_512B_ram.qftasm):

```
42
ROM size: 9634
n_steps: 20340
Nonzero write count ram addresses: 264
Nonzero write count ram max address: 511
[[65535, 21115], [487, 17], [509, 3], [0, 3893], [65533, 3673], [65494, 122], [65524, 1894], [65535, 209], [65533, 2024], [1, 4473], [896, 104], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0]]
```

primes.lisp (using lisp_5000B_ram.qftasm):

```
(2 3 5 7 11 13 17 19)
ROM size: 9634
n_steps: 2447497
Nonzero write count ram addresses: 3663
Nonzero write count ram max address: 4999
[[65535, 2552004], [3662, 838], [4978, 22], [0, 452523], [65533, 461548], [65490, 8206], [65524, 215834], [65535, 26143], [65533, 221950], [1, 570366], [896, 13071], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0]]
```


### On Golly
The following table summarizes the results for running the Lisp interpreter pattern on Golly. Cells noted as "-" is where I haven't experimented the pattern on Golly. Experiments on Golly was done on a 32GB machine.

| Pattern                                    | Rule    | Embedded Lisp Program | ROM Size | RAM Size | Number of Generations | Golly Running Time | Golly RAM Usage | Golly Max. Mem. Size | Host PC RAM Size | Golly Base Step | 
| ------------------------------------------ | ------- | --------------------- | -------- | -------- | --------------------- | ------------------ | --------------- | -------------------- | ---------------- | --------------- | 
| lisp_512B_ram_printstdin_QFT.mc      | Varlife | print.lisp            | 9634     | 512      | 1,308,622,784         | 7 minutes          | 20.9 GiB        | 28000 MB             | 31.4GiB          | 2               | 
| lisp_512B_ram_printstdin_GoL_QFT.mc  | GoL     | print.lisp            | 9634     | 512      | -                     | -                  | -               | -                    | -                | -               | 
| lisp_5000B_ram_primestdin_QFT.mc     | Varlife | primes.lisp           | 9634     | 5000     | -                     | -                  | -               | -                    | -                | -               | 
| lisp_5000B_ram_primestdin_GoL_QFT.mc | GoL     | primes.lisp           | 9634     | 5000     | -                     | -                  | -               | -                    | -                | -               | 

Since the metafication is done using OTCAMPs, the number of required GoL generations for the program to finish running should be the number of required Varilfe generations times the period of OTCAMPs (35328). Based on the rate of generation, the experiment for the metafied 512B-ram pattern is currently expected to finish in a few days.

## The Current Conclusion
Since the 512byte-RAM pattern is verified to run Lisp on the Varlife rule, this provides a strong support that the corresponding GoL pattern can run Lisp as well, since an arbitrary Varlife pattern can systematically be transformed into a GoL pattern. However, since I feel that it's necessary to actually run the GoL pattern in the GoL rule on Golly for true verification, I'm currently running the GoL pattern on Golly.

On the other hand, the 5000byte-RAM pattern is taking too much time to run on Golly, even for the Varlife pattern. Since the ROM on this pattern is derived from the same C source code as the 512byte-RAM pattern, and the derived ROM is verified on a QFTASM interpreter, I believe that both the Varlife and GoL patterns of the 5000byte-RAM version is capable of running Lisp as well.
