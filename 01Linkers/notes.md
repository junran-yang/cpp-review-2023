Section 1 + Lab 1: "Compile"
============================
* `g++ hello.cc --std=gnu++14 -isystem </path/to/include> -L </path/to/lib> -l<lib> -o <binary>`
    * g++ vs gcc
    * g++ is the compiler from the GNU compiler suite
    * -isystem include directory
    * -I. include current directory
    * -L lib path, not the lib to link against
    * -l<lib>, lib to link against

Section 2 + Lab 2: Link
=======================
* `g++` is a front end; behind the facade, it can preprocess, compile, and link
* link
    * `g++ x.o y.o z.o -o demo`, perform linking b/c only object files supplied
    * link our files, **and** standard files, e.g. standard lib, runtime lib
    * ld is the linking tool underneath the hood.
    * `g++ x.o y.o z.o -o demo -v`, to look at ld command

Section 3 + Lab 3: Object files (Symbols)
=========================================
* An object file contains
    * code (think about cs61 2018 Asm 1, disassembly)
    * information for creating and initializeing global variables
    * symbols for identifying code and variables
        * **string** & **offset** of where to find that item in the obj file

* linking
    * records which symbols are undefined, and when it finds a definition, it 
    can replace the undefined symbol with a reference to the definition
    * Some linkers work in a single pass, so the order in which files are 
    linked can be relevant.
    * Standard libraries, such as those providing I/O routines, are added to 
    the linking by default.
    * If a symbol remains undefined (also known as unresolved), then the 
    linking will fail.
    * If a symbol has two separate definitions, then the linking will fail. 
    This is probably a violation of the one definition rule 

## `nm` reference
* usage: `nm main.o --demangle`
* value of symbol: `U`, `T`, `D`, `B`, etc
    | Character | Meaning |
    | --------- | ------- |
    | U         | Undefined - this .o file needs it |
    | T         | Defined (in text) - this .o file provides a definition |
    | D         | Defined (in data) - this .o file provides a definition, non-zero initialized |
    | B         | Defined (in data) - this .o file provides a definition, zero initialized |
    * If the symbol is local (non-external), the symbol's type is instead 
    represented by the corresponding lowercase letter.
* e.g.
    * src
        ```cpp
        extern int humplecrimp;
        void frobscottle() {
            humplecrimp = 20;
        }
        int zoonk = 10;
        ```
    * `nm a.o`
        ```
        0000000000000000 T __Z11frobscottlev
                         U _humplecrimp
        0000000000000014 D _zoonk
        0000000000000000 t ltmp0
        0000000000000014 d ltmp1
        0000000000000018 s ltmp2
        ```
    * mental model for local 
        * locals are what're used within the obj file; 
        * they have external names, mapped to the same address
        * **they are not local variable**
        * see below
    * `objdump -d a.o` (ARM architecture has a different instr set)
        ```
        Disassembly of section __TEXT,__text:
        0000000000000000 <ltmp0>:
            0: 09 00 00 90   adrp    x9, 0x0 <ltmp0>
            4: 29 01 40 f9   ldr     x9, [x9]
            8: 88 02 80 52   mov     w8, #20
            c: 28 01 00 b9   str     w8, [x9]
            10: c0 03 5f d6   ret
        ```
    * `objdump -t a.o`
        ```
        SYMBOL TABLE:
        0000000000000000 l     F __TEXT,__text ltmp0
        0000000000000014 l     O __DATA,__data ltmp1
        0000000000000018 l     O __LD,__compact_unwind ltmp2
        0000000000000000 g     F __TEXT,__text __Z11frobscottlev
        0000000000000014 g     O __DATA,__data _zoonk
        0000000000000000         *UND* _humplecrimp
        ```

Section 4 + Lab 4: Compile (decl & def); Preprocessing (.h)
===========================================================
## Compilation
* Isolated Compilation
    * C++ is statically typed. A running C++ program knows very little about the 
    types that existed at compile-time
        * => Need to know the type
    * Each .cpp file is compiled individually 
        * => the compiler is unable to see across into the other file in order 
        to observe that ‘x’ is an int.

* Declarations & Definitions
    * Declaration
        * "Names are introduced by declarations."
        * "A declaration **provides the shape** of that thing."
        * "If a .cpp file uses a name, then there must have been a declaration 
        for that name."
    * Definition

## Preprocessing
* Put declarations in a header file
    * think of it as telling the outside world what names (variables,
    functions) are contained in the .cpp

*  `g++ --std=gnu++14 -E hello.cpp`: preprocessing only 

Section 5 + Lab 5: make; cmake
==============================
## make
* dependency 
    ```
                 preprocess    compile       link
    [.h] + [.cc] ------->  tu  -------> [.o] -------> [binary]
       *   1     1      1      1      1      *      1
                 --------------------->      ------->
                 *                    1      *      1
               sales.o: sales.cpp sales.h  revenue: licensing.o sales.o sumof.o main.o
    ```
