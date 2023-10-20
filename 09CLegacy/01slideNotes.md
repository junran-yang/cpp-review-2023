Section 01.01: Evaluation Order
================================================================================
## AST Construction
```cpp
7 * f() + g() + 4
```
* expressions -> abstract syntax trees
    * this will match the formal grammar for c++
* controlled by precedence and associativity
    * precedence ranks operators
    * associativity tie-breaks operators of same precedence
* construction of syntax tree is 100% specified and predictable

## Evaluation Order
```
            + 
           / \
          +   4
         / \
        *  g()
       / \
      7  f()
```
* evaluation order is constrained by the AST but it is not specified by the AST
    * each node ‘yields’ a value to be used by the node above, or to be 
    discarded if it comes off of the top of the tree
    * for a node to perform its operation, it just needs the values from the 
    lower nodes to be available. 

* in c++, the compiler is free, for most operators, to choose the order of 
  evaluation
    * this enables the compiler to choose a more efficient evaluation strategy
    * this can also lead to surprises and differences that are most often 
    observed when using a different compiler or architecture

* the evaluation order of most operators in C++ is unspecified

## Precedence & Associativity
* precedence
    * scope `::`
    * unary
        1. postfix `x++; x--; T(); f(); x[]; x.; x->`
        2. prefix `++x; --x; +x; -x; !x; ~x; (T)x; *x; &x; sizeof x; new x; delete x;`
        3. pointer to member (?) `.*; ->*`
    * binary
        1. arithmetic
            * `*; /; %`
            * `+; -`
        2. bit-wise shift `<<; >>`
        3. comparison
            * `<; <=; >; >=`
            * `==; !=`
        4. bit-wise
            * and `&`
            * xor `^`
            * or  `|`
        5. logical
            * and `&&`
            * or  `||`
        6. assignment
            * `?:; =; +=; -=; *=; /=; %=; <<=; >>=; &=; ^=; |=`
        7. sequencing `,`
* associativity
    * associativity is used when the compiler is unable to determine order using 
    precedence alone because the operators are in the same group. 

* common case
    * precedence
        * `++x.a` 
    * associativity (when ops in the same precedence group)

* reference
    * https://cplusplus.com/doc/tutorial/operators/

## Evaluation Order 
```cpp
e(f(), g(), h())
/*
       e()
     /  |  \
  f()  g()  h()
*/
```
* On Linux, and Windows, the order of evaluation of the arguments appears to be 
right-to-left
* On Solaris and AIX it appears to be left-to-right
* In the name of efficiency, the evaluations of the arguments may be interleaved 
  (until C++17)
    * cannot occur for e(f(), g(), h()), because of sequence points
    * evaluation for f(++x, --x - --y) is completely unpredictable
* If these functions have side effects, then there may be observable differences

## Side Effect Order (& Gray Expression)
* The side effect may happen immediately, or it may be delayed. The compiler is 
free to choose.
```cpp
i += ++i;           i = v[i++];
/*
    +=                  = 
   / \                 / \ 
  i  ++(pre)          i  []
       |                 / \
       i                v  ++(post)
                            |
                            i
*/
```
* prior to c++17, Side effects can be delayed. 

## Sequence Point
```cpp
if (f(x + 3) + 7 || y + 2) {}
      1.         3.      4. 

int f(int a) {}
             2. 
```
* rules
    * all pending evaluations and side effects must be complete and observable 
    at a sequence point
    * between sequence points the compiler is free to change order.
* sequence points (c++03)
    * function call - all arguments are fully evaluated before function call
    * function return - return val construction and all associated side effects
    * `||; &&; ,; ?:` - these are treated specially
        * `||; &&` - left evaluated before right
        * `,` - left to right, yields right most value
    * full expression, e.g. 
        * `if` condition
        * expressions that are a statement (ends with `;`)
    * member initialization

    
Section 01.02: Array
================================================================================
## array vs pointer
* when you use the name of the array in an evaluated context, it decays into a 
  pointer to the first element. 
    * this is an rvalue
    * you can't just assign to an array, because you would be assigning to an 
    rvalue
    ```cpp
    int a[10];
    int* p = a; // decay to pointer
    a = p;      // can't assign to an rvalue
    ```
* when you take the size of the array, you are using the array in an unevaluated
  context
    ```cpp
    int a[10];
    sizeof a;
    ```
* they are not the same thing
    * evidence of difference
        * `sizeof p` != `sizeof a`
        * `p = a; // ok`; `a = p; // error`
    * what might look misleading
        * indexing is defined in terms of pointers but is typically something 
        you want to do with arrays

## array initialization
```cpp
int a[5] = {1, 2, 3};
```
* single element
    | type      | auto/static*          | dynamic           | member    |
    |-----------|-----------------------|-------------------|-----------|
    | default   | int a;                | new int;          | empty     |
    | direct    | int a(1);             | new int(1);       | a(1),     |
    | copy      | int a = _a;*          | -                 | -         |
    | value     | int();                | new int();        | a(),      |
* array
    | type      | auto/static*          | dynamic           | member    |
    |-----------|-----------------------|-------------------|-----------|
    | default   | int a[5];             | new int[5];       | empty     |
    | direct    | -                     | -                 | -         |
    | copy      | -                     | -                 | -         |
    | value     | int a[5]();           | new int[5]();     | a(),      |
    | aggregate | int a[5] = {1, 2};    | new int[5]{1, 2}; | a{1, 2},  |

