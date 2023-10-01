Section 1: Types & Conversions
==============================
## Types
* built-in
    * integral (char, bool, short, int, long | unsigned)
    * floating-point(float, double, long double)
    * pointer (void*, T*, nullptr_t)
* user-defined
    * enum
    * union
    * class (incl. struct)

## Implicit Conversion
* built-in
    * all integral & floating-point
    * pointer -> bool, nullptr_t/0/NULL -> T*, T* -> void*
* user-defined
    * enum -> integral
    * upcast: derived -> base
        * `A a = b; demo(b);`
    * 1-arg constructor
        * `A a = 1; demo(1);` 

* side note
    * The compile-time value 0, when used in a pointer context, is treated as 
    null pointer. No other integral value is treated in this way, 0 is special. 
    Note, if you’re working with C++11 you should use nullptr.

## Explicit Conversion
* static_cast
    * built-in
        ```cpp
        (int)3.1415926;
        static_cast<int>3.1415926;

        // temporary, cannot bind a reference to it
        int& a = (int)3.1415926;                // error
        int& a = static_cast<int>(3.1415926);   // error
        ```
    * user-defined
        * upcast: derived -> base
            ```cpp
            B b;
            std::cout << (A) b << std::endl;
            std::cout << static_cast<A>(b) << std::endl;
            ```
        * 1-arg constructor 
            ```cpp
            std::cout << (A) 10 << std::endl;
            std::cout << static_cast<A>(10) << std::endl;
            ```
    * key difference from implicit conversion: can use explicit ctor
    * c-style cast: don't use
    * function style cast
        * direct initialization
    * side notes: The compiler confirms that the cast is ‘possible’ at compile 
    time. Possible here means there is a way that the types could be converted, 
    but it doesn’t guarantee that it is correct at run-time
* const_cast
    * applies to 
        * reference `const A a; A& a1 = const_cast<A&>(a);`
        * pointer   `const A* p; A* p1 = const_cast<A*>(p);`
* dynamic_cast
    * checks that a downcast or sidecast is correct at run-time
* reinterpret_cast
    * dangerous

## Conversion & Reference
* bind const reference to rvalue
    ```cpp
    int& a = 1;         // error
    const int& a = 1;   // ok
    ```
* cast to reference
    ```cpp
    class B : public A {};
    B b;
    A& a = static_cast<A&>(b); // ok
    ```

## User-defined Conversion Operators
```cpp
class A {
  public:
    operator const int&() { return i; } 
    operator B() const { return B(i); }
    int i = 10;
};

int i = a;
B b = a;
```
* Conversion operators allow conversions ‘away’ from the type.
* The return type is part of the name of the function. 
* Typically it is called implicitly or through a cast.
* static_cast can find and use both converting constructors or conversion 
operators

Section 2: Function Overload Resolution
=======================================
```cpp
void fn(const Figi& x);
void fn(const std::string& x, int y = 0);
void fn(const std::string& x, double y);
void gn(const std::string& x);

std::string s("abc");

fn(s);
```
* steps
    1. filter on name
    2. filter on # of args
    3. filter on best match

* conversion
    1. exact match
        * adding reference, adding const
        * it is a misnomer: it really means that no temporary is required 
        * only exception: `int a;` - `int&` is better than `const int&`
    2. promotion
        * built-in only
        * e.g. short->int, float->double
    3. conversion (many complicated rules here)
        1. built-in other types of conversion
            * e.g. float->int
        2. user-defined
            * e.g. 1-arg ctor, upcast

* best match
    * have no conversions that are ranked lower than the other
    * have at least one conversion ranked higher than the other

Section 3: operator overloading
===============================
* syntax
    ```cpp
    void operator+() {}
    ```
* e.g. `a1 == a2` rewritten by compiler into 2 function calls
    * member function `operator==(a2)`
    * free function `operator==(a1, a2)`
    * neither has priority over the other. an ambiguity is likely

Demystification
===============
## how does this fits into the whole picture
* function 
    * exception
    * function overloading (incl. operator overloading); overload resolution
* object
    * basics
        * ref, const
    * storage
        * lifetime: initialization, destruction, assignment
        * duration: static, automatic, dynamic
    * type
        * udts: enum, union, class (incl. struct); inheritance
        * conversion & cast
    * name

