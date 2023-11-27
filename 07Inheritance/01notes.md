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
