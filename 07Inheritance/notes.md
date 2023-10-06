Section 01: Inheritance
=======================
## running example
```cpp
class Security {};
class CmdyFuture : public Security {};
class Equity : public Security {};
class GovtBond : public Security {};
```

## purpose of inheritance
* model is-a-kind-of relationships (enable substitutability)
* **not** code re-use in derived class
    * However, substitutability will lead to potential code re-use for the 
    client.

## lifetime member functions
* The derived class doesn’t need its own copy-cons, assignment, or destructor. 
The compiler provided ones delegate to the base class, and hence will dtrt
* Constructors are not inherited in C++03. In C++11 it is possible to inherit 
constructor with using.
* The implementation of swap must use the function provided alongside Security
* The compiler provided copy constructor automatically copy constructs base 
classes.
* The compiler provided assignment operator automatically calls the base class’s
assignment operator.
* The compiler provided copy constructor and assignment operator hide the base 
class implementations.

## member variables
* An instance of the derived class must be at least big enough to hold all of 
  the instance data of the base class plus all of the instance data of the 
  derived class.
    * Padding may be added because of alignment.
    * We deliberately didn’t say it was the size of the base class plus the size
      of the derived class because of the Empty Base Class Optimization
        * The size of any object or member subobject is required to be at least 
        1 even if the type is an empty class type, in order to be able to 
        guarantee that the addresses of distinct objects of the same type are 
        always distinct.
        * However, base class subobjects are not so constrained, and can be 
        completely optimized out from the object layout 
* It doesn’t matter whether the instance data are private. They’re still 
contained within the complete object 
* The layout of the members in the derived class are made so that the members 
from the base class are in the same positions as if it was the base class. This 
enables a derived to be treated as a base (substitutability)

## implicit conversions
```cpp
CmdyFuture cf;

Security& sr = cf;
Security* srp = &cf;
Security s = cf; // slicing
```
* upcasts: implicit
* downcasts: not implicit

## accessibility modifiers
* private inheritance is occasionally useful when inheritance is being used for 
  re-use
    * developers should still consider using composition instead
* protected inheritance can be safely ignored

## note to self
* paradigm
    ```cpp
    class Base {};
    class Derived1 : public Base {};
    class Derived2 : public Base {};
    class Derived3 : public Base {};

    void demo(Base& base) { }

    Derived1 d1;
    Derived1 d2;
    Derived1 d3;
    demo(d1);
    demo(d2);
    demo(d3);
    ```

Section 02: Function Binding
============================
## choose implementation (from compiler's point of view)
```cpp
class A {
    public:
    virtual demo(); // OR,
    demo();
};

class B : A {};
class C : A {};

void demo(A& a) {
    a.demo();
    a.A::demo();
}
```
* steps 
    * the function is resolved by the compiler. this is static
    * the compiler looks to see whether the function is marked as virtual
    * if it is not marked virtual, then the compiler performs static binding
    * if it is virtual, then the compiler emits code to perform dynamic binding
        * this code determines the most derived type of the object and use that 
        information to choose the most overridden implementation of the function
* if you use scope resolution you are requesting that dynamic binding is not 
used for this particular call.

## static binding
* function resolution recap
    * find the name
    * find a function that can be called with the arguments supplied
    * find the best match

* steps
    * the type of the expression to the left of . or -> tells us where to start 
    looking for the name.
    * if we find the name in the derived class, then stop looking. If we don’t,
    then proceed to the base class, and so on
    * once the compiler know which class a name can be found in, it can then 
    perform the remaining stages of function resolution.

* explicit binding
    ```cpp
    derived.Base::demo();
    ```

## dynamic binding
* steps
    * the compiler statically still needs to check that the call is possible
    * the most derived type of the object dynamically determine which 
      implementation is chosen
        * choose the most overridden implementation of the function