## conversion
* performed whenever an expression of some type T1 is used in context that does 
  not accept that type, but accepts some other type T2, e.g. 
    ```cpp
    int a = 3.14;
    demo(3.14);
    return 3.14;
    switch (3.14) {}
    if (3.14)
    ```
* conversion
    * built-in types
        * numeric conversion
        * Demo* -> void*; nullptr_t|0|NULL -> Demo*; Demo* -> bool
    * user-defined types
        * upcast: derived -> base
        * single-argument constructor
        * user defined conversion operator
* "chaining" 
    * per cppreferecne: A standard conversion sequence consists of the 
      following, in this order:
        1. zero or one conversion from the following set: lvalue-to-rvalue 
        conversion, array-to-pointer conversion, and function-to-pointer 
        conversion
        2. zero or one numeric promotion or numeric conversion
        3. zero or one qualification conversion
    * note to self: 
        * user-defined conversions could not be chained, i.e. (x) A -> B -> C
        * numeric conversions do not need to be chained, b/c it can happen 
          between each pair 
        * pointer conversions and numeric conversions cannot be chained
* c++ abstract machine
    ```cpp
    class B {
      public:
        operator int() { return 20; }
    }
    void demo(int) {}

    int a = 3.14;   // movl    $3, -4(%rbp)

    demo(3.14);     // $3, %edi
                    // demo(int)
    B b;
    demo(b);        // leaq    -1(%rbp), %rax
                    // movq    %rax, %rdi
                    // call    B::operator int()
                    // movl    %eax, %edi
                    // call    demo(int)
    ```
    * note to self
        * looks like it really creates a temporary and store it somewhere

## cast
* static_cast
    * note to self: basically implicit conversion
    * when do you need static_cast
        * explicit 1-arg constructor; explicit conversion operator
        * function can take both types
    * note to self: static_cast yields an rvalue, which can later be implicitly
      converted. e.g.
        ```cpp
        void demo(int) {}
        demo(static_cast<float>('h'));  // ok
        ```
* const_cast
    ```cpp
    const int* p = new int;
    int* p2 = const_cast<int*>(p);
    ```
* dynamic_cast
* reinterpret_cast

## reference deserves its own section
* "conversion" to reference vs. cast to reference
    ```cpp
    // derived
    class A {};
    class B : public A {};
    B b;

    A& a = b;            // ok
    static_cast<A&>(b);  // ok, yields lvalue

    A&& a = b;           // error, an rvalue reference cannot be bind to an lvalue
    static_cast<A&&>(b); // ok, yields rvalue

    // binding won't work for 1-arg ctor or conversion operator
    class B { public: operator A() {  return A{i}; } };

    A& a = b;            // error: non-const lvalue reference to type 'A' cannot 
                         // bind to a value of unrelated type 'B'
    static_cast<A&>(b);  // error, same as above

    A&& a = b;           // ok
    static_cast<A&&>(b); // ok
    ```
    * note to self
        * `A& a = b; A&& a = b;` is really reference initialization, or 
        "binding" not the `A a = b` kind of convert-and-then-initialize process
        * `static_cast<A&>;` is binding
        * `static_cast<A&&>;` is something different
* bind const& to a temporary is allowed

## overload resolution
* intuition, 1-arg
    ```cpp
    demo(25);
    void demo(int&);
    void demo(long);
    void demo(float);
    void demo(Number);
    ```
* intuition, n-args
    ```cpp
    int a = 25; 
    demo(25, 3.14);
    demo(a, 3.14);
    void demo(int&, long);
    void demo(Number, int);
    void demo(long, int);
    ```

## operator overloading
* https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading

* The Three Basic Rules of Operator Overloading in C++
    * Whenever the meaning of an operator is not obviously clear and undisputed,
      it should not be overloaded. 
        * Basically, the first and foremost rule for overloading operators, at 
        its very heart, says: Don’t do it. 
    * Always stick to the operator’s well-known semantics.
    * Always provide all out of a set of related operations.
        * `+, +=`; `i++, ++i`; `>, <`, etc

