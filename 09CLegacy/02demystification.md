Demystification
================================================================================
## notable about precedence and associativity
* precedence
    * scope 
        * scope resolution above everything else
    * unary
        * (note to self) unary: those you don't put space around
        * unary postfix > unary prefix
            * `++x.a`
    * binary
        * arithmetic > relation > logical > assignment
            1. `* /; + -`
            2. `>  ; ==` - comparison > equality
            3. `&& ; ||` - and > or
            4. `=`           
            * e.g. `bool valid = x + y > 10 || z > 20; `
        * bit-wise: shift > logical
            1. `<<; >>`
            2. `& ; ^; |`
    * ops that I don't know of
        * `+x; -x; .*; ->*; ,`

* associativity
    * only 2 right associative
        * unary prefix
        * assignment

## (note to self) 5 phases of compilation
* lexical analysis      => tokens
* syntactic analysis    => abstract syntax tree
* semantic analysis     => intermediate representation (asm-like)
    - name table lookup
    - type checking
* optimization          => intermediate representation (asm-like)
* code generation       => target language (asm)                    
    - register assignment

## AST Construction
* construction of abstract syntax tree is specified
    * controlled by precedence and associativity
    * "formal grammar"
```cpp
7 * f() + g() + 4;
/*
            + 
           / \
          +   4
         / \
        *  g()
       / \
      7  f()
*/
```

## Evaluation Order
* the evaluation order of most operators in C++ is unspecified (until c++17)
```cpp
7 * f() + g() + 4;      e(f(), g(), h());
/*
            +                  e()
           / \               /  |  \
          +   4           f()  g()  h()
         / \
        *  g()
       / \
      7  f()
*/
```

## Side Effect Order
* The side effect may happen immediately, or it may be delayed (until c++17)
```cpp
j += ++i;        i += ++i;           i = v[i++];
/*
  +=(*)           +=(*)                =(*)
 / \              / \                 / \ 
j  ++(pre)(*)    i  ++(pre)(*)       i  []
    |                |                  / \
    i                i                 v  ++(post)(*)
                                            |
                                            i
(*) denotes side effect at the operator
*/
```

## Sequence Point (all from cppreference)
* A sequence point is a point in the execution sequence where all side effects 
from the previous evaluations in the sequence are complete, and no side effects 
of the subsequent evaluations started.
* full expression `a = 1 + 3;`
    * at the end of each full-expression (typically, at the semicolon)
    ```
      ;                 <- fully evaluated at this point   
      |
      =
     / \
    a   +
       / \
      1   3
    ```
* the following built-in op `||; &&; ,; ?:` 
    * (note to self) where short circuiting is possible
    * there is a sequence point after the evaluation of the expression a.
    ```
    a && b
    a || b
    a ? b : c
    a , b
    ```
* function call `f(a, b, c)`
    * after the evaluation of all function arguments (if any) which takes place 
    before execution of any expressions or statements in the function body
* return statement `return 1 + 3;`
    * at the end of expression in the return statement (if any), after the 
    copy-initialization of the result of the function call, and before the 
    destruction of all temporary objects
* returned value `int a = f();`
    * after the copying of a returned value of a function and before the 
    execution of any expressions outside the function.
* functions cannot be interleaved
    * Once the execution of a function begins, no expressions from the calling 
    function are evaluated until execution of the called function has completed 

## Order of evaluation (cppreference)
* Order of evaluation of any part of any expression, including order of 
evaluation of function arguments is unspecified. The compiler can evaluate 
operands and other subexpressions in any order, and may choose another order 
when the same expression is evaluated again.
* evaluation of each expressions includes
    * value computations
    * initiation of side effects
* ordering
    * sequenced before/after
    * unsequenced: may be performed in any order and may overlap 
    * indeterminately sequenced: may be performed in any order but may not overlap
* rules (subset)
    * Each value computation and side effect of a full-expression is sequenced 
    before each value computation and side effect of the next full-expression
    * The value computations (but not the side effects) of the operands to any 
    operator are sequenced before the value computation of the result of the 
    operator (but not its side effects).
    * When calling a function, every value computation and side effect 
    associated with any argument expression, is sequenced before execution of 
    every expression or statement in the body of the called function.

## multidimensional arrays (cppreference)
* The important thing to note are the numbers that define how large to make each
  dimension. Because of this it is not possible to pass a multidimensional array 
  without also providing the length of its dimensions.
    ```cpp
    int a[5][7][10];
    void foo(int array[][7][10], int size);
    foo(a, 5);
    ```
* type of multidimensional array (note to self)
    ```cpp
    int a[3][5][7];
    int (*p1)[5][7] = a;
    int (*p2)[7] = a[0];
    ```

## questions
* [x] where does the concept of evaluation order still apply? 
    - within an expression, according to cpprefrence
* [x] dynamic initialization vs unspecified evaluation order (note to self) 
    - diff
        - dynamic initialization mostly happens when loading the program, and 
        there's no ordered code that does the initialization for the globals
        - evaluation order is within an expression, and there's no specification 
        for the ordering
    - similarity
        - underterministic ordering of execution 
        - becomes a problem when there can be side effect
