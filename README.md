# Lisp in Conway's Game of Life

It's not Conway's Game of Life implemented in Lisp - it's Lisp implemented in Conway's Game of Life.

The entire pattern is viewable [here]().

This repository contains a Conway's Game of Life (GoL) pattern that runs a Lisp interpreter. The pattern is configurable and can load and run your own lisp code that is up to 1000 characters. The program is loaded into the pattern by writing the ASCII representation of the program into the RAM module, by editing certain cells within the pattern. The Lisp implementation supports lexical closures and macros, allowing one to write Lisp programs in a Lisp-like taste, as far as the memory limit allows you to.

The architecture is based on the computer used in the [Quest For Tetris](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life) project.


## How is it Done?

![The toolchain used for this project](./img/toolchain.png)

### Porting C code to Conway's Game of Life
The final Game of Life pattern for the lisp interpreter is created by porting C code into a Game of Life pattern. The most important step for accomplishing this was to create a module that would provide a link between two existing works, [ELVM](https://github.com/shinh/elvm) and [The Quest For Tetris](https://github.com/QuestForTetris/QFT) (QFT).

[ELVM](https://github.com/shinh/elvm) (the Esoteric Language Virtual Machine) is a toolchain intended to compile C programs to various target languages. It consists of a frontend and a backend, where the frontend compiles C code to the ELVM assembly language, and the backend compiles the ELVM assembly language to the target language.

[The Quest For Tetris](https://github.com/QuestForTetris/QFT) (QFT) is a collaborative project that creates a computer that runs Tetris in the Game of Life. The computer is created by first defining a virtual Harvard architecture RISC CPU (which I will refer to as the QFT architecture, or simply QFT) that runs an assembly language called QFTASM. The project is explained in detail in this [Stack Exchange post](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life).

In this project, starting from these two existing works, the first largest step in accomplishing the creation of the final pattern was writing the [ELVM QFTASM backend](https://github.com/shinh/elvm/tree/master/tools/qftasm), which compiles ELVM assembly to QFTASM. This step allowed to create a pathway for compiling C code and porting it to the Game of Life. I therefore then wrote the Lisp interpreter in C, which is finally ported into the Game of Life pattern.


### Game of Life-Specific Optimizations
However, simply compiling the Lisp interpreter and porting it to QFT resulted in a Game of Life pattern that consumed too much computational resources to run in a practical time. I therefore 

A more detailed list

#### The C Compiler layer
- Added the computed goto feature to ELVM
- Modified the compiler to preserve and output memory address symbols and program address symbols, for their usage in the compiler optimization tool in the QFTASM layer

#### The QFTASM layer
- Created a compiler optimization tool to reduce the QFTASM assembly size

#### The Varlife layer (the computer architecture)
- Recreated the ROM module with a cascaded hash table structure for the ROM module to save space
- Added new optimized instructions to the ALU, and removed unused ones
- Optimizations based on the fact that the absence of signals represents 0
- Reduced the ROM word size from 58 bits to 45 bits
- Extended the ROM and RAM address space from 9,7-bit to 12,10-bit

#### The Game of Life layer
- Improved the metafier script so that it could handle large patterns, by improving the memory usage of the memoization procedure
- Aligned the pattern to 8x8 Varlife cells, to reduce the resulting [Macrocell](https://www.conwaylife.com/wiki/Macrocell) format output (Since the Macrocell format is the data structure used for the [Hashlife](https://en.wikipedia.org/wiki/Hashlife) algorithm, I believe this serves for optimization in the Hashlife layer)

Since the C compiler proided in ELVM is designed to be compact, it lacks rich compiler optimization features. Therefore, various optimization methods are used in the C layer as well, such as:
- Extensive use of global variables, which leads to prevention of indirect references
- Specially defined structs and unions for the stack region of the main evaluation function



The largest parts of accomplishing the this was writing the following tools:
- The extended ELVM QFTASM backend
- The QFTASM optimizer and preprocessor



The extended ELVM QFTASM backend is a part of the ELVM toolchain. [ELVM](https://github.com/shinh/elvm) is a toolchain intended to compile C programs to various target languages. I first wrote the [ELVM QFTASM backend](https://github.com/shinh/elvm/tree/master/tools/qftasm), The target language is QFTASM, the assembly language for the computer architecture built in Conway's Game of Life (GoL). I will refer to the GoL computer architecture as the QFT architecture, or simply QFT.

The first step for this project was to create a link between two existing works, [ELVM](https://github.com/shinh/elvm) and [The Quest For Tetris](https://github.com/QuestForTetris/QFT) (QFT). Making the resulting pattern runnable in a practical amount of time required lots of optimizations in all layers, such as the C compilation layer, the interpreter layer, the Varlife layer, the Game of Life layer, and the Lisp program layer, was done as well.

## Sample Lisp Programs

- print.lisp
- object-oriented-like.lisp
- primes.lisp
- backquote-splice.lisp

### print.lisp
This program is short enough to be actually run in a computer in the Game of Life format. It took 6 hours to run on a 32GB-RAM computer (the Hashlife algorithm used for the simulation consumes a lot of memory space) on [Golly](https://en.wikipedia.org/wiki/Golly_(program)). Note that this pattern runs in XX minutes in the Varlife rule, the abtraction layer used when creating the final pattern - details will be explained later.


### object-oriented-like.lisp
This program is a demonstration of a program writen in an Object-Oriented-Programming-like style, with user-defined classes. Multiple objects can be instantiated, where each object is allocated with distinct memory spaces for their field variables, which can be individually manipulated. Objects are represented as lexical closures over the field variables of the class. This program also features the use of macros to introduce the `.` syntax, dynamically extending the interpreter for expressing programs in a more OOP-like style.

<!-- This program runs in XX minutes in the Varlife layer, but the running time in the Game of Life layer is unconfirmed, since it takes too long to run. -->

<!-- The entire source code is as follows:

```lisp
(define counter
  (lambda (n)
    (lambda (methodname)
      (if (eq methodname (quote inc))
        (lambda () (define n (+ n 1)))
      (if (eq methodname (quote dec))
        (lambda () (define n (- n 1)))
      (if (eq methodname (quote get))
        (lambda () n)
      (if (eq methodname (quote set))
        (lambda (m) (define n m))
        ())))))))

(define . (macro* (object methodname) (list object (list (quote quote) methodname))))
(define new (lambda (x) (x)))

(define counter1 (new counter))
(define counter2 (new counter))

((. counter1 set) 0)
((. counter2 set) 8)

;; `(print x t)` prints the value with a newline
(print ((. counter1 inc)) t) ;; => 1
(print ((. counter1 inc)) t) ;; => 2
(print ((. counter1 inc)) t) ;; => 3
(print ((. counter2 inc)) t) ;; => 9
(print ((. counter2 dec)) t) ;; => 8
(print ((. counter1 inc)) t) ;; => 4
(print ((. counter2 inc)) t) ;; => 5
``` -->


### primes.lisp
This program calculates and prints a list of prime numbers up to 20.


### backquote-splice.lisp
This program implements the [backquote macro](http://cl-cookbook.sourceforge.net/macros.html#LtohTOCentry-2), which supports the unquote and unquote-splice operations. Unquote and unquote-splice are each written as `~` and `~@`.


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

The first step for this project was to create a link between two existing works, [ELVM](https://github.com/shinh/elvm) and [The Quest For Tetris](https://github.com/QuestForTetris/QFT) (QFT). Making the resulting pattern runnable in a practical amount of time required lots of optimizations in all layers, such as the C compilation layer, the interpreter layer, the Varlife layer, the Game of Life layer, and the Lisp program layer, was done as well.

Here I will first explain the original two projects, ELVM and QFT, and then will explain the details of this project.


### ELVM
[ELVM](https://github.com/shinh/elvm), the Esoteric Language Virtual Machine, is a project inspired by LLVM, which can be used to port C code to [Esoteric Programming Languages](https://en.wikipedia.org/wiki/Esoteric_programming_language) such as [Unlambda](https://en.wikipedia.org/wiki/Unlambda) and [Piet](https://en.wikipedia.org/wiki/Esoteric_programming_language#Piet). It is done by first compiling C code to an assembly language targeted for a virtual RISC architecture, and then porting the assembly language to the target Esoteric Language. The latter step is called as the backend of ELVM. In this project, one of the main works I have done is to create a QFTASM backend for ELVM.

### The Quest For Tetris 
[The Quest For Tetris](https://github.com/QuestForTetris/QFT) (QFT) is a collaborative project that was aimed to run Tetris in the Game of Life. The project is explained in detail in this [Stack Exchange post](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life).

The method is as follows. First, the authors define a Life-like cellular automaton rule which they call as Varlife. Each Varlife cell can be assigned one rule among four different rules, and one state among two different states; therefore, each Varlife cell has eight distinct states. The important property of Varlife is that any given Varlife pattern can be systematically transformed into a GoL pattern that has an isomorphic behavior. This is done by replacing each Varlife cell with an [OTCA metapixel](https://www.conwaylife.com/wiki/OTCA_metapixel) (OTCAMP), i.e. metafying each cell. The trick is to tile OTCAMPs with different rules next to each other, corresponding to the rule assigned to each Varlife cell (I really like this clever way the OTCA metapixel is used). Therefore, by verifying a pattern's behavior on Varlife, one can automatically verify the behavior of the corresponding metafied GoL pattern.

Next, the authors create a programmable computer in the Varlife rule. They create an assembly language for the computer called QFTASM. The authors write a Tetris program on QFTASM, and port the written program to a Varlife pattern and integrate it into the computer. This pattern is capable of running Tetris in the Varilfe rule. In this writing, the computer here will be referred to as the QFT computer.

Finally, the Varlife pattern is metafied to obtain the final GoL pattern, to obtain a pattern that runs Tetris on the Game of Life.


### This Project
I first started by creating a [QFTASM backend for ELVM](https://github.com/woodrush/elvm/tree/add-qftasm/tools/qftasm). From there, I have done lots of optimizations in various layers:

This allows one to use ELVM to compile C code to QFTASM, which can be ported to the QFT computer, and metafied into a GoL pattern.

I have also done the following on the QFT side as well:

- The original pattern for the QFT architecture on the QFT repo ([Tetris8.mc](https://github.com/QuestForTetris/QFT/blob/master/Tetris8.mc)) had a 9-bit ROM, 7-bit RAM demultiplexer address size, designed to fit the original Tetris program which had 290 instructions in the ROM, and used 71 bytes of RAM. Since the rest of the hardware is designed to be capable of having 16 bits of both ROM and RAM address size, I extended the Tetris8.mc pattern to a 15-bit ROM, 14-bit RAM address size architecture. The pattern available [here](https://github.com/woodrush/QFT-devkit/blob/main/QFT_15bit_rom_14bit_ram_blank.mc) in the QFT-devkit repository. This was done by reading QFT's Stack Exchange post, and carefully studying the contents of the QFT repo, which contained commented patterns of modules contained in the architecture.
- The next obstacle was extending the metafier to transform Varlife patterns to GoL patterns in the original QFT repository ([MetafierV2.py](https://github.com/QuestForTetris/QFT/blob/master/MetafierV2.py)) so that it could handle large patterns. For this, I studied the Macrocell format (*.mc) and the Hashlife algorithm. The example that ultimately led me to understanding the format was the pattern mentioned by Adam P. Goucher in the first post in [this thread](https://sourceforge.net/p/golly/mailman/golly-test/thread/983116CEB3464154AF26245F052EFB36%40adamspackard/). [Golly's page](http://golly.sourceforge.net/Help/formats.html#mc) and the [Life Lexicon's page](https://www.conwaylife.com/ref/lexicon/lex_m.htm#macrocell) was also informative. For Hashlife, [this writing](https://github.com/mafm/HashLife) let me understand the algorithm.

These tools are all summarized and available in the [QFT-devkit](https://github.com/woodrush/QFT-devkit) repository.


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


----
### anonymous-recursion.lisp
This source code features [Anonymous Recursion](https://en.wikipedia.org/wiki/Anonymous_recursion), to feature the power of lexical closures of the Lisp interpreter. The factorial function is defined entirely by lambda expressions that does not use the global variable namespace.

An equivalent JavaScript code is:

```javascript
console.log(
  (function (f) {
    return ((function (x) { return f(function(v){ return x(x)(v); }) })
            (function (x) { return f(function(v){ return x(x)(v); }) }));
  })(function (fact){
    return function (n) {
      return n == 0 ? 1 : n * fact(n-1);
    };
  })(5)
);
```

Although the Lisp interpreter supports defining recursive functions in a named way as in usual languages (which is done in primes.lisp), this source code rather uses this method for the purpose of the demonstrating the use of lambda expressions. The source code can also be written in alternatively as follows, in a more simple way:

```lisp
(define fact (lambda (n)
  (if (eq n 0) 1 (* n (fact (- n 1))))))
(print (fact 5))
```