* dependencies can be expressed in a makefile. 
* default file name: `Makefile`; command: `make`
* `make` looks at the time an item was created or updated and compares it to the
update times of its dependencies. 
* e.g.
    ```Makefile
    revenue: licensing.o sales.o sumof.o main.o
        ${CC} -o revenue main.o licensing.o sales.o sumof.o ${LINK_OPTS}

    licensing.o: licensing.cpp licensing.h
        ${CC} ${COMPILE_OPTS} -c licensing.cpp

    sales.o: sales.cpp sales.h
        ${CC} ${COMPILE_OPTS} -c sales.cpp

    sumof.o: sumof.cpp sumof.h
        ${CC} ${COMPILE_OPTS} -c sumof.cpp

    main.o: main.cpp licensing.h sales.h
        ${CC} ${COMPILE_OPTS} -c main.cpp
    ```

## cmake
* cmake is a tool that generates makefiles; cmake file lists the .cpp files.
* default file name: `CMakeLists.txt`; command: `cmake </root/dir/of/proj>`, then `make`
* transitive dependencies are worked out automatically

Section 6 + Lab 6: Library
==========================
* A library is a container for object files
* Inside a library it is still just the code and symbols
* A library is manually created with the `ar` (archive) command

Side Notes
==========
* why is global variables an anti-pattern? 
    - Global variables are problematic because they introduce largely 
    unnecessary dependencies across modules. These dependencies make it harder 
    to debug problems and reuse code.

* rerun cmake
    - The cmake binary of course needs to re-run each time you make changes to 
    any build setting, but you wont need to do it by design; hence "never" is 
    correct regarding commands you have to issue. When invoking make (assuming 
    unix here) this automatically triggers a previous execution of cmake if 
    necessary; so your generated projects include logic to invoke cmake itself!

* one definition rule
    - applies to variables, functions, and classes
    - WITHIN a translation unit

* forward declaration
    - not every type we depend on in a class definition has to be defined itself
    - Declare a member to be a pointer or a reference to the incomplete type:
		```cpp
		class Foo {
			X *p;
			X &r;
		};
		```
	- Declare functions or methods which accept/return incomplete types:
		```cpp
		void f1(X);
		X    f2();
		```
    - Define functions or methods which accept/return pointers/references to 
    the incomplete type (but without using its members):
		```cpp
		void f3(X*, X&) {}
		X&   f4()       {}
		X*   f5()       {}
		```

* libs are not linked

* is it possible to declare but not define local variables?
    - no
    - for local variables, there is no concept of definition. They are just 
    declared and are conditionally instantiated according to the flow of program
    - separate declaration and definition are used for global vars and funcs

* it the following okay? 
    ```cpp
    class A {B* b;}; 
    class B {};
    ``` 
    - no. causes compilation error `unknown type name 'B'`

* misc (take with a grain of salt)
    * when we use member of a class, do we need to include the class that 
    defined that memeber type
        - yes, for sure
    * class in class
        - think of it as if there's not nesting relationship

Demystification
===============
## build commands
* `g++ x1.cc x2.o x3.a -lx4 -o hello`
    * preprocess: `g++ -E hello.cc`
    * compile:    `g++ -c hello.cc`
    * library   
        * create: `ar rvs helpers.a helpers.o`
        * use:    `g++ hello.cc helpers.a -o hello`
    * link:       `g++ hello.o -o hello`, `ld`
    * `objdump`

```
    .h   -  .h   -  .h   -  .h   -  .h      g++ x1.cc x2.o x3.a -lx4 -o hello
    |/      \|/     \|/     \|/     \|      ---------------------------------
    .cc     .cc     .cc     .cc     .cc
    |       |       |       |       |       preprocess  -E
    tu      tu      tu      tu      tu  
    |       |       |       |       |       compile     -c
    .o      .o      .o      .o      .o     (archive     ar)
    \       \       |       /       /       link        .o; ld
                binary
```
* `make`
    * `.h, .cc -> .o; .o -> binary`
    * expresses dependencies, to avoid unnecessary work (not to store cmd)
    * compares ts to that of its dependencies (not the `make` ts, not content)

* `cmake`
    * `.cc -> binary`
    * lists.cc files, automatically works out transitive dependencies

## symbols and name
* comparision
    * intension
        * symbol: offset 
        * name: shape of it introduced by declaration
    * applies to 
        * symbol: only funcs & globals; no local var
        * name: local vars as well
    * similarities
        * for funcs & globals, symbol & name match

* "definition"
    * one definition rule 
        - refers to translation unit, **not object file** 
        - applies to variables, functions, **and classes**
    * linking will fail if
        - a symbol remains undefined
        - a symbol has two separate definitions
            - inline functions are okay
            - this is **probably** a violation of the one definition rule 