* cost of dynamic binding
    * the potential cost to performance is paid each time the function is called
    * the storage cost is paid the first time a virtual function is introduced
    * dynamic binding needs to be explicitly enabled because it has a cost 

* virtual
    * virtual turns on dynamic binding
    * once a function has been made virtual, it is virtual from that point in 
    the inheritance hierarchy down. virtual can never be removed
    * any derived class may override this function, but can only do so if it 
    exactly matches the name and signature
    * "RTTI" - run time type information
* final 
    * a function that must be virtual (it’s been inherited as virtual), but 
    cannot be overridden in derived classes.
* override 
    * this asks the compiler to check that a function is definitely overriding, 
    and not hiding

## virtual table
* vtable
    * there is a vtable created for each type that contains a virtual function. 
    two instances of the same type will point to the same vtable
    * the vtable contains pointers to function implementations, and data used 
    for other purposes
    * every instance of that type has an additional member added, the vptr. the 
    vptr points to the type’s vtable. 

* cost
    * performance cost (dynamic binding only)
        * is paid each time the function is called
    * storage cost per class
        * is paid the first time a virtual function is introduced
    * storage cost per instance
        * objects don't need more than one vptr, so once the price has been 
          paid, it won’t get 'charged' again 
            * a lie! multiple inheritance and virtual inheritance mean that this 
            statement is incorrect.
        * for a small type, the addition of a vptr might increase its size 
        substantially

## virtual destructor
* if you have a virtual function (declared or inherited), then you need a 
virtual destructor
* if you have provided a virtual function in a class, then you expect the class 
to be used as a base class. Make the destructor virtual. If you haven’t 
explicitly implemented a destructor, then do so. And make it virtual.

Section 03.01: Abstract Class
=============================
* pure virtual function
    * a virtual function can be made pure. To do this add `=0` to the end.
    * a pure virtual function does not need an implementation. 
    * it is, however, permitted to have one. It’s still pure virtual. The 
    function can be called through static binding. 

* abstract class
    * if a class has at least one pure virtual function, then the class is an 
    abstract base class
    * an abstract base class is uninstantiable

* lifetime functions
    * the copy-swap assignment operator will no longer compile
    * swap could do real damage if it was used with 2 different base class
    * the compiler provided `operator=` for derived class is no longer viable
    * by making the choice to prevent assignment, we’re making `CmdyFuture` a 
    type with reference semantics. This is a type for which an instance’s 
    identity is important, rather than just its value. This distinction is not 
    recognized by the C++ standard.

* interface
    * C++ has no distinct interface type like Java. A class consisting of pure 
    virtual functions and no instance data serves the same purpose.

Section 03.02: Multiple Inheritance
===================================
* multiple inheritance
    * you can multiply inherit using a list of base classes
    * each gets its own public/private/protected modifier

* virtual inheritance
    * diamond of doom
        * happens when a common base class is inherited through multiple paths
        * it causes problems with duplication of instance variables.

* mixins
    * this is inheritance for code-reuse
    * but it’s done with awareness of its purpose. A well designed mixin is 
    using a language feature, not abusing substitutability

Section 03.03: dynamic_cast
===========================
```cpp
CmdyFuture cf(Figi("BBG00F0S24M9"), 5);
Security* s_p = &cf;
JsonSerializer* j_p = &cf;

static_cast<CmdyFuture*>(s_p);      // allowed, but dangerous
static_cast<CmdyFuture*>(j_p);      // allowed, but dangerous
static_cast<Security*>(j_p);        // error
static_cast<JsonSerializer*>(s_p);  // error
dynamic_cast<CmdyFuture*>(s_p);     
dynamic_cast<CmdyFuture*>(j_p);
dynamic_cast<Security*>(j_p);
dynamic_cast<JsonSerializer*>(s_p);
```
* static_cast
    * zero run-time cost
    * downcast - undefined behavior: cannot check that it was correct
    * sidecast - error: no static relationship between the 2 types
