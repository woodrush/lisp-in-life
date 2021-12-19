# Lisp in Conway's Game of Life

It's not Conway's Game of Life implemented in Lisp - it's a Lisp interpreter implemented in Conway's Game of Life.

The entire pattern is viewable on the browser [here](https://woodrush.github.io/lisp-in-life).


## Running Lisp and C on the Game of Life
This repository contains a Conway's Game of Life pattern that runs a Lisp interpreter.
The Lisp program is provided by editing certain cells within the pattern to represent the ASCII-encoding of the Lisp program.
The interpreter writes its standard output to the bottom end of the RAM module, which can be directly examined in a Game of Life viewer.
The Lisp implementation supports lexical closures and macros, allowing one to write Lisp programs in a Lisp-like taste, as far as the memory limit allows you to.

The [Lisp interpreter](./src/lisp.c), written in C, is compiled to an assembly language for a CPU architecture implemented in the Game of Life, which is a modification of the computer used in the [Quest For Tetris](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life/142673#142673) (QFT) project.
The compilation is done using [ELVM](https://github.com/shinh/elvm) (the Esoteric Language Virtual Machine). (I implemented the Game of Life backend for ELVM myself for this project. The backend is further modified (available in the submodule) for supporting QFT-specific optimizations.)

Using the build system for this project, you can also compile your own C11-compatible C code and run in on Conway's Game of Life.
Please see [build.md](./build.md) for more details.


### Implementations and Optimizations
Generating a short enough Lisp interpreter assembly code and a Game of Life pattern that runs in a reasonable amount of time required a lot of effort.
This required optimizations and improvements in every layer of the project, including the C compiler layer, the CPU architecture layer, Game of Life layer, the C program layer, etc.
Examples of these optimizations include:

- The C Compiler layer - adding the [computed goto](https://en.wikipedia.org/wiki/Goto#Computed_GOTO_and_Assigned_GOTO) feature to the C compiler, preserving address symbols to be used after compilation, etc.
- The C layer (the [Lisp interpreter](./src/lisp.c)) - using a string hashtable and binary search for Lisp symbol lookup, minimization of stack region usage with union memory structures, careful memory region map design, etc.
- The QFTASM layer - writing a [compiler optimizer](./src/qftasmopt.py) to optimize the length of the assembly code
- The VarLife layer - creating a lookup table architecture for faster ROM access, expanding the size and length of the RAM module, adding new opcodes, etc.
- The Game of Life layer - [Hashlife](https://en.wikipedia.org/wiki/Hashlife)-specific optimization

A more detailed description of the optimizations done in this project is available in [details.md](./details.md).
Details for building the interpreter's C source code is available in [build.md](./build.md).

An overview of the build flow is as follows (click for a larger view):

![The toolchain used for this project](./img/toolchain.png)


## Screenshots
![An overview of the entire architecture.](./img/ss1.png)

An overview of the entire architecture.

![An overview of the CPU and its surrounding units.](./img/ss2.png)

An overview of the CPU and its surrounding modules. On the top are the ROM modules, with the lookup module on the right, and the value modules on the left. On the bottom left is the CPU. On the bottom right is the RAM module.

This pattern is the VarLife version of the architecture. VarLife is an 8-state cellular automaton defined in the [Quest For Tetris](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life/142673#142673) (QFT) Project, which is used as an intermediate layer to create the final Conway's Game of Life pattern. The colors of the cells indicate the 8 distinct states of the VarLife rule.

The architecture is based on [Tetris8.mc](https://github.com/QuestForTetris/QFT/blob/master/Tetris8.mc) in the [original QFT repository](https://github.com/QuestForTetris/QFT). Various modifications were made from the original architecture, such as removing and adding new opcodes, creating a new lookup table architecture for the ROM module, reducing the bit length of the instruction size, extending the RAM address space, etc.

![The Conway's Game of Life version of the architecture, converted from the VarLife pattern.](./img/ss3.png)

The Conway's Game of Life version of the architecture, converted from the VarLife pattern.
What appears to be a single cell in this image is actually an [OTCA metapixel](https://www.conwaylife.com/wiki/OTCA_metapixel) zoomed away to be shown 2048 times smaller.

![A close-up view of a part of the ROM module in the Conway's Game of Life version.](./img/ss4.png)

A close-up view of a part of the ROM module in the Conway's Game of Life version.
Each pixel in the previous image is actually this square-shaped structure shown in this image.
These structures are [OTCA metapixels](https://www.conwaylife.com/wiki/OTCA_metapixel), which can be seen to be in the On and Off meta-states in this image.
The OTCA metapixel is a special Conway's Game of Life pattern that can emulate cellular automatons with customized rules.
The original VarLife pattern is simulated this way so that it can run in Conway's Game of Life.

![A close-up view of a part of the ROM module in the Conway's Game of Life version.](./img/ss5.png)

The ALU unit in the CPU. From the left are the modules for the `ANT`, `XOR`, `SRE`, `SRU`, `SUB`, `ADD`, `MLZ`, and the `MNZ` opcodes.

The `SRE` and the `SRU` opcodes were newly added for this project.

![A video of the RAM module of the QFT computer in the VarLife rule in action.](./img/lisp_512B_ram_printstdin_QFT.mc.gif)

A video of the RAM module of the QFT computer in the VarLife rule in action.

![The QFT computer showing the results of the computation of `(print (* 3 14))`.](./img/ss6.png)

The computer showing the results of the following Lisp program:

```lisp
(print (* 3 14))
```

The result is `42`, shown in binary ascii format (0b110100, 0b110010), read in bottom-to-up order.


## Pattern Files
| Program                                                | VarLife Pattern                                                       | Conway's Game of Life Pattern                                                                    |
|--------------------------------------------------------|-----------------------------------------------------------------------|--------------------------------------------------------------------------------------------------|
| [object-oriented-like.lisp](object-oriented-like.lisp) | [QFT_object-oriented-like.mc](./patterns/QFT_object-oriented-like.mc) | [QFT_object-oriented-like_metafied.mc](./patterns/metafied/QFT_object-oriented-like_metafied.mc) |
| [z-combinator.lisp](z-combinator.lisp)                 | [QFT_z-combinator.mc](./patterns/QFT_z-combinator.mc)                 | [QFT_z-combinator_metafied.mc](./patterns/metafied/QFT_z-combinator_metafied.mc)                 |
| [backquote-splice.lisp](backquote-splice.lisp)         | [QFT_backquote-splice.mc](./patterns/QFT_backquote-splice.mc)         | [QFT_backquote-splice_metafied.mc](./patterns/metafied/QFT_backquote-splice_metafied.mc)         |
| [backquote.lisp](backquote.lisp)                       | [QFT_backquote.mc](./patterns/QFT_backquote.mc)                       | [QFT_backquote_metafied.mc](./patterns/metafied/QFT_backquote_metafied.mc)                       |
| [primes-print.lisp](primes-print.lisp)                 | [QFT_primes-print.mc](./patterns/QFT_primes-print.mc)                 | [QFT_primes-print_metafied.mc](./patterns/metafied/QFT_primes-print_metafied.mc)                 |
| [primes.lisp](primes.lisp)                             | [QFT_primes.mc](./patterns/QFT_primes.mc)                             | [QFT_primes_metafied.mc](./patterns/metafied/QFT_primes_metafied.mc)                             |
| [print.lisp](print.lisp)                               | [QFT_print.mc](./patterns/QFT_print.mc)                               | [QFT_print_metafied.mc](./patterns/metafied/QFT_print_metafied.mc)                               |

Pattern files preloaded with various Lisp programs are available here.
Details of the Lisp programs are explained in the next section.
Detailed statistics such as the running time and the memory consumption are available in the "Running Times and Stats" section.

The patterns can be simulted on the Game of Life simulator [Golly](https://en.wikipedia.org/wiki/Golly_(program)).
VarLife is an 8-state cellular automaton defined in the [Quest For Tetris](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life/142673#142673) (QFT) Project, explained in detail in the next section.
VarLife patterns can be simulated on Golly as well, which requires additional settings. Please see [build.md](./build.md) for a detailed description.


### Descriptions of the Lisp Programs

- **object-oriented-like.lisp**:
    This example creates a structure similar to classes in Object-Oriented Programming, using closures.

    - The class has methods and field variables, where each instance carries distinct and persistent memory locations of their own.
    The example instantiates two counters and concurrently modifies the value held by each instance.
    - New syntaxes for instantiation and method access (`(new classname)` and `(. instance methodname)`) are introduced using macros and functions.

    The Lisp interpreter's variable scope and the macro feature is powerful enough to manage complex memory management,
    and even providing a new syntax to support the target paradigm.

- **z-combinator.lisp**:
Demonstration of the [Z Combinator](https://en.wikipedia.org/wiki/Fixed-point_combinator#Strict_fixed-point_combinator) to implement a factorial function
using [anonymous recursion](https://en.wikipedia.org/wiki/Anonymous_recursion).

- **backquote-splice.lisp**:
Implements the [backquote macro](http://cl-cookbook.sourceforge.net/macros.html#LtohTOCentry-2) used commonly in Lisp to construct macros.
It also supports the unquote and unquote-splice operations, each written as `~` and `~@`.

- **primes.lisp**: Prints a list of prime numbers up to 20. This example highlights the use of the `while` syntax.


The contents of print.lisp is quite straightforward.
backquote.lisp and primes-print.lisp are similar to backquote-splice.lisp and primes.lisp, mainly included for performance comparisons.
backquote.lisp doesn't implement the unquote-splice operation, and demonstrates some more examples.
primes-print.lisp reduces the number of list operations to save memory usage.



### What is VarLife?
VarLife is an 8-state cellular automaton defined in the [Quest For Tetris](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life/142673#142673) (QFT) Project.
It is used as an intermediate layer to generate the final Conway's Game of Life pattern;
the computer is first created in VarLife, and then converted to a Game of Life pattern.

When converting VarLife to Conway's Game of Life, each VarLife cell is mapped to an [OTCA Metapixel](https://www.conwaylife.com/wiki/OTCA_metapixel) (OTCAMP).
The OTCA Metapixel is a special pattern that is capable of emulating a different game-of-life-like rule within the Game of Life.
Each cell carries a binary meta-state and a pre-programmed rule.

The conversion from VarLife to the Game of Life is done in a way so that the behavior of the states and generations of the VarLife pattern
matches exactly with the converted Game of Life pattern's meta-states and meta-generations.
Therefore, it is enough to verify the behavior of the VarLife pattern to verify the behavior of the Game of Life pattern.


## Running Times and Stats
**VarLife Patterns**
| Lisp Program                                           | Pattern (VarLife)                                                     | #QFT CPU Cycles | QFT Memory Usage (QFT bytes) | #Halting Generations (VarLife) | Running Time (VarLife) | Memory Usage (VarLife)   |
|--------------------------------------------------------|-----------------------------------------------------------------------|-----------------|------------------------------|--------------------------------|------------------------|--------------------------|
| [print.lisp](print.lisp)                               | [QFT_print.mc](./patterns/QFT_print.mc)                               |           4,425 |                           92 |            105,413,068 (exact) |             1.159 mins |                  5.0 GiB |
| [z-combinator.lisp](z-combinator.lisp)                 | [QFT_z-combinator.mc](./patterns/QFT_z-combinator.mc)                 |          58,883 |                          544 |          1,700,000,000         |             9.823 mins |                 23.4 GiB |
| [backquote-splice.lisp](backquote-splice.lisp)         | [QFT_backquote-splice.mc](./patterns/QFT_backquote-splice.mc)         |         142,353 |                          869 |          4,100,000,000         |            20.467 mins | 27.5 GiB (max. capacity) |
| [backquote.lisp](backquote.lisp)                       | [QFT_backquote.mc](./patterns/QFT_backquote.mc)                       |         142,742 |                          876 |          4,100,000,000         |            21.663 mins | 27.5 GiB (max. capacity) |
| [object-oriented-like.lisp](object-oriented-like.lisp) | [QFT_object-oriented-like.mc](./patterns/QFT_object-oriented-like.mc) |         161,843 |                          838 |          4,673,000,000         |            22.363 mins | 27.5 GiB (max. capacity) |
| [primes-print.lisp](primes-print.lisp)                 | [QFT_primes-print.mc](./patterns/QFT_primes-print.mc)                 |         281,883 |                          527 |          8,880,000,000         |            27.543 mins | 27.5 GiB (max. capacity) |
| [primes.lisp](primes.lisp)                             | [QFT_primes.mc](./patterns/QFT_primes.mc)                             |         304,964 |                          943 |          9,607,100,000         |            38.334 mins | 27.5 GiB (max. capacity) |

**Conway's Game of Life (GoL) Patterns**
| Lisp Program                                           | Pattern (GoL)                                                                                    | #QFT CPU Cycles | QFT Memory Usage (QFT bytes) | #Halting Generations (GoL) | Running Time (GoL) | Memory Usage (GoL)       |
|--------------------------------------------------------|--------------------------------------------------------------------------------------------------|-----------------|------------------------------|----------------------------|--------------------|--------------------------|
| [print.lisp](print.lisp)                               | [QFT_print_metafied.mc](./patterns/metafied/QFT_print_metafied.mc)                               |           4,425 |                           92 |         3,724,032,866,304  |      382.415 mins  | 27.5 GiB (max. capacity) |
| [z-combinator.lisp](z-combinator.lisp)                 | [QFT_z-combinator_metafied.mc](./patterns/metafied/QFT_z-combinator_metafied.mc)                 |          58,883 |                          544 |        60,057,600,000,000  |                 -  |                        - |
| [backquote-splice.lisp](backquote-splice.lisp)         | [QFT_backquote-splice_metafied.mc](./patterns/metafied/QFT_backquote-splice_metafied.mc)         |         142,353 |                          869 |                         -  |                 -  |                        - |
| [backquote.lisp](backquote.lisp)                       | [QFT_backquote_metafied.mc](./patterns/metafied/QFT_backquote_metafied.mc)                       |         142,742 |                          876 |                         -  |                 -  |                        - |
| [object-oriented-like.lisp](object-oriented-like.lisp) | [QFT_object-oriented-like_metafied.mc](./patterns/metafied/QFT_object-oriented-like_metafied.mc) |         161,843 |                          838 |                         -  |                 -  |                        - |
| [primes-print.lisp](primes-print.lisp)                 | [QFT_primes-print_metafied.mc](./patterns/metafied/QFT_primes-print_metafied.mc)                 |         281,883 |                          527 |                         -  |                 -  |                        - |
| [primes.lisp](primes.lisp)                             | [QFT_primes_metafied.mc](./patterns/metafied/QFT_primes_metafied.mc)                             |         304,964 |                          943 |                         -  |                 -  |                        - |

The running times for each program are shown above. The [Hashlife](https://en.wikipedia.org/wiki/Hashlife) algorithm used for the simulation requires a lot of memory in exchange of speedups.
The simulations were run on a 32GB-RAM computer, with Golly's memory usage limit set to 28000 MB, and the default base step to 2 (configurable from the preferences).
The memory usage was measured by Ubuntu's activity monitor.
The number of CPU cycles and the QFT memory usage was obtained by running the QFTASM interpreter on the host PC.
The QFT memory usage shows the number of RAM addresses that were written at least once.
The memory usage is measured in QFT bytes. Since one RAM memory slot includes 16 bits, 1 QFT byte equals 16bits.

Since one OTCA Metapixel has a bounding box size of 2048x2048 (when tiled together), converting the VarLife pattern to a Conway's Game of Life pattern will expand the pattern size by a factor of 2048x2048.
Also, since the OTCA Metapixel has a phase of 35,328 (according to [LifeWiki - OTCA metapixel](https://www.conwaylife.com/wiki/OTCA_metapixel)), the number of Conway's Game of Life generations required becomes 35,328 times the number of required VarLife generations.
Therefore, running the Game of Life (GoL) version is significantly slower than running the VarLife version.

After the program counter is set to 65535 and the program exits, no more ROM and RAM I/O signals become apparent in the entire module.
This makes the VarLife pattern becomes completely stationary, where every pattern henceforth becomes completely identical.
Defining this as the halting time for the calculation, the pattern for [print.lisp](print.lisp) halts at exactly 105,413,068 VarLife generations.
By generation 105,387,540, the value 65535 gets written to the program counter. At generation 105,413,067, the last signal becomes just one step from disappearing, and at generation 105,413,068 and onwards, the pattern becomes completely stationary and every pattern becomes identical to each other.
In the Game of Life version, since the OTCA Metapixel continues running indefinitely, the pattern does not become completly stationary, but the meta-states of the OTCA Metapixels will become completely stationary, since it is an emulation of the VarLife pattern.
Note that the halting times for programs other than print.lisp is just a sufficient number of generations, and not the exact values.

The required number of generations per CPU cycle depends on many factors such as the ROM and RAM addresses and the types of opcodes, since the arriving times of the I/O signals depend on factors such as these as well. This makes the number of generations required for the program to halt become different between each program.
For example, print.lisp has a rate of 23822.16 generations per CPU cycle (GpC), but z-combinator.lisp has a rate of 28870.81 GpC, and primes-print.lisp has 31502.43 GpC. 23822.16 GpC is in fact insufficient for z-combinator.lisp to finish running, and 28870.81 is also insufficient for primes-print.lisp to finish running.


### Loading and Running Your Own Lisp and C Programs
You can load your own Lisp program into the Lisp interpreter pattern and run it on Game of Life. This is explained in detail in [./build.md](./build.md).

You can also compile your own C program and run it on the Game of Life, by using the build system used for [hello.c](./misc/hello/hello.c)).
Further details are available in [build.md](./build.md).


## Building from Source
Instructions for building the Game of Life pattern from the Lisp interpreter's C source ([lisp.c](./src/lisp.c)) is available in [build.md](./build.md).

The Lisp interpreter used in the Game of Life pattern can also be compiled with GCC. The details for this are available in [build.md](./build.md) as well.

### Tests
There are tests to check the behavior of the Lisp interpreter.
There is a test for checking the QFTASM-compiled Lisp interpreter using the QFTASM interpreter, and a test for checking the GCC-compiled Lisp interpreter on the host pc.
To run these tests, use the following commands:

```sh
make test             # Run the tests for the QFTASM-compiled Lisp interpreter, using the QFTASM interpreter
make test_executable  # Run the tests for the executable compiled by GCC
```

Running `make test` requires [Hy](https://github.com/hylang/hy), a Clojure-like Lisp implemented in Python available via `pip install hy`.
Some of the tests compare the output results of Hy and the output of the QFTASM Lisp interpreter.

The tests were run on Ubuntu and Mac.


## Details of the Lisp Interpreter
### Special Forms and Builtin Functions

- define
- if
- quote
- car, cdr
- cons
- list
- atom
- print
- progn
- while
- lambda, macro
- eval
- eq
- +, -, *, /, mod, <, >

### Lexical Closures
This Lisp implementation supports lexical closures.
The implementation of lexical closures is powerful enough to write an object-oriented-like code as shown in [object-oriented-like.lisp](./object-oriented-like.lisp),
where classes are represented as lexical closures over the field variables and the class methods.

### Macros
This Lisp implementation has a macro feature. Lisp macros can be thought as a function that receives code and returns code.
Following this design, macros are treated exacly the same as lambdas, except that it takes the arguments as raw S-expressions,
and evaluates the result twice (the first time to build the expression, and the second time to actually evaluate the builded expression).
