Take-Away
================================================================================
## inheritance
* accessibility modifier: 
    * public - remember this keyword, esp in multiple inheritance
    * private - for code reuse
    * protected - safely ignore
* member variables, member functions
    | A             | B             | C             |
    |---------------|---------------|---------------|
    | a, b, c       | b, d          | e, f          |
    | void demo1(); |               |               |
    | void demo2(); | void demo2(); |               |
    | void demo3(); |               | void demo3(); |
    |               | void demo4(); |               |
* hiding
    * is a form of name hiding
    * stop looking once we've found a name match, even if the argument 
    count/types don't match
* lifetime functions
    * ctor 
        * not inherited, not even interface; "inherited" in c++11
        * called in derived like a mem var
    * dtor
        * not inherited, indeed interface
        * called in derived like a mem var
    * assignment operator
        * inherited, but hidden
        * called in derived if generated; not if user-provided; like a mem var
    * swap
        * free function: inheritance, generation doesn't apply
        * move into class, make it protected, and call it `copy`, so that it can
        be used by derived 
    
## function binding
* choose implementation
    1. static checking
    2. choose static vs dynamic
        * if not virtual: static binding
        * if virtual: dynamic binding
        * scope resolution: static binding
* functions
    * non-virtual
        * static binding
    * virtual
        * substitution
            ```cpp
            void demo(A& a) {
                a.demo();
            }
            ```
        * override (vs. hiding)
            ```cpp
            /*
            | A                         | B             | C             |
            |---------------------------|---------------|---------------|
            | virtual void demo();      | void demo();  |               |
            | virtual void demo(int);   |               |               |
            */
            C c; A& a = c;
            a.demo(10); // ok
            c.demo(10); // error; 
            ```
        * keyword: `override`, `final`
        * virtual table
            * 1 per polymorphic class
            * contains only virtual functions; not non-virtual functions
            * 1 pointer to virtual table per instance of polymorphic class
    * pure virtual
        * derived must provide implementation to be concrete
        * pure virtual function can have implementation
* class
    * non-polymorphic
    * polymorphic: 1 virtual function
        * lifetime function implication
            * virtual destructor
                * Make the destructor virtual. 
                * If you haven’t explicitly implemented a destructor, then do 
                so. And make it virtual.
                * if `~A()` is virtual, `~B()` is also virtual
    * abstract: 1 pure virtual function
        * uninstantiable
        * lifetime function implication
            * abstract class also have constructor & destructor
            * copy-swap assignment operator will no longer work
    * interface: all pure virtual functions, no data members

## multiple inheritance
* inheritance
    * member variables
    * member functions
    * lifetime functions: ctor, dtor, assignment operator
        * assignment operator generation: calls both parents' `=`
    * name collision: ambiguous 
        ```cpp
        /*
            | B             | C             | D             |
            |---------------|---------------|---------------|
            | a             | a             |               |
            | demo();       | demo(int)     |               |
        */
        class D : public B, public C {}
        D d;      // ok
        d.a;      // error: member 'a' found in multiple base classes of different types
        d.demo(); // error: member 'demo' found in multiple base classes of different types
        ```
* function binding
    * static binding
    * dynamic binding
        * 2 virtual pointers
        ```cpp
        /*
            | B                 | C                 | D             |
            |-------------------|-------------------|---------------|
            | virtual demo();   | virtual demo();   |               |
        */
        class D : public B, public C {}
        D d; B& b = d; C& c = d;
        d.demo(); // error: member 'demo' found in multiple base classes of different types
        b.demo(); // B::demo()
        c.demo(); // C::demo()
        ```
* virtual inheritance
    * a C++ technique that ensures that only one copy of a base class’s members 
    are inherited by second-level derivatives (i.e. grandchild derived classes). 

## conversion
* implicit
    * upcast
* `static_cast`
    * upcast
    * downcast: undefined behavior
    * sidecast: error: no static relationship between the 2 types
* `dynamic_cast`
    * upcast, downcast, sidecast
    * only
        * pointers or references; AND
        * polymorphic classes 
    * if a dynamic_cast fails
        * to pointer - then a null pointer is returned
        * to a reference - then an `std::bad_cast` exception is thrown.

## paradigm
```cpp
// code reuse
class Base { 
    public: virtual void demo() = 0;
    protected: void baseDemo() { }
};
class Derived : public Base {
    public: void demo() { Base::baseDemo(); }
};

// substitution
class Base { 
    public: void demo() { derivedDemo(); }
    protected: virtual void derivedDemo() = 0;
};
class Derived : public Base {
    protected: void derivedDemo() { }
};
```

Abbreviated Take-away
================================================================================
* inheritance
    * public, private, protected
    * member variables
    * member functions 
    * lifetime: ctor, dtor, `=`
    * name hiding
* function binding
    * choose impl (compiler): 1. static checking 2. choose binding
    * virtual
        * concept: override
        * keyword: `orverride`, final
        * impl: virtual table
    * => polymorphic class
        * substitution
        * virtual destructor
    * pure virtual
        * derived must provide impl to be concrete
    * => abstract class
        * uninstantiable
        * copy-swap won't work
        * interface
* multiple inheritance
    * member variables:             from both parents
    * member functions:             from both parents 
    * lifetime: ctor, dtor, `=`:    from both parents
    * name collision
    * function binding: 2 virtual pointers
    * virtual inheritance
* conversion
    * implicit: upcast
    * static:
        * value: upcast
        * reference & pointer: upcast, downcast
    * static:
        * reference & pointer: upcast, downcast, sidecast
        * failure: nullptr, exception
* paradigm
    * code reuse
    * substitution