* dynamic_cast 
    * downcast or sidecast
    * pointers or references
    * runtime information must be available for the object, i.e. it must have a 
    vtable
    * if a dynamic_cast fails
        * to pointer - then a null pointer is returned
        * to a reference - then an `std::bad_cast` exception is thrown.

Demystification
===============
## hiding vs overriding
* function hiding 
    * is a form of name hiding
    * essentially, the functions in the base class are always hidden by 
    functions of the same name in a derived class; no matter if the function in 
    the derived class overrides a base class' virtual function or not
* function overriding 
    * is linked to the concept of virtual functions
        > ... then `Derived::vf` is also **virtual** (whether or not it is so 
        > declared) and it **overrides** `Base::vf`
    * final overrider
        * if you have an object of type S, the final overrider is the first 
        overrider you see when traversing the class hierarchy of S back to its 
        base classes
* to demonstrate
    ```cpp
    /*
    | A                         | B             | C             |
    |---------------------------|---------------|---------------|
    | virtual void demo();      | void demo();  |               |
    | virtual void demo(int);   |               |               |
    */
    C c;
    A& a = c;
    a.demo(10); // ok
    c.demo(10); // error; 
    ```
    * note to self: 
        * `c.demo(10)` failed at step 1: statically check that the call is 
        possible. This means that there is a suitable function that can be found
        through the type of the expression to the left of `.` or `->`. However, 
        `A::demo(int)` is hidden by `B::demo()`
        * `a.demo(10)` is successful because C's virtual table still point 
        `demo(int)` to `A::demo(int)`
        * `B::demo()` did not override `A::demo(int)`
* note to self
    * hiding is from derived class'es point of view
    * overriding is from base class'es point of view

## everything (else) about virtual
* what happens when a virtual function is called inside a non-virtual function? 
    - polymorphic behaviour works even when a virtual function is called 
    inside a non-virtual function

* can we override private virtual method? 
    - C++ has access control, but not visibility control. This means that 
    private functions are visible but not accessible. A private virtual 
    function can be overridden by derived classes, but can only be called 
    from within the base class.

## lifetime functions
* constructor
    * not inherited, not even an interface
        * "There can't be any proper inheritance of constructors in C++, ..."
        * "what do we mean by "Foo is inherited"? We mean that if objects of 
        class A have Foo in its interface, then objects of class B which is a 
        subclass of A also have Foo in its interface."
        * "Constructors aren't a part of objects' interface. They belong 
        directly to classes. Classes A and B may provide completely different 
        sets of constructors. No "being inherited" here."
    * "inherited" in c++11
        ```cpp
        class B : public A {
            using A::A;
        };
        ```
        * "in C++11, you can instruct the compiler to generate a set of 
        constructors for you that take the same arguments as the constructors 
        from the base class and that just forward those arguments to the base 
        class. Although it is officially called inheritance, it isn't truly so 
        because there still is a derived-class specific function. Now it just 
        gets generated by the compiler instead of explicitly written by you."
    * called in derived class'es constructor
    * no virtual constructor
        * `error: constructor cannot be declared 'virtual'`
        * "To create an object you need complete information. In particular, you 
        need to know the exact type of what you want to create. Consequently, a 
        "call to a constructor" cannot be virtual."
    * abstract classes also have constructors
        * used in derived class
        * just that you can not instantiate the base class itself
