# Embedded Learner

A practical, continuously evolving knowledge base for Embedded Systems Engineering — covering Embedded C, microcontroller programming, hardware fundamentals, development resources, and interview preparation.

This repository brings together hands-on code, study material, reference documents, and practice questions in one place. It is intended to support a structured learning path from C fundamentals to working with microcontrollers and embedded peripherals.

## Learning Areas

The repository is organised around the core skills used in embedded development.

### 1. C Programming for Embedded Systems
- Data types, operators, control flow, functions, and storage classes
- Arrays, strings, pointers, and pointer arithmetic
- Structures, unions, enumerations, and bitwise operations
- Memory layout, alignment, and low-level data handling
- Writing readable, modular, and reusable C code

### 2. Microcontroller Fundamentals
- Microcontroller architecture and memory-mapped registers
- GPIO configuration and digital input/output
- Clock and peripheral configuration
- Interrupts and interrupt service routines
- Timers, counters, and basic peripheral concepts

### 3. Peripheral Programming
- UART / USART serial communication
- SPI and I²C communication fundamentals
- ADC and other common MCU peripherals
- Polling, interrupt-driven, and (where applicable) DMA-based approaches

### 4. Embedded Software Concepts
- Bare-metal programming and register-level development
- HAL-based development and peripheral drivers
- Debugging, testing, and understanding compiler output
- RTOS fundamentals and task-based design
- Reading datasheets and reference manuals effectively

## Getting Started

### Prerequisites

You can explore the C examples with a standard C toolchain. For microcontroller projects, install the toolchain and IDE appropriate to the target board.

Common tools used in embedded workflows include:

- **C compiler:** GCC / Arm GNU Toolchain
- **IDE:** STM32CubeIDE or another vendor-supported IDE
- **Debugging:** ST-LINK and a compatible debugger
- **Documentation:** MCU datasheets, reference manuals, and ARM technical documentation

Tool requirements may differ between individual examples.

### Clone the repository

git clone https://github.com/keerthi176/Embedded_learner.git
cd Embedded_learner

### Explore the code

Start with the C exercises, then move to the microcontroller examples and supporting documentation:

Embedded_learner/
├── Books/
├── C_learner/
├── HAL_program/
│   └── stm_learner/
├── datasheets_&_reference_manual/
└── question_bank/

For a C source file, compile it with a C compiler (adjust the filename and options as needed):

gcc -Wall -Wextra -std=c11 C_learner/your_file.c -o learner
./learner

Microcontroller projects must be built using the project-specific configuration, device support package, and toolchain. A desktop GCC command alone is not sufficient to build an MCU firmware project.


## Suggested Learning Workflow

1. **Build the C foundation** — practise pointers, arrays, structures, bitwise operations, and memory concepts.
2. **Understand the hardware** — read the target MCU datasheet and reference manual before configuring registers or peripherals.
3. **Start with GPIO** — configure an LED output and a digital input.
4. **Add communication interfaces** — explore UART, SPI, and I²C.
5. **Learn interrupts and timing** — work with external interrupts, timers, and non-blocking code.
6. **Explore software architecture** — organise code into drivers and modules, then progress to RTOS concepts.
7. **Practise explaining your work** — use the question bank to revise concepts and prepare for technical discussions.


## How to Use This Repository

- **Learners:** Follow the workflow above and practise the examples rather than only reading them.
- **Interview preparation:** Use the question bank to test your understanding, then write and debug code for the concepts.
- **Working engineers:** Use the reference material as a quick starting point, and always verify device-specific details against the manufacturer's latest documentation.
- **Contributors:** Add focused examples with descriptive filenames, comments, and build or test instructions.

## Contributing

Contributions that make the material more accurate, useful, and easier to follow are welcome.

1. Fork the repository.
2. Create a branch for your change.
3. Add or improve an example, explanation, or resource.
4. Include the target MCU/board and required tools when relevant.
5. Test code where practical and document any hardware dependencies.
6. Submit a pull request describing the change.

For code contributions, prefer:
- Clear filenames and meaningful function/variable names
- Comments that explain intent or non-obvious hardware behaviour
- No hard-coded assumptions without documenting the target
- Build instructions and expected output where applicable

## Important Notes

- This repository is a learning and reference resource; examples may require adaptation for a specific MCU, board revision, clock configuration, or SDK version.
- Always consult the official datasheet and reference manual before using register addresses, peripheral bits, or electrical limits.
- Respect the licences and copyrights of third-party books and documents. Do not redistribute material unless you have permission to do so; link to the official publisher or author resource where appropriate.

## Future Improvements

Possible areas to expand as the repository grows:

- Topic-wise notes for Embedded C and ARM Cortex-M
- More register-level and HAL-based peripheral examples
- UART, SPI, I²C, CAN, ADC, timer, and interrupt exercises
- RTOS examples and task synchronisation
- Debugging notes, test procedures, and practical mini-projects
- Clear board-wise build and wiring instructions
