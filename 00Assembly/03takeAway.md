Take-away
=========
## address & value
* register
    - e.g. `%rax`
* memory
    - e.g. `0x18(%rax,%rbx,4)`, `4`
    - symbol: 
        - `g`: conventional program
        - `g(%rip)` - position independent executable (PIE)
* immediate 
    - e.g. `$4`, `$g`
    - jump: e.g. `4`, `g`

## size
```
64      32      16      8       - bits
q       l       w       b       - instr
%rax    %eax    %ax     %al     - reg
%r8     %r8d    %r8w    %r8b    - reg
```
* `sizeof(bool)` == `sizeof(char)` == 1 byte
* Each address identifies a single byte (eight bits) of storage.


TIL
===
* `mov` and size
    * `mov` to 8-, 16-, 32-bit registers
        * loading into 32-bit reg sets the upper 32 bits to zero
        * loading into 16- or 8-bit reg leaves all other bits unchanged
    * `mov` from 8-, 16-, 32-bit quantities
        * `movzbl` - src: b(8), dest: l(32), zero-extension(upper 24)
        * `movslq` - src: l(32), dest: q(64), sign-extension(upper 32)
        * `movzbl` - no need
    * sign extension (filling with 1's or 0's)
* instr
    * `incl (%rax)`: `movl (%rax), %ebx; incl %ebx; movl %ebx, (%rax)`
    * `leaq 0x18(%rax,%rbx,4), %rcx`: `%rcx := %rax + 2 * %rbx`
* labels
    * used to compute addresses
* directives
    * instructions to the assembler
    * used w/ labels
        ```asm
            .globl  _Z1fiii
            .type   _Z1fiii, @function
        _Z1fiii:
            ...
            .size   _Z1fiii, .-_Z1fiii
        ```
    * `.long`
        ```asm
        g:
            .long 10
        ```
    * `.text`, `.data`

* disassembly 
    * assembly that is disassembled from executable instructions
    * GDB, or `objdump -d <obj file>`, or `objdump -S <obj file>`
    * no intermediate labels or directives; they're compiled away
    * linking changes the address
* position independent executables
    * a security feature
* assembly standard
    * AT&T & Intel 
* RISC vs CISC
    * RISC - reduced instruction set computer
    * CISC - complex instruction set computer
    * The fundamental difference between the two is that RISC has less number of
    instructions, with each one capable of performing a single operation, while
    CISC has a large number of complex instructions capable of carrying out
    multiple internal operations.

* calling convention: return
    * If the return value takes more than eight bytes, then the **caller** 
    reserves space for the return value, and passes the address of that space as 
    the first argument of the function. The **callee** will fill in that space 
    when it returns
* calling convention: alignment
    * each stack frame be a multiple of 16 bytes
    * when a callq instruction begins execution, `%rsp` must be 16-byte aligned
    * every function’s entry %rsp address will be 8 bytes off a multiple of 16
* calling convention: save register
    * Callers can simply use callee-saved registers across function calls; in 
    this sense they behave like C++ local variables
* condition flags 
    * are set as a side effect of **every** arithmetic operation
