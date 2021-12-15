# Lisp in Conway's Game of Life

It's not Conway's Game of Life implemented in Lisp - it's a Lisp interpreter implemented in Conway's Game of Life.

The entire pattern is viewable on the browser [here]().

This repository contains a Conway's Game of Life pattern that runs a Lisp interpreter. The pattern is configurable and can load and run your own lisp code up to 1000 characters. The program is loaded into the pattern by writing the ASCII representation of the program into the pattern's RAM module, expressed by editing certain cells in the pattern. The Lisp implementation supports lexical closures and macros, allowing one to write Lisp programs in a Lisp-like taste, as far as the memory limit allows you to.

The architecture is based on the computer used in the [Quest For Tetris](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life) project. The [Lisp interpreter](./src/lisp.c), written in C, is compiled using the Game of [ELVM](https://github.com/shinh/elvm) (the Esoteric Language Virtual Machine) - I have implemented the Game of Life backend for ELVM myself for this project. (A modified branch used for this project is available [here](TODO).)

Using the toolchains in this project, you can compile any C code compatible with C11 and run in on Conway's Game of Life. This requires pre-compilation of the C code on a host computer before pre-loading it into the pattern. I took a step further and let the Game of Life pattern *interpret* Lisp code, provided as ASCII characters loaded into the pattern.


## Pattern Files
Pattern files preloaded with various Lisp programs are available here. For details on each Lisp program, please see the Sample Lisp Programs section.

The patterns can be simulted on the Game of Life simulator [Golly](https://en.wikipedia.org/wiki/Golly_(program)). VarLife patterns can be simulated on Golly as well. Details on VarLife are explained in the next section. For details on running VarLife patterns on Golly, please see the "Running the Patterns" section.

| Program                      | VarLife Pattern                             | Conway's Game of Life Pattern          |
|------------------------------|---------------------------------------------|----------------------------------------|
| arithmetics.lisp             | [lisp_varlife_arithmetics.mc]()             | [lisp_b3s23_arithmetics.mc]()          |
| object-oriented-like.lisp    | [lisp_varlife_object-oriented-like.mc]()    | [lisp_b3s23_object-oriented-like.mc]() |
| primes.lisp                  | [lisp_varlife_primes.mc]()                  | [lisp_b3s23_primes.mc]()               |
| backquote-splice.lisp        | [lisp_varlife_backquote-splice.mc]()        | [lisp_b3s23_backquote-splice.mc]()     |


## Loading and Running Your Own Lisp Program
You can load your own Lisp program into the pattern and run it on Game of Life. This is explained in detail in [./build.md](./build.md).

### What is VarLife?
VarLife is an 8-state cellular automaton defined in the [Quest For Tetris](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life) (QFT) Project. It is used as an intermediate layer to generate the final Conway's Game of Life pattern; the computer is first created in VarLife, and then converted to a Game of Life pattern.

When converting VarLife to Life, each VarLife cell is mapped to an [OTCA Metapixel](https://www.conwaylife.com/wiki/OTCA_metapixel) (OTCAMP). The OTCA Metapixel is a special pattern that is capable of emulating a different game-of-life-like rule within the Game of Life. Each cell carries a binary meta-state and a pre-programmed rule. The conversion from VarLife to the Game of Life is done in a way so that the behavior of the states and generations of VarLife pattern matches exactly with the converted Game of Life pattern's meta-states and meta-generations. Therefore, it is enough to verify the behavior of the VarLife pattern to verify the behavior of the Game of Life pattern.

### The Running Time
Since one OTCA Metapixel is 2048 pixels wide and high, this makes the converted Life pattern expand 2048 times larger from the VarLife pattern. Therefore, running the Life version is significantly slower than running the VarLife version.

The running time for each program is shown here. Unfortunately, programs other than arithmetics.lisp is unconfirmed, and is expected to take over one day to finish running.

| Program                      | Running time (VarLife) | Running Time (Conway's Game of Life) |
|------------------------------|------------------------|--------------------------------------|
| arithmetics.lisp             |                        | 6 hours                              |
| object-oriented-like.lisp    |                        | (unconfirmed)                        |
| primes.lisp                  |                        | (unconfirmed)                        |
| backquote-splice.lisp        |                        | (unconfirmed)                        |



### How is the Output Read Out? How is the Program and the Standard Input Provided?
The output of the computer is read out by examining the bits of the RAM module.

The input is provided byt editing address XX-XX of the RAM module...


## Running the Patterns
Both the Game of Life and VarLife patterns can be run by using [Golly](https://en.wikipedia.org/wiki/Golly_(program)). Golly supports the [Hashlife](https://en.wikipedia.org/wiki/Hashlife) algorithm, which speeds up the computation of the generations of the pattern.

To run VarLife on Golly, additional settings are required. First, obtain [Varlife.rule](https://github.com/QuestForTetris/QFT/blob/master/Varlife.rule) from [the original Quest For Tetris GitHub repository](https://github.com/QuestForTetris/QFT). Then, place Varlife.rule in the same directory as the pattern file you have downloaded. You can also place the rule in Golly's custom rules folder, which is `TODO` on Linux, `~/Library/Application Support/Golly/Rules` on Mac, etc.

### Running your Own Lisp Code
For instructions on running your own Lisp code on the pattern, please see [details.md](./details.md), which explains the technical details of Lisp in Life.


## How is it Done?

![The toolchain used for this project](./img/toolchain.png)

### Porting C code to Conway's Game of Life
The final Game of Life pattern (GoL) for the lisp interpreter is created by porting C code into a Game of Life pattern. The most important step for accomplishing this was to create a module that would provide a link between two existing works, [ELVM](https://github.com/shinh/elvm) and [The Quest For Tetris](https://github.com/QuestForTetris/QFT) (QFT).

[ELVM](https://github.com/shinh/elvm) (the Esoteric Language Virtual Machine) is a toolchain intended to compile C programs to various target languages. It consists of a frontend and a backend, where the frontend compiles C code to the ELVM assembly language, and the backend compiles the ELVM assembly language to the target language.

[The Quest For Tetris](https://github.com/QuestForTetris/QFT) (QFT) is a collaborative project that creates a computer that runs Tetris in the Game of Life. The computer is created by first defining a virtual Harvard architecture RISC CPU (which I will refer to as the QFT architecture, or simply QFT) that runs an assembly language called QFTASM. The project is explained in detail in this [Stack Exchange post](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life).

In this project, starting from these two existing works, I first wrote the [ELVM QFTASM backend](https://github.com/shinh/elvm/tree/master/tools/qftasm). This module allows the ELVM toolchain to compile ELVM assembly to QFTASM, which means creating a pathway for compiling C code and porting it to the Game of Life. Therefore, I then wrote the Lisp interpreter in C.


### Optimization in Various Layers
However, simply compiling the Lisp interpreter and porting it to QFT resulted in a Game of Life pattern that consumed too much computational resources to run in a practical time. This involved performing optimizations in each of the following layers of the project:

- The C Compiler layer - adding the computed goto feature, preserving address symbols, etc.
- The C layer - implementing and optimizing the Lisp interpreter
- The QFTASM layer - writing a compiler optimizer
- The VarLife layer - the computer architecture
- The Game of Life layer - Hashlife-specific optimization

A more detailed view of the optimizations done in this project is available in [details.md](./details.md).


## Building from Source
### Building the Game of Life Pattern from Source
Instructions for building the Game of Life pattern from the C source ([lisp.c](./src/lisp.c)) is available in [build.md](./build.md).

### Compiling the Lisp Interpreter with GCC
The source code used for building the Game of Life pattern can also be compiled with GCC. The details for this are available in [build.md](./build.md) as well.


## Sample Lisp Programs

- arithmetics.lisp
- object-oriented-like.lisp
- primes.lisp
- backquote-splice.lisp

**arithmetics.lisp**: This program is simple enough to be actually run in a computer in the Game of Life format. It took 6 hours to run on a 32GB-RAM computer (the Hashlife algorithm used for the simulation consumes a lot of memory space) on [Golly](https://en.wikipedia.org/wiki/Golly_(program)). Note that this pattern runs in XX minutes in the VarLife rule, the abtraction layer used when creating the final pattern - details will be explained later.


**object-oriented-like.lisp**: This program is a demonstration of a program writen in an Object-Oriented-Programming-like style, with user-defined classes. Multiple objects can be instantiated, where each object is allocated with distinct memory spaces for their field variables, which can be individually manipulated. Objects are represented as lexical closures over the field variables of the class. This program also features the use of macros to introduce the `.` syntax, dynamically extending the interpreter for expressing programs in a more OOP-like style.


**primes.lisp**: This program calculates and prints a list of prime numbers up to 20.

**backquote-splice.lisp**: This program implements the [backquote macro](http://cl-cookbook.sourceforge.net/macros.html#LtohTOCentry-2), which supports the unquote and unquote-splice operations. Unquote and unquote-splice are each written as `~` and `~@`.


## Screenshots
![An animation of the RAM module of the QFT computer in the VarLife rule, while it is running.](./img/lisp_512B_ram_printstdin_QFT.mc.gif)

An animation of the RAM module of the QFT computer in the VarLife rule, while it is running. The colors of the cells represent the 8 distinct states of the VarLife rule.

![The QFT computer showing the results of the computation.](./img/lisp_512B_ram_printstdin_results_QFT.mc.gif)

The QFT computer showing the results of the computation of the following code:

```lisp
(print (* 3 14))
```

The result is `42`, shown in binary ascii format (0b110100, 0b110010), read in bottom-to-up order.

![The RAM module of the QFT computer converted to a Conway's Game of Life pattern while running.](./img/lisp_512B_ram_printstdin_QFT.mc.png)

The RAM module of the QFT computer converted to a Conway's Game of Life pattern while running. Each "cell" visible here is actually an [OTCA metapixel](https://www.conwaylife.com/wiki/OTCA_metapixel) (OTCAMP) zoomed far away.


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
- lambda, lambda*, macro, macro*
- eval
- eq
- +, -, *, /, mod, <, >

### Lexical Closures
This Lisp implementation supports lexical closures. The implementation of lexical closures is powerful enough to write an object-oriented-like code as shown in [object-oriented-like.lisp](./object-oriented-like.lisp), where classes are represented as lexical closures over the field variables of the class.

### Macros
This Lisp implementation has a macro feature. Lisp macros can be thought as a function that receives code and returns code. Following this design, macros are treated exacly the same as lambdas, except it takes the arguments as raw S-expressions, and evaluates the result twice (the first time to build the expression, and the second time to actually evaluate the builded expression).

### `lambda*` and `macro*`
`lambda*` and `macro*` are byproducts of the lack of proper garbage collection. Both special forms are designed to save memory than their counterparts, `lambda` and `macro`. `lambda*` reuses the graph structure created by the variable namespace when possible. `macro*` builds the expression to be evaluated in a temporary memory region, which is assumed to be overwritten after the macro finishes being evaluated.


## Limitations
As explained in the first section, unfortunately, the running time for the actual Game of Life patterns is unconfirmed.


