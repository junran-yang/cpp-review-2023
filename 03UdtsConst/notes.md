Section 01 Enumerations
=======================
## example
```cpp
enum Flavor {
    Pistachio = 5,
    Vanilla = 4,
    Strawberry // = 5
};

Flavor f0; // uninitialized
Flavor f1 = Pistachio;
Flavor f2 = (Flavor) 1; // no implicit conversion: int -> Flavor
int i = f0;             // implicit conversion: Flavor -> int
```

## syntax
* `enum`; `,`; `;`

## semantics
* type
    * An enum is a user defined type
    * The compiler gets to choose the underlying type. It just has to be big 
    enough to hold all of the enumerator values
    * C++ allows implicit conversion from an enumerator to an integral type, 
    such as int. It does not implicitly convert in the reverse direction.
* enumerator 
    * The enumerator names appear at the same scope as the enum itself
    * Enumerators can be set to have the same value as another enumerator
    * If you don’t provide a value for the first enumerator, then it is given 
    the value 0.
* `Flavor f0;` is uninitialized

## take-away
* think of it as `Flavor` inherit from `int`, ... 
    * (but not really, e.g. can use static_cast to convert `int` to `Flavor`)
    * `int` cannot be used as `Flavor`, e.g. pass to `void demo(Flavor);`
    * but `Flavor` can be used as an `int`, e.g. `switch (flavor) {case Pistachio: ...;}`
* ... with enumerators as user-defined literals
    * Flavor's value is not limited to those with an enumerator, just not as
    easy to use as those with an enumerator
    * by default starts with 0; +1; can provide value for it (int literal, const
    int variable, other enumerators); can have same value; 

Section 02 Class
================
* missing `;` at end of class definition might cause a compile error on the 
**following** line. Typically that's in a different file because this has been 
`#included`
* "scope resolution operator" (::)
* make data private unless it's a Plain Old Data (POD) class. This is a term 
used in the C++ standard, but we're using it slightly more generally to mean 
something that exists just to hold data
* member functions, member variables;
    - static member functions, static member variables;
    - instance functions, instance variables;

Section 03.01 Class Continued
=============================
* struct
    * class and struct are almost the same thing
    * the standard calls them both class types to save making a needless 
    distinction
    * inheritance is public by default with structs and private by default with 
    classes

* friend
    * syntax
        ```cpp
        friend class Icecream;
        friend void debug();
        friend void Icecream::create();
        ```
    * friendship side steps accessibility checks
    * friendship doesn't define a member, it declares a relationship
    * therefore it's not public or private 
    * friendship is not transitive

* size
    * all objects are at least one byte large
    * all objects need an identity. In C++ this is their address in memory.

* static
    * static data: its lifetime is similar to a global variable but its scope is
    restricted to this class
    * instance member functions implicitly get a this pointer. Static functions 
    do not.
    * static members need definitions

* this
    * `this` is occasionally useful, e.g. `Foo& demo() {return *this;}`
    * you normally do not need to use ‘this’ in C++ for disambiguation. A good 
    naming convention means that member variable names will not be confused with
    parameter names

* mangling
    * The name of the free function includes the name of the containing type and
    parameter information (to enable overloading)

* multiple definitions
    * The type definition will need to appear in every TU that meaningfully uses
    the type because its size will be needed. In contrast, a function's
    declaration, not definition, provides enough information about its shape
    * C++'s one definition rule permits the same type definition to appear in 
    multiple TUs. 
    * If you manage to have the same type name with different definitions in 
    different TUs, then your C++ program is not valid. There is a very good 
    chance that the linker can't tell you this though! Thus, ODR violations mean
    undefined behavior at run time.
    * e.g. `main` can still call `demo` even tho its private in `security`  
    ```cpp
    // Main
    class Demo {
        public:
            void demo();
    };

    int main(int argc, char** argv) {
        Demo d;
        d.demo();
    }

    // Security
    class Demo {
        private:
            void demo();
    };

    void Demo::demo() {
        std::cout << "securityDemo" << std::endl;
    }
    ```

Section 03.02 Union
===================
## example
```cpp
union Demo {
    char a = 'A';
    long b[4];

    void print() {
        std::cout << a << std::endl; // a = 'A';
        std::cout << b << std::endl; // b = -7764829128137965503 
                                     // (first byte: 65; last 7 bytes: random;)
    }
};
```
## syntax
* section is public by default
* unions cannot have base class
## semantics
* member
    * provides enough storage so that any one of its members may be stored
    * members are left-aligned
        * e.g. `char a; long b[4];`
            ```
             a
            |-|
            |--------|--------|--------|--------|   - 1 byte
             b0       b1       b2       b3
            ```
    * unions cannot tell you which of the values is currently valid
    * it is common to use a discriminator with a union. The discriminator is 
    another variable that tells you which member of the union was last set.