* The Decision between Member and Non-member
    * The binary operators = (assignment), [] (array subscription), -> (member 
    access), as well as the n-ary () (function call) operator, must always be 
    implemented as member functions, because the syntax of the language requires 
    them to.
    * Other operators can be implemented either as members or as non-members. 
        * When the compiler encounters an operator being used with a user 
        defined type, it rewrites the operator into two function calls. e.g. 
            ```cpp
            class Demo {
                Demo& operator++() { i++; return *this; }
            };
            Demo& operator++(Demo& d) { d.i++; return d; }
            
            Demo d; d++; // error: use of overloaded operator '++' is ambiguous 
            ```
        * rule of thumb 
            0. Some of them usually have to be implemented as non-member 
            functions, because their left operand cannot be modified by you. 
            1. If it is a unary operator, implement it as a member function.
            2. If a binary operator treats both operands equally (it leaves them 
            unchanged), implement this operator as a non-member function.
            3. If a binary operator does not treat both of its operands equally 
            (usually it will change its left operand), it might be useful to 
            make it a member function of its left operand’s type, if it has to 
            access the operand's private parts.

* common operators to overload
    * assignment operator
        ```cpp
        Demo& operator=(const Demo& other) {
        return *this;
        }
        ```
    * arithmetic operator
        * unary
            * Note that the postfix variant is implemented in terms of prefix. 
            * Also note that postfix does an extra copy.
            ```cpp
            Demo& operator++() {
                i++;
                return *this;
            }

            Demo operator++(int) { // must be int
                Demo tmp = *this;
                operator++();
                return tmp;
            }
            ```
        * binary
            * do not forget to obey the third basic rule operator overloading: If 
            you provide `+`, also provide `+=`

            ```cpp
            Demo& operator+=(const Demo& other) {
                i += other.i;
                return *this;
            }

            Demo operator+(const Demo& other) const {
                Demo result = *this;
                result += other;
                return result;
            }
            ```
    * stream insertion and extraction operator
        * their left operands are streams from the standard library, and you 
        cannot add member functions to those1, so you need to implement these 
        operators for your own types as non-member functions
        ```cpp
        std::ostream& operator<<(std::ostream& os, const Demo& demo) {
            os << demo.i;
            return os;
        }
        // note you cannot do demo << stdin and assume it uses >>
        std::istream& operator>>(std::istream& is, Demo& demo) {
            is >> demo.i;
            return is;
        }
        ```
    * comparision operator
    * logical operator
    * function call operator
    * operators for pointer-like types
    * subscript operator

Take-Away
=========
## big picture
* function
    * exception
    * overload: operator; resolution
* object
    * basics: ref; const
    * name
    * storage: lifetime; storage duration 
    * type: 
        * udts: num, union, class, struct; inheritance
        * conversion and cast

## conversion
* situation
    * `int a = 3.14; demo(3.14); return 3.14;` 
    * `if (3.14); switch(3.14)`
* built-in
    * numeric conversion: all numeric types (integral & floating point)
        * concept of promotion
    * pointer: Demo* -> void*; 0 -> Demo*; Demo* -> bool
* user-defined
    * derived -> base  
        * concept of upcast
    * 1-arg constructor; conversion operator
* "chaining"
    * 1 built-in conversion; OR
    * 1 user-defined conversion accompanied by 1 built-in before & after
* c++ abstract machine
    * `int a = 3.14`, store the temporary somewhere
* conversion operator
    ```cpp
    class B {
      public:
        operator A() {}
        operator A&() {}
        operator const A&() {}
    }
    ```

## cast
* static_cast
    * the old same
        * numeric conversion
        * pointer conversion
        * udts conversion (derived->base, 1-arg ctor, conversion op)
    * but can work with 
        * base->derived
        * explicit 1-arg ctor, conversion op
    * and useful when you'd like to make sure it's converted to a type
        * tho it can then be implicitly converted away
* const_cast
    * only work with pointer and reference
* dynamic_cast
* reinterpret_cast

## "conversion" & cast to reference
* `A& a = b; static_cast<A&>(b);` 
    * binding, not conversion
    * work with derived-base; not 1-arg ctor, or conversion op `operator A()`
* `A&& a = b; static_cast<A&&>(b)` - deferred
* `const int& a = 10;`

## overload resolution
* 1-arg
    * exact match
    * promotion
    * conversion
        * built-in
        * user-defined
* n-arg
    * no 1 worse
    * at least 1 better

## operator overloading
* guideline: don't do it
* member vs non-member
    * member only
        * forced by syntax: =, [], ->, (), 
    * non-member only
        * cannot modify left operand: <<, >>
    * others can be either member or non-member, regardless of binary/unary
        * non-member: binary treated equally
        * member: otherwise
* common operator
    * think about the operator syntax, what to pass in, what to return
    * assignment
    * ++, ++
    * +, +=
    * <<, >>