* destructor
    * not inherited, indeed part of the interface
        * "Destructors indeed are a part of each object's interface, since the 
        object's user is responsible for calling them. Each object has exactly 
        one destructor: its own destructor, which might optionally be a virtual 
        one. It is always its own, and it's not inherited."
    * called in derived class'es destructor
        * "Even if you write a destructor, you do not need to destruct the base.
        The compiler does this automatically."
        * the above applies to member variables as well
    * virtual desturctor
        * if we make A's dtor virtual, does B's dtor become virtual? 
            ```cpp
            class A { public: ~A() { std::cout << "~A()\n"; } };
            class B : public A { public: ~B() {std::cout << "~B()\n";}};
            class C : public B { public: ~C() {std::cout << "~C()\n";}};

            B* b = new C;
            delete b; // prints ~C(), ~B(), ~A()
            ```
            * if it does not make `~B` virtual, it would otherwise print out 
            `~B(), ~A()`
        * you can make it pure virtual, but still need to provide implementation
    * virtual destructor paradigm
        * from lecture - "if you have provided a virtual function in a class, 
        then you expect the class to be used as a base class. Make the 
        destructor virtual. If you haven’t explicitly implemented a destructor, 
        then do so. And make it virtual."
        ```cpp
        A *a = new B();
        delete a;
        ```
        * "[In delete b], if the static type of the object to be deleted is 
        different from its dynamic type, the static type shall be a base class 
        of the dynamic type of the object to be deleted and the static type 
        shall have a virtual destructor or the behavior is undefined."
        * "to sum up, always make base classes' destructors virtual when they're 
        meant to be manipulated polymorphically." 
        * "if you want to prevent the deletion of an instance through a base 
        class pointer, you can make the base class destructor protected and 
        nonvirtual; by doing so, the compiler won't let you call delete on a 
        base class pointer."
    * abstract classes also have constructors
        * used in derived class
* assignment operator
    * inherited, just like other regular functions
        ```cpp
        A a;
        B b;
        b = a; // error
        b.A::operator=(a); // ok
        ```
        * `B::operator=` is generated
        * `b = a` found the name `operator=` in `B`, but no matching arg  
        * `A::operator=` is inherited, therefore can be accessed using scope 
        resolution
    * when generated, base's `=` is called in derived's `=`; but not 
    automatically called in user supplied `=`
    * virtual assignment operator
        * you can make it virtual, just like any other regular functions
        * won't dive in too deep here since I don't see much use case for this
        * you can make it pure virtual, again won't dive in too deep here 
    * in abstract class 
        * copy-swap assignment operator will no longer work
    * "Assignment and inheritance often do not ‘play nicely’ together. It is 
    very common to hide the assignment operator for classes that represent some 
    kind of entity." 
* swap
    * free function, so inheritance, function generation, virtual doesn't apply
    * "swap could do some real damage if it was used with 2 subtypes. parts will 
    be transplanted from one to the other. To prevent this, we remove the free 
    function form of swap. We move it to a protected member function. Derived 
    classes can use it in their implementation of swap."

## multiple inheritance
* non-polymorphic
    ```cpp
    /*
        | B, C          | D             |
        |---------------|---------------|
        | B: a          | D:            |
        | C: a          |               |
        |               |               |
        | B: demo();    | D:            |
        | C: demo(int); |               |
    */
    class B : { public: int a = 20; void demo() {} };
    class C : { public: int a = 30; void demo(int) {} };
    class D : public B, public C { public: int d = 40; };

    D d;        // ok
    d.a;        // error: member 'a' found in multiple base classes of different types
    d.demo();   // error: member 'demo' found in multiple base classes of different types
                // note: the function signature is actually different
    ```
    * note to self
        * member variables
            * from both parents; name collision; name hiding
        * member functions
            * from both parents; name collision; name hiding
            * note: colliding names with different signature are still not allowed

* polymorphic
    ```cpp
    /*
        | B, C                  | D             |
        |-----------------------|---------------|
        | B: virtual demo();    | D:            |
        | C: virtual demo();    |               |
    */
    class B { public: virtual void demo() { std::cout << "B::demo()" << std::endl; } };
    class C { public: virtual void demo() { std::cout << "C::demo()" << std::endl; } };
    class D : public B, public C { };
    int main() {
    D d;
    B& b = d;
    C& c = d;
    b.demo();     // B::demo()
    c.demo();     // C::demo()
    d.demo();     // error: member 'demo' found in multiple base classes of different types
    }
    ```
    * note to self
        * 2 vtable pointer