## use case
* Unions are often used to convert between the binary representations of 
    integers and floats:
    ```cpp
    union Demo {
        int i;
        float f;
    };

    // Convert floating-point bits to integer:
    Demo d;
    d.f = 3.14159f;
    printf("As integer: %08x\n", u.i); // 40490fd0
    ```
    * Although this is technically undefined behavior according to the C 
    standard (you're only supposed to read the field which was most recently
    written), it will act in a well-defined manner in virtually any compiler
* Another useful feature is the bit modifier
    ```cpp
    union HW_RegisterB {
        struct {
            unsigned char b1:1;
            unsigned char b2:1;
            unsigned char b3:1;
            unsigned char b4:1;
            unsigned char reserved:4;
        } bits;
        unsigned char byte;
    };
    HW_RegisterB reg;
    ```
## take-away
* think of it as 1 piece of data with different views

const
=====
## conversion between (const) object, reference, pointer
* rules
    * pointers/references can have different qualifiers to the referent object, 
    but they must be more restrictive
    * having a const reference to a non-const object doesn’t make the object 
    const. It is the reference’s view of the object that is const.
* examples
    * `Price p;` 
        * `Price* pp = &p; const Price* pp = &p;`
        * `Price& pr = p; const Price& pr = p;`
    * `const Price p;` 
        * `const Price* pp = &p; // Price* pp = &p; `
        * `const Price& pr = p;  // Price& pr = p; `
* the above rules applies when
    * pass by const reference/pointer
    * return by const reference/pointer

## a const object, reference, pointer
* if an object is const, then its members are treated as const
    ```cpp
    class Price {
    public:
        double bid = 0.99;
        double ask = 1.01;
    };

    class Security {
    public:
        Price* pp;
        Price& pr;
        Security(Price* _p) : pp(_p), pr(*_p) {}
        void demo() const {
            pp->bid = 0.97;
            pr.ask = 1.03;
        }
    };

    Price p;
    const Security s(&p);
    s.pp->bid = 0.98;
    s.pr.ask = 1.02;
    s.demo();
    ```
    * `Price* pr` is treated as `Price* const pr`, not `const Price* pr` 
    * `Price& pr` is treated as `Price& const pr`, not `const Price& pr` 
* For const obj, only const member functions may be called
    * e.g. assignment is not const member function
    * reason
        * when calling member funcs, its pointer is of type `const Price*`
        * `this` of non-const function has type `Price*`
* mutable
    * This means that they are not subject to the const restrictions.
    * Allows the object’s state to appear const externally to user
    * Internal, non observable, state may be changed, e.g. caching

## const member function
* The const/non-const ness of a function are part of its signature – they 
  take part in overloading. This means you can have two functions with the 
  same name but one is const and one is non-const.
    ```cpp
    class Demo {
    public:
        // Demo d;
        void demo() {
            std::cout << "demo" << std::endl;
        }
        // const Demo d;
        void demo() const {
            std::cout << "const demo" << std::endl;
        }
    };
    ```
* If you returning a non-const reference to a member variable in const member 
  function, the compiler would raise an error.
    ```cpp
    class Demo {
        public:
        // error: binding reference of type 'int' to value of type 'const int' 
        // drops 'const' qualifier
        int& demo() const {
            std::cout << "const demo" << std::endl;
            return a;
        }
        // error: cannot initialize return object of type 'int *' with an rvalue
        // of type 'const int *'
        int* demo() const {
            std::cout << "const demo" << std::endl;
            return &a;
        }
        int a = 10;
    };
    ```

## adv
* constexpr
* volatile
    * prevents certain optimizations

Dimystification
===============
## size in assembly
* assembly size
    * `movb $65, -8(%rbp)`: byte at `-8(%rbp)` set to 65
    * `movq $65, -8(%rbp)`: 8 bytes at `-8(%rbp)` set to 65, (last 7 bytes set 
                            to 0, the 8-byte num has value 65)
    ```
    // memory
    65 x x x x x x x
    |-|-|-|-|-|-|-|-|
    65 0 0 0 0 0 0 0

    // what we can think of register
    |-|-|-|-|-|-|-|-|
     0 0 0 0 0 0 0 65
    ```
