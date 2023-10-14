
Section 01 Static
=================
## automatic, static, dynamic storage duration
* automatic
    * local variables, temporaries, function parameters, function return values
* static
    * global variables, class static variables, local static variables
    * local static variable
        * has static storage duration – it lives in the data segment
        * scoped within the block - only the function knows the name of the var
        * initialized the first time the initialization is encountered - not the
        start of the program
        * **shows that an obj’s lifetime is separate to the var name’s scope**
        * often superior to a class static because its scope is more restrictive
* summary
    |location/duration |kind           |lifetime
    |------------------|---------------|---------------------------------------|
    |stack/auto        |auto variable  |initialization - end of block
    |                  |parameter      |function call  - function end
    |                  |temporary      |initialization - end of full expression
    |heap/dynamic      |               |whenever       - whenever
    |data/static       |local static   |initialization - program end
    |                  |class static   |program start  - program end
    |                  |global         |program start  - program end
    * instance variables has the same lifetime as containing object
    * the storage duration dictates the location

## static initialization order
* dynamic initialization
    * requires code to be executed
    * not zero initialization
    * not initialization from compile time constants
* local statics
    * initialized when first encountered
* global variables, class static variables
    * within a single tu
        * guaranteed to be dynamically initialized in definition order
    * across tus
        * they can be interleaved
    * there’s not even a guarantee that dynamic initialization will have
    happened before main is called.
    * the only guarantee is that before actively using an object or function
    from a TU, initialization will be started.

## raw questions
* dynamic initialization
    * not compile time, not runtime, but when loading the program, right?
        - can be after main
    * why is it not considered as part of execution?
        - not part of main
    * why don't we talk about dynamic initialization for other storage duration
        - because they all happen after main for sure
    * why does static initialization order matter
        - dependency
    * how do they call functions?
        - just call it
    * can they call functions that involve static local variables?
        - yes
    * can you device a scenario where 2 global variable depends on each other
      in their initialization?
        - yes, and no loop will be created, because one would be assumed to be 0
    * is built-in type zero initialized in data section
        - yes
    * what is the syntax for default/direct/copy/value init global/class static?
        - same as before

Demystification
===============
## storage duration vs. name scope
* how object lifetime relates to name scope
    * not related
    |           |global  |class s. |local s. |local |param |return  |dynamic
    |-----------|--------|---------|---------|------|------|--------|--------|
    |life time  |program |program  |program  |block |block |expr    |as spec
    |name scope |global  |class    |block    |block |block |no name |no name
    * local s. shows that an obj’s lifetime is separate to the var name’s scope

## initialization in data section
* Why are global variables always initialized to '0', but not local variables?
    * zero initialization (cppreference, partial)
        * for every named variable with static storage duration that is not 
        subject to constant initialization, before any other initialization
    * constant-initialization (cppreference, partial)
        * if a static variable is constant-initialized, constant initialization 
        is performed instead of zero initialization before all other 
        initializations

## dynamic initialization
* Does global initialization order ignores dependencies?
    * yes
    ```cpp
    // file a.cc
    extern int b;
    int a = initA();
    int initA() { return b + 1; }

    // file b.cc
    extern int a;
    int b = initB();
    int initB() { return a + 1; }

    // possible results
    // a = 1; b = 2;
    // a = 2; b = 1;
    ```

* note to self
    * load code to text
    * load zero/constant to data
    * perform dynamic initialization on data, no dependency considered
    * main could start running before dynamic initialization finishes

## initialization summary
 type       |auto/static* |dynamic     |array(a./s.) |array(d.)     |member |basic      |udts             
|-----------|-------------|------------|-------------|--------------|-------|-----------|-------------------|
 default    |int a;       |new int;    |int a[5];    |new int[5];   |empty  |no-op      |default            
 direct     |int a(1);    |new int(1); |-            |-             |a(1),  |as defined |direct, copy     
 copy       |int a = _a;* |-           |-            |-             |-      |as defined |copy (non-explicit)
 value      |int();       |new int();  |int a[5]();  |new int[5](); |a(),   |zero-init  |default, "value"

* syntax
    * additional syntax for automatic
        * param: `demo(a)`
        * return: `return a;`
        * (both copy initialization, required to be non-explicit)
    * additional syntax for static
        * class static, local static: `static int a;`
        * (`static` optional for global)
    * dynamic & member
        * value initialization is in-place (no additional copy init involved)
        * no syntax for copy initialization
* behavior
    * static is the only thing that differs
        * zero-initialzation + other initialization
        * unless when there's a handy way of doing constant-initialization
    * member vs containing object
        * the same storage duration 
        * independent initialization method
        * think basic types as leaf nodes, udts as non-leaf nodes
    * array
        * just initialize the components separately all at once

* note to self
    * stick to initialization type - syntax - behavior
    * the only exception is static: zero/constant initialization performed 
    before the regular initialization
    * initialization is a tree: basic type as the leaf, udts as non-leaf