## multidimensional array
* a multidimensional array is not an array of arrays. 
* it is a single block of memory that allows offset calculations to be performed
using more than one index.
* the name of the multidimensional array decays into a pointer to the first 
element.
* however, this pointer now needs to include additional information so that 
offset calculations can be made correctly.
```cpp
double d[5][7][10];
double (*p1)[7][10] = d;
double* p2 = reinterpret_cast<double*>(d);
ad[2][3][4] == p2[2 * 7 * 10 + 3 * 10 + 4];
```

## pass and return by array
* when arrays are passed into functions, they are passed in as pointers to the 
  first element.
    * this follows from the fact that arguments are evaluated and array names, 
    when evaluated, decay into a pointer to the first element.
    * another reason: functions need to know the size of their arguments. if 
    arrays were passed by value, then only arrays of the correct size could be 
    passed to the function. 
* Parameter types T[] and T* mean the same thing

## questions
* what's the type of `a[1]` in `int a[2][5]; `? 
    - `int (*p)[5]`

Section 02: String
================================================================================
## recap: char array
```cpp
// automatic
char s[10];
char* p = s;
// dynamic
char* p = new char[10]();
```

## string literal 
* it's an array of characters, with a final char of `\0` (the null char)
* initialization from string literal
    ```cpp
    // char arrays can be initialized from a string literal 
    char s1[] = "hello";        // sizeof(s1) == 6
    char s2[] = "hello";        // s1 and s2 are different

    // char pointers can be initialized from a string literal
    const char* p1 = "hello"; 
    const char* p2 = "hello";   // p1 and p2 may be equal

    // std::string can be constructed from a string literal
    std::string a = "hello";
    ```
    * for legacy reasons it is possible to have a non-const char* point to the 
    first character of a string literal. However it is now deprecated and the 
    compiler may treat it as an error. It is undefined behavior to assign to an 
    element within a string literal.
* string literal storage
    * string literals need to live somewhere. they can be any size so there is 
    no ‘immediate’ representation in assembly for them
    * string literals have static storage – they exist for the lifetime of the 
    program
    * if a program uses a string literal more than once it is implementation 
    defined whether it uses exactly the same array
* string literal modification
    * string literals should not be modified. Modifying them is undefined 
    behavior
    * rather than place the string literal in the data segment, some compilers 
    place them in the process’s text segment – where the code lives. This 
    segment is normally protected by the OS from writing. Writing to a string 
    literal in this case would cause a segmentation or protection fault

## manipulating c style strings
* lib functions
    ```cpp
    char* std::strcpy(char* dest, const char* src); 
    char* std::strncpy(char* dest, const char* src, std::size_t count);
    char* std::strcat(char* dest, const char* src); 
    char* std::strncat(char* dest, const char* src, std::size_t count);
    std::size_t std::strlen(const char* str);
    int std::strcmp(const char* lhs, const char& rhs);
    ```
* impl of `strcpy`
    ```cpp
    char* std::strcpy(char* dest, const char* src) {
        char* const savedDest = dest;
        while (*dest++ = *src++) {}
        return savedDest;
    }
    ```
* tricks: non unique
    ```cpp
    const char path[] = "/home/fred/file.txt";  // "/home/fred/file.txt"
    const char* const file = path + 11;         // "file.txt"
    ```
* tricks: tokenization without requiring any additional space
    * replace the first space after a word with a `\0`
    ```
    the  quick  brown 0
    the0 quick0 brown00
    ```

## question
* what is the type of string literal? 
    - (cppreference) `const char[N]`
* how does std::string a = "hello" work? 
    * (note to self) "hello" is an array of chars, decay to const char*, then 
    implicit converted to std::string? 


Section 03.01: Pointers to functions
================================================================================
## syntax
* define & use function pointer
    ```cpp
    void (*demo_p)(int) = demo;     // &demo
    demo_p(5);                      // (*demo_p)(5)
    ```
    * you do not need to use the & when taking the address of a function, 
    although it is permitted
    * when applying a function through a pointer, you do not need to use 
    indirection, although it is permitted
* pass in & return function pointer
    ```cpp
    // pass
    int c(int (*a)(int), int (*b)(int), int n) {
        return a(b(n));
    }

    // return
    void f(int) {}
    void (*demo())(int) {
        return f;
    }
    ```
* functions & objects
    * functions aren't first-class in c++ so we can't really create them at run
    time
    * this is one of the reasons that function objects may be preferred to 
    regular functions

Section 03.02: MACROS
================================================================================
```cpp
#define LENGTH 12
#define MAX(A, B) (A > B) ? (A) : (B)
```
* macros perform textual substituition
* they are part of the preprocessor, not the compiler


Section 03.03: switch statement
================================================================================
* fall through
* c++ has `goto`. don't do it
    ```cpp
        goto world;
    hello: 
        std::cout << "hello" << std::endl;
    world: 
        std::cout << "world" << std::endl;
    ```

Section 03.04: What C Legacy to Use? 
================================================================================
* undefined evaluation order    - yes, you have no choice
* arrays                        - no, consider `std::vector`; except
    * automatic/statics         - yes, if size known at compile time
    * multidimensional arrays   - yes
* char arrays                   - no, consider `std::string`; except
    * const char*               - yes, to avoid `std::string` conversion
    * strcmp                    - yes, to work with c style strings
* pointers to functions         - no, consider function objects
* macros                        - no, unless you have to 
* switch                        - yes