## virtual inheritance
* diamond problem
    ```cpp
    class A { public: int a = 10; void demo() {} };
    class B : public A { public: int b = 20; };
    class C : public A { public: int c = 30; };
    class D : public B, public C { public: int d = 40; };

    D d;            // movl    $10, -32(%rbp)
                    // movl    $20, -28(%rbp)
                    // movl    $10, -24(%rbp)
                    // movl    $30, -20(%rbp)
                    // movl    $40, -16(%rbp)
            
    d.a;            // error: non-static member 'a' found in multiple base-class subobjects of type 'A'
    d.A::a;         // error: ambiguous conversion from derived class 'D' to base class 'A'
    d.demo();       // error: non-static member 'demo' found in multiple base-class subobjects of type 'A'
    d.A::demo();    // error: ambiguous conversion from derived class 'D' to base class 'A'
    ```
* virtual inheritance
    ```cpp
    /* 
    ok: 
    | ----> B - - > A
    D
    | ----> C - - > A

    error:
    | ----> B ----> A
    D
    | ----> C - - > A

    error:
    | - - > B ----> A
    D
    | - - > C ----> A

    ok: 
    | ----> B1 ----> B - - > A
    D 
    | ----> C1 ----> C - - > A
    */
    ```
    * virtual inheritance is a C++ technique that ensures that only one copy of a 
    base class’s members are inherited by second-level derivatives (a.k.a. 
    grandchild derived classes). 

## conversions
* implicit
    * upcast only, no downcast, no sidecast
* `static_cast`
    * value: 
        * upcast only, no downcast, no sidecast
    * reference & pointer
        * upcast, downcast(undefined behavior), no sidecast(error)
    ```cpp
    A a;
    static_cast<B>(a);  // error: no suitable user-defined conversion from "A" to "B" exists
    static_cast<B&>(a); // ok
    ```
* `dynamic_cast` can only work with references and pointers
    ```cpp
    // error: invalid target type 'float' for dynamic_cast; target type must be 
    //        a reference or pointer type to a defined class
    dynamic_cast<float>(10); 
    ```
* `dynamic_cast` can work with upcast, downcast, sidecast
    ```cpp
    class A { public: int a = 10; };
    class B : public A { public: int b = 20; };
    ```
    * upcast
        ```cpp
        dynamic_cast<A&>(b);
        ```
    * downcast
        * polymorphic class only (cast-from object)
            ```cpp
            B b;
            A& a; 
            dynamic_cast<B&>(a); // compile time error: 'A' is not polymorphic

            // adding a virtual destructor 
            class A { public: int a = 10; virtual ~A() = default; };
            dynamic_cast<B&>(a); // compile time ok
            ```
        * exception
            ```cpp
            A a;
            dynamic_cast<B&>(a); 
            // libc++abi: terminating with uncaught exception of type 
            //            std::bad_cast: std::bad_cast
            ```
    * sidecast
        * polymorphic class only (cast-from object)
        * exception

* polymorphic class (cppreference): 
    * a non-union class that declares or inherits at least one virtual function

## common paradigm (note to self) 
    ```cpp
    class Base { 
        public: void demo() { derivedDemo(); }
        protected: virtual void derivedDemo() = 0;
    };
    class Derived : public Base {
        protected: void derivedDemo() { }
    };

    // Paradigm 2
    class Base { 
        public: virtual void demo() = 0;
        protected: void baseDemo() { }
    };
    class Derived : public Base {
        public: void demo() { Base::baseDemo(); }
    };
    ```

side notes
==========
* [x] what is the case with Logger by oremanj? 
    - it's not because Logger is not polymorphic
    - non-polymorphic can still do `A* = new B;`
    - it's because its destructor is marked protected
    - it's actually more constraining than `virtual = 0;`
    - the latter can still do `delete a;`