## rvalue reference
```cpp
A a;
A&  a_ref1 = a;
A&& a_ref2 = a;

A&  a_ref3 = A(); // Error
A&& a_ref4 = A();
```
* an rvalue reference behaves just like an lvalue reference except that it can 
bind to a temporary 
* the combination of rvalue references and lvalue references is just what is 
needed to easily code move semantics.
* it is up to client code to overload key functions on whether their argument is
an lvalue or rvalue (e.g. copy constructor and assignment operator).

## some epiphany
* rvalue reference
    * so that we have 2 func overloading
    * vector impl is based on heap! therefore move makes a lot of sense there!
    * do we call move constructor when we pass something by rvalue? 
* move
    * pass by &&
        * normal function
        * move constructor
    * copying: may use move constructor
        * vanilla copy
        * pass by value
        * return by value
            * rvo (return value optimization)

## questions
* is there move constructor involved in a function call? 
    - of course not its a reference for god's sake
    - when pass by value, and a rvalue is provided, then move ctor
* how do we call a move constructor
    - just when rvalue is provided to the ctor
* do we make a copy of things when we return by value? 
    - yes, logical copy
* why is rvalue reference a lvalue? 