## forward declaration 
* when can we use it
    * we don't need the shape of the class; **and**
    * we don't need the member functions of the class
* why do we want to use it
    * so that we can remove dependencies between header files ("won't pollute 
    the header file")
    * not because we can avoid `#include`, often times we can only postpone it 
    to .cc
* specifically
    - Declare a member to be a pointer or a reference to the incomplete type:
		```cpp
		class Foo {
			X *p;
			X &r;
		};
		```
	- Declare functions or methods which accept/return incomplete types:
		```cpp
		void f1(X);
		X    f2();
		```
        * we will need complete type when we call the function
    - Define functions or methods which accept/return pointers/references to the
      incomplete type (but without using its members):
		```cpp
		void f3(X*, X&) {}
		X&   f4()       {}
		X*   f5()       {}
        ```

## circular dependency @todo
* class
    * circular dependency
        - create tight couplings between the classes or modules involved
* preprocess
    * circular dependency 
        - among header files, but not .cc files
        - => infinite loop in preprocessing
    * "include guard"/"header guard"
        - prevent infinite loop in preprocessing (e.g. cyclic includes)
        - prevent duplicate definition in compiliation (e.g. include same .h 
        more than once + class definition in .h)
* linking
    * circular dependency
        * The linker searches symbols in the order that they appear, so if you 
        have a source file that contains a call to a library function, you need 
        to put it before the library, or the linker won't know that it has to 
        resolve it. 
    * providing one file twice in linking
        ```
        gcc main.c -lslA -lslB -lslC -lslA -lslB -o final
        ```
        * The only reason to include the library multiple times is, for example, 
        if slA requires a symbol resolved by slB but slB requires a symbol 
        required by slA. The linker does a single pass to resolve symbols, but 
        repeating your library causes, in effect, a second pass on that library. 
        It won't change the size of your output, but it's not necessary either
        ```
        gcc main.c -Wl,--start-group -lslA -lslB -lslC -Wl,--end-group -o final
        ```
* make file
    * no circular dependency
    * dag w/ scc

TIL
===
* side notes
    * everything in that section
* `git clean`
    * `-f`
    * `-n` dry run
    * `-d` directory
    * `-x` ignored file as well
* command
    * make express dependency
    * cmake lists .cc files 
    * libs are not linked
* symbol & name
    * symbol is offset
    * declaration is to provide shape
    * definition of a symbol has different meaning as definition of a name
    * one definition rule applies to translation unit, **not object file**
    * one definition rule applies to classes as well

Take-away
=========
## preprocess + compile; link
* preprocess + compile
    * name; provides shape
    * one definition rule in tu; no definition okay
    * .h + .cc -> tu -> .o
* link
    * symbol: provides offset
    * undef (x); 1+ def (x)
    * .o -> bin
* forward declaration
    * remove dependencies between header files
    * var declaration in class; function declaration; function definition
    * think about whether the user need the shape & functions

## tools
* `g++ .cc .o .a -lbtrade`; `-E`, `-c`, `ar`, `ld` 
* `make`: .h + .cc -> .o (pre + compile); .o -> bin (.o -> bin); `Makefile`
* `cmake`: .cc -> bin; `CMakeLists.txt`
* dependency
    * class
    * preprocessing; linking
    * make

Another Round of Take-away
==========================
```
    .h   -  .h   -  .h   -  .h   -  .h      preprocess: one def rule in TU
    |/      \|/     \|/     \|/     \|         compile: name - shape     
.cc|TU|.o   .cc     .cc     .cc     .cc     
    \       \       |       /       /             link: one def across .o's       
                  binary                                symbol - offset
```
* in 1 TU
    * 1 definition: var, func, class
    * recap 
        ```
        def    | var: 1    | func: 1   | class: 1  
        decl   | var: n    | func: n   | class: n  
        ```
        * you can't have 2 definitions in 1 TU, let alone conflicting defs
        * you can have 2 declarations, as long as they don't conflict 
            * e.g. `int a; double a;`
            * e.g. `int a; void a()` - var and func cannot have the same name
    * one possible scenario
        ```
        .h  .h      no: both contains definition for same var|func|class
        |   /           even if they're identical; even if they're inlined
        .cc
        ```
* across TU's
    * 1 definition: var, func; class is compiled away
    * recap
        ```
        def    | var: 1    | func: 1   | class: n  
        decl   | var: n    | func: n   | class: n  
        ```
        * for var|func, you can't have 2 definitions, let alone conflicting ones
            * var and func can't share name
        * class def, like decls, it evaporates in compilation, so you can have 2 in different TUs
        * conflicting ones => undefined behavior (esp. applies to class def)
    * one possible scenario
        ```
        .h          no: contains definition for var|func
        |   \       ok: contains definition for class
        .cc .cc
        ```