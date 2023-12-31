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