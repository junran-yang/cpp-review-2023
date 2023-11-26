Demystification
===============
## Address vs Value
* regular
    * register
        - e.g. `%rax`
    * memory
        - e.g. `0x18(%rax,%rbx,4)`, `4`
        - symbol: 
            - `g`: conventional program
            - `g(%rip)` - position independent executable (PIE)
    * immediate 
        - e.g. `$4`, `$g`
* jump
    * register
        - e.g. `*%rax`
    * memory
        - e.g. `*0x18(%rax,%rbx,4)`, `*4`, `*g`, `*g(%rip)`
    * immediate
        - e.g. `4`, `g`
* side notes
    * think (register, memory) as variables in c++, literals as literals in c++
    * `g` label represents an address
* examples
    * register & literals only
        - `addl %rax, %rbx`   : `%rbx := %rbx + %rax`
        - `addl $1, %rbx`     : `%rbx := %rbx + $1`
    * address & value in memory
        - `movl %rbx, (%rax)` : `(%rax) := %rbx`
        - `movl (%rax), %rbx` : `%rbx := (%rax)`
        - `movl $1, (%rax)`   : `(%rax) := $1`

## Size
* assembly instructions are generally suffixed with the letters "b", "s", "w", 
"l", "q" or "t" to determine what size operand is being manipulated.
* If the suffix is not specified, and there are no memory operands for the 
instruction, GAS infers the operand size from the size of the destination 
register operand (the final operand).
* `movzbl` moves an 8-bit quantity (a byte) into a 32-bit register (a longword) 
with zero extension

* instructions
    * `q, l, w, b`
    * 64 - `addq`
    * 32 - `addl`
    * 16 - `addw`, `adds`
    * 8  - `addb`
    * diff meaning for floating point
* registers, part 1
    * `r, e, -, l/h`
    * 64 - `rax, rbx, rcx, rdx, rsi, rdi`
    * 32 - `eax, ebx, ecx, edx, esi, edi`
    * 16 - `ax, bx, cx, dx, si, di`
    * 8  - `al, bl, cl, dl, sil, dil`
* registers, part 2
    * `-, d, w, b`
    * 64 - `r8, r9`
    * 32 - `r8d, r9d`
    * 16 - `r8w, r9w`
    * 8  - `r8b, r9b`

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
