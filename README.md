# Two-Pass Custom Assembler

A custom two-pass assembler implemented in **C**, built to translate a theoretical assembly language into machine code. The project demonstrates a deep understanding of low-level system architecture, dynamic memory management, bitwise operations, and modular software design.

## 🚀 Key Features

* **Macro Pre-Processor:** Successfully identifies, extracts, and deploys macro definitions (`mcro` ... `mcroend`) before the assembly passes begin.
* **Two-Pass Translation:**
  * **First Pass:** Scans the code to build a comprehensive Symbol Table, calculates Instruction Counters (IC) and Data Counters (DC), and validates syntax.
  * **Second Pass:** Resolves relative and direct labels, links external/entry variables, and constructs the final binary machine words.
* **Robust Error Handling:** Detects and reports compilation errors with precise line numbers (e.g., undefined labels, syntax errors, illegal commas, memory overflow) and prevents output generation if errors exist.
* **Dynamic Memory Management:** Utilizes safe, custom wrapper functions for memory allocation (`malloc`, `realloc`) and ensures zero memory leaks using linked lists for symbols, data images, and macros.

## 🛠️ Architecture & Modules

The system is highly modular, divided into distinct logical units:
* `assembler.c`: Main entry point orchestrating the passes.
* `pre_assembler.c`: Handles the macro deployment phase.
* `first_pass.c` & `second_pass.c`: The core logic for encoding the assembly commands and addressing modes.
* `structs.c`: Manages the dynamic linked lists (Symbol Table, Macro List, Data Image).
* `output_files.c`: Generates the final output files (`.ob`, `.ent`, `.ext`).
* `utilities.c`: Helper functions for string manipulation, file handling, and memory.

## 💻 Technologies & Concepts
* **Language:** C (ANSI Standard)
* **Concepts:** Bitwise Operations, Pointers, Linked Lists, File I/O, Parsing algorithms.
* **Build Tool:** `Makefile` for streamlined compilation.

## ⚙️ How to Run

1. Clone the repository:
   ```bash
   git clone [https://github.com/sefatieyal-max/Assembler-Project.git](https://github.com/sefatieyal-max/Assembler-Project.git)
