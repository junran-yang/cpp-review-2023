**This note is based on CS61 2018 offering**

Calling Convention
==================
## Argument passing and stack frames
* args:
    * first 6  `%rdi, %rsi, %rdx, %rcx, %r8, %r9`
    * 7th and on: stack
* return value: `%rax`
* rule highlights
    * A structure argument that fits in a single machine word (64 bits/8 bytes) 
    is passed in a single register.
    * A structure that fits in two to four machine words (16–32 bytes) is passed
    in sequential registers, as if it were multiple arguments.
    * A structure that’s larger than four machine words is always passed on the 
    stack.
    * Floating point arguments are generally passed in special registers
    * If the return value takes more than eight bytes, then the **caller** 
    reserves space for the return value, and passes the address of that space as
    the first argument of the function. The **callee** will fill in that space 
    when it returns.

## Stack
* `push` 
    * modifies both stack pointer & stack segment
    * `pushq X`: `subq $8, %rsp; movq X, (%rsp)` 
* `pop`
    * `popq X`: `movq (%rsp), X; addq $8, %rsp`
* alignment
    * each stack frame be a multiple of 16 bytes
    * when a callq instruction begins execution, `%rsp` must be 16-byte aligned
    * every function’s entry %rsp address will be 8 bytes off a multiple of 16

## Return address and entry and exit sequence
* entry
    * caller
        1. stores the first six arguments in the corresponding registers
        2. store the surplus arguments (> 6 args, large args) on its stack frame
            * increasing order
            * The 7th argument must be stored at (%rsp) when executing `callq`
        3. saves any caller-saved registers
        4. executes `callq FUNCTION`
            * = `pushq $NEXT_INSTRUCTION; jmp FUNCTION`
            * = `subq $8, %rsp; movq $NEXT_INSTRUCTION, (%rsp); jmp FUNCTION`
* exit
    * callee
        1. places its return value in `%rax`
        2. restores the stack pointer to its value at entry, if necessary
        3. executes `retq`
            * = `popq %rip`
    * caller
        1. cleans up any space it prepared for arguments
        2. restores caller-saved registers if necessary

## Callee-saved registers and caller-saved registers
* Callers can simply use callee-saved registers across function calls; in this 
sense they behave like C++ local variables
* Caller-saved registers behave differently: if a caller wants to preserve the 
value of a caller-saved register across a function call, the caller must 
explicitly save it before the callq and restore it when the function resumes.

## Base pointer
* For simple functions, an optimizing compiler generally treats this like any 
other callee-saved general-purpose register. 
* However, for more complex functions, %rbp is used in a specific pattern that 
facilitates debugging
* entry
    1. `pushq %rbp`
    2. `movq %rsp, %rbp`
* exit
    1. ...
    2. `movq %rbp, %rsp; popq %rbp; retq`
        * = `leave; retq`

## Stack size and red zone
* stack size
    * if a function accesses nonexistent memory near %rsp, the OS assumes it’s 
    for the stack and transparently allocates new memory there 
* red zone
    * small area above the stack pointer (that is, at lower addresses than %rsp) 
    that can be used by the currently-running function for local variables
    * for small functions push and pop instructions end up taking time

Branches
========
* A branch instruction jumps to a new instruction without saving a return 
address on the stack

## unconditional
    * `jmp`, `j`

## conditional
* condition flags
    * branch if come condition holds, represented by condition flags 
    * condition flags are set as a side effect of **every** arithmetic operation
* Arithmetic instructions change part of the %rflags register as a side effect
of their operation.
    - zf: zero flag
    - sf: sign flag, sign bit == 1
    - cf: carry flag, unsigned overflowed
    - of: overflow flag, signed overflowed
* jump
    - j, jump
    - je, jump if equal 0 (check zf)
    - jne, jump if not equal
    - jg, jump if greater
    - jge, jump if greater or equal
    - jl, jump if less
    - jle, jump if less or equal
    - ja, jump if above (unsigned)
    - jae, jump if above or equal (unsigned)
    - jb, jump if below (unsigned)
    - jbe, jump if blow or equal (unsigned)
    - js, jump if sign bit
    - jns, jump if not sign bit
* `test` & `cmp`
    * throw away the result, except for condition codes.
    * `test`: binary-and
        * `testq %rax, %rax`: 
            * load the condition flags appropriately for a single register
            * `testq %rax, %rax; je L`: jumps to L if and only if `%rax` is zero
    * `cmp`: substraction
        * `cmpq %rax, %rbx` evaluates `%rbx - %rax`
        * `cmpq %rax, %rbx; jg L`: jump iff `%rbx` is greater than `%rax`

TIL
===
* If the return value takes more than eight bytes, then the **caller** reserves 
space for the return value, and passes the address of that space as the first 
argument of the function. The **callee** will fill in that space when it returns
* alignment
    * each stack frame be a multiple of 16 bytes
    * when a callq instruction begins execution, `%rsp` must be 16-byte aligned
    * every function’s entry %rsp address will be 8 bytes off a multiple of 16
* Callers can simply use callee-saved registers across function calls; in this 
sense they behave like C++ local variables
* condition flags are set as a side effect of **every** arithmetic operation

Demystification
===============
## Calling convention
* diagram
    ```
    rax - return val
    rsp               rbp 
     |                 |
     V                 V 
      locals    regs   rbp  rip |  args    regs
    |--------|--------|----|----|--------|-------|----
                                |
    |<------ stack frame ------>|
                                |
                                |16-byte aligned
    ```

## Common Instrs & Examples
* `add`, `sub`
    - `addl %rax, %rbx` : `%rbx := %rbx + %rax`
    - `addl $1, %rbx`   : `%rbx := %rbx + $1`
    - `subl %rax, %rbx` : `%rbx := %rbx - %rax`
    - `subl $1, %rbx`   : `%rbx := %rbx - $1`
* `mov`, `lea`
    - `movl %rbx, (%rax)`   : `(%rax) := %rbx`
    - `movl (%rax), %rbx`   : `%rbx := (%rax)`
    - `leaq (%rax), %rbx`   : `%rbx := addr((%rax))`
* `ldr`, `str`
    - On most RISC CPUs
        - `ldr` is used to move data from memory into a CPU register
        - `str` is used to move data from a CPU register into memory
        - `mov` is used to move data from one CPU register to another register
    - On some CISC CPUs
        - `mov` is used both for accessing memory and registers
* `inc`, `dec`, 
    - `incq %rbx`   : `addq $1, %rbx`
    - `incq (%rax)` : `movq (%rax), %rbx; incq %rbx; movq %rbx, (%rax)`
* `push`, `pop`,
    - `pushq %rbx`  : `subq $8, %rsp; movq %rbx, (%rsp)`
    - `popq %rbx`   : `movq (%rsp), %rbx; addq $8, %rsp`
* `call`, `ret`
    - `call ADDR`   : `push [next instr pointer]; j ADDR`
    - `ret`         : `pop %rip`
* unconditional branch
    * `j`
        - e.g. `j .L3`
* conditional branch
    * `cmp`
        - e.g. `cmpq %rax %rbx`: evaluate `%rbx - %rax`, sets condition flags
    * `je`: jump if equal (zero)
    * `jg`: jump if `%rbx - %rax > 0`
