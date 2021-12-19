# Implementation Details
This writing is a more detailed explanation of the implementation of Lisp in Life.

## Overview of the Build Flow
![The toolchain used for this project](./img/toolchain.png)

The final Game of Life pattern (GoL) for the Lisp interpreter is created by porting C code into a Game of Life pattern. The most important step for accomplishing this was to create a module that would provide a link between two existing works, [ELVM](https://github.com/shinh/elvm) and [The Quest For Tetris](https://github.com/QuestForTetris/QFT) (QFT).

[ELVM](https://github.com/shinh/elvm) (the Esoteric Language Virtual Machine) is a toolchain intended to compile C programs to various target languages. It consists of a frontend and a backend, where the frontend compiles C code to the ELVM assembly language, and the backend compiles the ELVM assembly language to the target language.

[The Quest For Tetris](https://github.com/QuestForTetris/QFT) (QFT) is a collaborative project that creates a computer that runs Tetris in the Game of Life. The computer is created by first defining a virtual Harvard architecture RISC CPU (which I will refer to as the QFT architecture, or simply QFT) that runs an assembly language called QFTASM. The project is explained in detail in this [Stack Exchange post](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life/142673#142673).

In this project, starting from these two existing works, I first wrote the [ELVM QFTASM backend](https://github.com/shinh/elvm/tree/master/tools/qftasm). This module allows the ELVM toolchain to compile ELVM assembly to QFTASM, which means creating a pathway for compiling C code and porting it to the Game of Life. Therefore, I then wrote the Lisp interpreter in C.

A more detailed description of the implementations of this project is available in [details.md](./details.md).


### How is the Program and the Standard Input Provided? How is the Output Read Out? 
The Lisp interpreter accepts the Lisp code from the standard input and writes the results to the standard output.
Both the standard input and output reside within the RAM module, with memory addresses 290 and 790, respectively.

Since each byte of the RAM module can be ordered arbitrarily in the CPU's architecture, the RAM is arranged so that the standard output is written at the very bottom of the RAM module, and proceeds upwards. Therefore, the contents of the RAM can easily be observed in a Game of Life viewer by directly examining the bottom of the RAM module. A script for examining the output, as well as the input and other register values are available in the [QFT-devkit](https://github.com/woodrush/QFT-devkit) (also included as a submodule in this repository).

Since RAM has 16 bits of memory per memory address, it allows to fit two ASCII-encoded characters per one address. Therefore, the standard input is read out by reading two characters per address. For the standard output, one character is written to one address for aesthetic reasons, so that the characters can be directly observed in a Game of Life viewer the pattern more easily. Also, for the standard output to proceed upwards within the RAM module pattern, the memory pointer for the standard output proceeds backwards in the memory space, while the pointer for the standard input proceeds forwards in the memory space.


## Optimizations
### The C Compiler layer
- Added the computed goto feature to ELVM
- Modified the compiler to preserve and output memory address symbols and program address symbols, for their usage in the compiler optimization tool in the QFTASM layer

### The C layer (the implementation of the Lisp interpreter)
- Usage of binary search and hashtables for string comparison
- Recycling memory for unused objects
  - This was done by the special forms `lambda*` and `macro*`, although they were removed from the final Lisp programs.
- Miscellaneous C optimizations (to compensate for the lack of compiler optimizations in the compiler layer)
  - Extensive use of global variables, which leads to prevention of indirect references
  - Specially defined structs and unions for the stack region of the main evaluation function

### The QFTASM layer
- Created a compiler optimization tool to reduce the QFTASM assembly size

### The Varlife layer (the computer architecture)
- Recreated the ROM module with a cascaded hash table structure for the ROM module to save space
- Added new optimized instructions to the ALU, and removed unused ones
- Optimizations based on the fact that the absence of signals represents 0
- Reduced the ROM word size from 58 bits to 45 bits
- Extended the ROM and RAM address space from 9,7-bit to 12,10-bit

### The Game of Life layer
- Improved the metafier script so that it could handle large patterns, by improving the memory usage of the memoization procedure
- Aligned the pattern to 8x8 Varlife cells, to reduce the resulting [Macrocell](https://www.conwaylife.com/wiki/Macrocell) format output (Since the Macrocell format is the data structure used for the [Hashlife](https://en.wikipedia.org/wiki/Hashlife) algorithm, I believe this serves for optimization in the Hashlife layer)
