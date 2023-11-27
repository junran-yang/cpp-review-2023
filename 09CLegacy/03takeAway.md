Take Away
================================================================================
## Evaluation Order
* precedence & associativity - syntactic analysis
    * precedence
        1. scope, `::`
        2. unary, e.g.`&a.b->c()`
        3. binary
            * `*/; +-` > `>; ==` > `&&; ||` > `=`
            * `>>` > `&; ^; |`
    * associativity
        * right associative: prefix unary, assignment

* evaluation order - semantic analysis
    * order of evaluation of any part of any expression is unspecified
    * includes
        * value computation
        * side effects
    * guarantees (note to self)
        * full-expression is fully evaluated before the next one
        * value computation 
            * child nodes is done before parent
        * side effects 
            * `a && b`, etc
            * function arguments
        * with respect to interleaving
            * function calls are not interleaved

## Array 
* array
    * a continuous chuck in memory
    * initialization
        * automatic: `int a[5]; int a[5](); int a[5] = {1, 2, 3};`
        * dynamic: `new int[5]; new int[5](); new int{1, 2, 3};`
        * member: `-; a(); a{1, 2, 3}; `
    * decay to pointer in evaluated context
        * `int* p = a;`
        * `demo(a);`
            * `void demo(int[])`, `void demo(int*)` mean the same thing
        * `a[3];`
            * indexing is defined in terms of pointers 
* multidimensional array
    * `int a[3][5][7];`
    * initialization
    * decay & type
        ```cpp
        int (*p1)[5][7] = a;
        int (*p2)[7] = a[0];
        void demo(int (*p)[5][7]); void demo(int (*)[5][7]);
        void demo(int a[][5][7]); void demo(int [][5][7]);
        a[1][2];
        ```
        * (note to self) since indexing is defined in terms of pointers, what 
        really matters is the type of the pointer that name of an array decays to

## string (note to self) 
* c style string: char array
    * for char array, "initializer must be an initializer list or string literal"
* string literal 
    * type: const char array
    * tailing `\0`
    * has storage
        * data/text
        * 1/multiple
    * can not be modified
* char pointer
    * name decay applys as normal
    * `std::strlen`, `<<`, etc has special handling with regard to `\0`

## pointers to functions
* define & use
    ```cpp
    void (*demo_p)(int) = demo;     // &demo
    demo_p(5);                      // (*demo_p)(5)
    ```
* pass & return
    ```cpp
    int c(int (*a)(int), int (*b)(int), int n) {
        return a(b(n));
    }

    void f(int) {}
    void (*demo())(int) {           // inner: signature; outer: return type
        return f;
    }
    ```

## macros
* macros perform textual substitution
* they are part of the preprocessor, not the compiler

## switch statement
* c++ has `goto`

## which ones to use
* evaluation order - no choice
* array - consider vector
    * array might still come in handy
    * multidimensional array
* c style string - consider string
    * char* might still come in handy
    * along with lib functions that manages char*
* pointers to function - consider function object
* macro - no
* switch - ok

Shortened Take-away
================================================================================
## Evaluation Order
* precedence & association
    * `::`
    * unary, e.g. `&a.b->c`
    * binary
        * arithmetic > comparison (`>; ==`) > logical (`&&; ||`)> assignment
        * bitwise: shift > logical (`&; ^; | `)
* evaluation order
    * unspecified
    * within an expression
    * value computation: guarantee - tree
    * side effects: guarantee - sequence pointes, e.g. `a && b`; `f(a, b, c)`

## Array & String
* array
    * aggregate initialization
    * name decay to pointer
        * `int* p = a;`
        * `demo(a);`, `int*, int[]`
        * `a[3];` - indexing is for pointer
    * multidimensional array
        * the type is only important for pointer it decays to 
        * `int (*p)[5][7] = a;`
* string
    * char array, nothing special
    * char pointer - special handling for `\0`
    * string literal - e.g. "hello", `const char[N]`, trailing `\0`, storage

## function pointer
* syntax
    * `void (*demo)(int) = f;`
    * syntax for passing & returning

## macro
* preprocessing

## switch
* `goto`
