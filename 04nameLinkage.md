Linking
================================================================================
## linking
    ```
    .cc name                                                    a::A::x
                                                               
     |           file 1       file 2       file 3                 |   
     V          +----------+ +----------+ +----------+            V   
                | external | | external | | external |  (def)         
    .o symbol   | local    | | local    | | local    |          _a_A_x 
                +----------+ +----------+ +----------+                
     |            external     external    external     (use)     |   
     V            local        local       local                  V   
                                                                 
    bin offset                                                  0x0008
    ```

Compiling
================================================================================
## which name has symbol
* namespace                                     - no 
* class                                         - no 
* function                                      
    * in a namespace                            - yes
    * in a class    
        * class static                          - yes
        * instance                              - yes
* variable
    * in a namespace                            - yes
    * in a class
        * class static                          - yes
        * instance                              - no
    * in a function
        * local, parameter                      - no
        * local static                          - yes

## name introduction
* introduce 2 identical names of the same type but different linkage
    ```cpp
    // error: static declaration of 'count' follows non-static declaration
    extern int count;
    static int count;

    // ok
    static int count;
    extern int count;

    // error: static declaration of 'f' follows non-static declaration
    void f();
    static void f();

    // ok
    static void f();
    void f();
    ```
* introduce 2 identical names
                | variable  | function  | class     | namespace 
    ------------|-----------|-----------|-----------|-----------
    variable    | ok        | -         | -         | -
    function    | sym redef | ok        | -         | -
    class       | ok        | ok        | ok        | -
    namespace   | sym redef | sym redef | sym redef | ok: re-open
    ```cpp
    // ok
    extern int count;
    extern int count;

    // ok
    void f();
    void f();

    // ok
    class Foo;
    class Foo;

    // error: redefinition of 'foo' as different kind of symbol
    extern int foo; 
    void foo();

    // ok
    extern int foo; 
    class foo; // hidden

    // ok
    void foo();
    class foo; // hidden
    ```
* introduce a name in global scope and anonymous namespace: different names
    ```cpp
    // they are different names
    namespace {
        void f() {}         // _ZN12_GLOBAL__N_11fEv
        namespace {
            void f() {}     // _ZN12_GLOBAL__N_112_GLOBAL__N_11fEv
        }
    }
    void f() {}             // _Z1fv
    ```

## name lookup
* name in global scope and anonymous namespace
    ```cpp
    // ok when introduced; ambiguous if used
    int count = 20;
    namespace { int count = 30; }
    void caller() { count++; }      // error: reference to 'count' is ambiguous

    // ok when introduced; ambiguous if used
    static int count = 20;
    namespace { int count = 30; }
    void caller() { count++; }      // error: reference to 'count' is ambiguous

    // ok
    namespace { int count = 30; }   // _ZN12_GLOBAL__N_15countE:
    int caller() { return count; }  // movl    _ZN12_GLOBAL__N_15countE(%rip), %eax
    int count = 20;                 // count:
    ```

## name mangling
* scope
    ```cpp
    int count = 10;                         // D count            : _count
    namespace a { int count = 10; }         // D _ZN1a5countE     : _a_count
    int A::count = 10;                      // D _ZN1A5countE     : _A_count
    void demo() { static int count = 10; }  // d _ZZ4demovE5count : _demo_count
    
    void demo() {}                          // T _Z4demov         : _demo
    namespace a { void demo() {} }          // T _ZN1a4demoEv     : _a_demo
    class A { public: void demo() {} };     // T _ZN1A4demoEv     : _A_demo
    ```
* anonymous namespace
    ```cpp
    int count = 10;                         // D count                                  : _count
    namespace { int count = 10; }           // d _ZN12_GLOBAL__N_15countE               : _anonymous_count
    namespace { 
        namespace { int count = 10; }       // d _ZN12_GLOBAL__N_112_GLOBAL__N_15countE : _anonymous_anonymous_count
    }
    ```
* function params
    ```cpp
    void demo() {}                          // T _Z4demov         : _demo_void
    void demo(int) {}                       // T _Z4demoi         : _demo_int
    void demo(bool, A) {}                   // T _Z4demob1A       : _demo_bool_A
    ```
* static (declarations don't co-exist)
    ```cpp
    int count = 10;                         // D count            : _count
    static int count = 10;                  // d _ZL5count        : _count_L

    void demo() {}                          // T _Z4demov         : _demo
    static void demo() {}                   // t _ZL4demov        : _demo_L
    ```

## put it all together
* introduce name & provide definition
    * see above
* introduce name & use definition
    ```cpp
    extern int count;
    void caller() { count++; }  // movl    count(%rip), %eax                    : _count

    static int count;
    void caller() { count++; }  // movl    _ZL5count(%rip), %eax                : _count_L

    namespace { int count; }
    void caller() { count++; }  // movl    _ZN12_GLOBAL__N_15countE(%rip), %eax : _count_N

    void demo();
    void caller() { demo(); }   // call    _Z4demov                             : _demo

    static void demo();
    void caller() { demo(); }   // call    _ZL4demov                            : _demo_L

    namespace { void demo(); }
    void caller() { demo(); }   // call    _ZN12_GLOBAL__N_14demoEv             : _demo_N

## case study
* define: external; use: internal
    ```cpp
    // main.cc
    static void priceDemo(); 
    int main() { priceDemo(); }

    // price.cc
    void priceDemo() { }

    // main.cc:(.text+0x5): undefined reference to `priceDemo()'
    // collect2: error: ld returned 1 exit status
    ```
* define: internal; use: external
    ```cpp
    // main.cc
    void priceDemo(); 
    int main() { priceDemo(); }

    // price.cc
    static void priceDemo() { }

    // main.cc:(.text+0x5): undefined reference to `priceDemo()'
    // collect2: error: ld returned 1 exit status
    ```

Misc
================================================================================
## anonymous namespace
* cppreference
    * This definition is treated as a definition of a namespace with unique name
    and a using-directive in the current scope that nominates this unnamed 
    namespace
    * The unique name is unique over the entire program, but within a 
    translation unit each unnamed namespace definition maps to the same unique 
    name: multiple unnamed namespace definitions in the same scope denote the 
    same unnamed namespace.
    * (until c++11) Even though names in an unnamed namespace may be declared 
    with external linkage, they are never accessible from other translation 
    units because their namespace name is unique.
    * (since c++11) Unnamed namespaces as well as all namespaces declared 
    directly or indirectly within an unnamed namespace have internal linkage, 
    which means that any name that is declared within an unnamed namespace has 
    internal linkage. 

* can you have a class outside a namespace, but put its function inside a 
  anonymous namespace
    - no
    ```cpp
    class Price { public: void priceDemo(); };
    // error: cannot define or redeclare 'priceDemo' here because namespace '' does not enclose namespace 'Price'
    namespace { void Price::priceDemo() { } }
    ```

## what are we talking about when we talk about name linkage (note to self) 
* whether we can access the definition of a symbol from another file
* static global variable / free function: not accessible
* global variable / free function in an anonymous namespace: not accessible

## moving to c++ (note to self)
* class static functions, class instance functions, class static variables
* putting a class in a namespace (doesn't seem to have much use case to me as 
of now)
