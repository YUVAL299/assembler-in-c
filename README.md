# Assembler in C

This project is a two-pass assembler implemented in C, developed as part of the **Systems Programming Laboratory** course at The Open University of Israel.

It reads assembly source files (`.as`), builds a symbol table, handles directives and macros, and generates machine code output files.

## Features

- Two-pass assembly process
- Symbol table and label resolution
- Support for directives: `.data`, `.string`, `.extern`, `.entry`
- Macro processing
- Output of object (`.ob`), entry (`.ent`), and extern (`.ext`) files

## Getting Started

### Prerequisites

- GCC or any C compiler
- Command-line interface

### Building the Project

In the terminal, run:

```bash
gcc -o assembler *.c