* endienness
    * little endien: reversed bytes (not reversed 2/4/8-bytes)
    * Endianness inside a register makes no sense since endianness describes if 
    the byte order is from low to high memory address or from high to low memory 
    address. Registers are not byte addressable so there is no low or high 
    address within a register. What you are seeing is how your debugger print 
    out the data.
    * you don't need to think too much about endienness unless when interpreting
    the same memory region into different types 

## memory layout
* alignment & padding
    * How padding is done, is not part of the standard. So it can be done 
    differently on different systems and compilers. It is often done so that 
    variables are aligned at there size, i.e. size=1 -> no alignment, size=2 -> 
    2 byte alignment, size=4 -> 4 byte alignment and so on. For size=8, it is 
    normally 4 or 8 bytes aligned. The struct it self is normally 4 or 8 bytes 
    aligned. But - just to repeat - it is system/compiler dependent.
* the layout of member var: left to right
    ```asm
    movl    $10, -12(%rbp)  // a
    movl    $20, -8(%rbp)   // b
    movl    $30, -4(%rbp)   // c
    ```

## bit field
* a feature in C++ for specifying the size of struct and class members so 
that they occupy a specific number of bits, not the default memory size
```cpp
struct Loan {
    // principal variable can store maximum value of 1,048,575
    unsigned int principal : 20;
    // Maximum interest rate of 63
    unsigned int interestRate : 6;
    // Maximum period of 63 months
    unsigned int period : 6;
};
```
* operand of sizeof may not be a bit field
* if assign an int literal to a bit field with size 1: "implicit truncation from 
'int' to bit-field changes value from 15 to 1"

## friend 
* what's the compiling process
    * when the friend function/class of `Foo` uses `Foo`, compiler will
    look at `Foo`'s declaration, and bypass accessibility checks
* what if friend function is not defined? 
    * it's okay
* vs declaration
    * declaration provides type, forward declaration provides name where type isn't needed
    * friend provides relationship, used to identify what can bypass accessibility checks

TIL
===
## Enum
* enumerators by default starts with 0; +1; can provide value for it (int 
literal, const int variable, other enumerators); can have same value;  
* `Flavor f0;` is uninitialized

## Class
* inheritance is public by default with structs 
* all objects are at least one byte large
* friendship doesn't define a member, it declares a relationship
* `this` is occasionally useful, e.g. `Foo& demo() {return *this;}`
* If you manage to have the same type name with different definitions in 
different TUs, then your C++ program is not valid. There is a very good chance 
that the linker can't tell you this though

Take-away
=========
## Enum
* think of it as a specialied `int` ...
    * `int` cannot be used as `Flavor`, e.g. pass to `void demo(Flavor);`
    * but `Flavor` can be used as an `int`, e.g. `switch (flavor) {case Pistachio: ...;}`
* ... with enumerators as user-defined literals
    * Flavor's value is not limited to those with an enumerator, just not as
    easy to use as those with an enumerator

## Union
* think of it as 1 piece of data with different views, e.g. `char a; long b[4];`
    ```
     a
    |-|
    |--------|--------|--------|--------|   - 1 byte
     b0       b1       b2       b3
    ```
* use case 
    * convert between the binary representations of integers and floats
    * use as the bit modifier
    * use with a discriminator

## Class: All about memory
* member layout
    * padding is often done so that variables are aligned at there size
    * the layout of member var: left to right (also applies to arrays)
* bit field
    * a feature in C++ for specifying the size of struct and class members so 
    that they occupy a specific number of bits, not the default memory size
* size in assembly
    * `movb $65, -8(%rbp)`: byte at `-8(%rbp)` set to 65
    * `movq $65, -8(%rbp)`: 8 bytes at `-8(%rbp)` set to 65, (last 7 bytes set 
                            to 0, the 8-byte num has value 65)
    * little endienness come in handy

## const
* conversion
    ```
    Price       const Price
      |     \        |
      v      \       v
    Price&/*    const Price&/*
    ```
    * applies to: local, pass, return
* const object
    * all member vars are const, except for mutable vars
        * `Price* const`, `Price& const`, not the other way around
    * a `const Price*` is passed as the first argument to const member funcs, 
      so that the ops on all member vars are const (recursively)
        * const obj can only call const member funcs, bc `this`
        * notice: cannot return non-const `Price&`

## Deferred
* scope
    * scope of a name vs scope of a variable
    * global scope vs global lifetime
    * life time vs scope