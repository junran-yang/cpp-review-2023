Section 01: Initialization 
==========================
## basic syntax
 type       |local                              
|-----------|-------------------|
 default    |int a;                             
 direct     |int a(1);
 copy       |int a = _a;                        
 value      |int();*                            
* there is no syntax for value initializing a named variable

## primitive types
 type       |behavior           
|-----------|-------------------|
 default    |no-op              
 direct     |as defined         
 copy       |as defined         
 value      |zero-initialized   

## user defined types
* constructor
    * vs initialization: "constructors are called to perform initialization"
    * memory has already been allocated for the object. the object is 
    initialized in place

 type       |behavior
|-----------|---------------------------------------------------------------|
 default    |default constructor
 direct     |direct constructor, copy constructor
 copy       |copy constructor (non-explicit)
 value      |default constructor, OR "value constructor"* 

* "value constructor": 
    * from slides 
        * if the type doesn’t have any user-supplied constructors then it 
        performs value-initialization for all the members.
    * note to self
        * use "value constructor" if there is
            * implicitly generated zero-argument ctor
            * explicit `=default`ed zero-argument ctor
        ```cpp
        class Price {
        public:
            // Price() : bid(0.99), ask(1.01) {}
            double bid;
            double ask;
        };

        class Security {
        public:
            Price p;
        };

        Security s1;                // random bits
        Security s2 = Security();   // 0, 0
        // after uncommented Price() : bid(0.99), ask(1.01) {}
        Security s3 = Security();   // 0.99, 1.01
        ```
    * user-provided
        * "a function is user-provided if it is user-declared and not explicitly 
        defaulted or deleted on its first declaration."
        * user-declared
            * `= delete`
            * `= default`
            * user-provided

* compiler generated functions
    * rules
        * default constructor   - if no user-**declared** ctor
        * "value constructor"   - if no user-declared ctor
        * copy constructor      - if no user-declared copy ctor
        * copy assignment       - if no user-declared copy assignment op
        * destructor            - if no user-declared destructor
    * generated special member functions are implicitly **inline**
    * `= delete` to not generate func
        * deleted functions are **declared**
        * they are just declared to be deleted
        * member function: prevent function generation
        * member & free function: prevent function overloading
    * `= default` to explicitly generate func
        * "using `= default` for their **definitions**
        * inline b/c "a function defined within a class definition is an 
        inline function"
        * to explicit generate member functions
    * to summarize declaration
        * normal, `delete`, `default`

* default constructor
    * `Demo(int a = 1, int b = 2);` 
        * is a default constructor
        * thus preventing the generation of one 

* copy constructor
    * `Demo(const Demo&)` `Demo(Demo&)` 
        * are they both copy ctors
            * they can both exist
            * they are both considered to be copy constructor
            * you can define both with `= default`
        * function generation
            * the existence of one prevents the generation of another
            * if none declared, compiler will generate `Demo(const Demo&)`

## syntax expanded
* it's time to read cppreference
    * default initialization
        * regular
            * `T object;`
        * pointer
            * `new T`
    * direct initialization syntax (`{}` elided)
        * regular
            * `T object(arg);`
            * `T object(arg1, arg2, ...);`
        * member
            * `Class::Class() : member(args, ...) { ... }`
        * pointer
            * `new T(args, ...)`
        * cast
            * `T (other)`
            * `T (arg1, arg2, ...)`
            * `static_cast<T>(other)`
    * copy initialization syntax (`{}` elided)
        * regular
            * `T object = other;`
        * pass & return
            * `f(other)`
            * `return other;`
            * `throw object;`, `catch (T object)`
    * value initialization (`{}` elided)
        * regular
            * `T()`
        * member
            * `Class::Class(...) : member() { ... }`
        * pointer
            * `new T()`

* summary: member & pointer
     type       |member                 |pointer                        
    |-----------|-----------------------|-----------------------------------|
     default    |by default             |new int;                       
     direct     |a(1, 2, 3);* a(_a);    |new int(1, 2, 3); new int(_a); 
     copy       |-                      |-
     value      |a();                   |new int();                     
    * int a(1, 2, 3) [大雾], just for illustration

* cast is also direct initialization syntax
    * func-style 
        * int(1); int a(1, 2, 3); int(_a); 
    * explicit (one-arg only, not copy)
        * static_cast<int>(1); 
        * static_cast<int>(_b); (suppose B has a conversion operator)

* pass & return is also copy initialization syntax
    * `f(other)`
    * `return other;`
    * `throw object;`, `catch (T object)`

## copy initialization
* cannot use explicit copy constructor
    * copy initialization **cannot** use explicit ctor 
    * default, direct, value initialization **can** use explicit ctor 

* must use copy constructor (convert with direct ctor and then copy)
    ```cpp
    // build with -fno-elide-constructors
    class Demo {
    public:
        Demo(int _a) : a(_a) {
            std::cout << "direct " << a << std::endl;
        } 
        Demo(const Demo& _demo) : a(_demo.a) {
            std::cout << "copy " << a << std::endl;
        } 
        int a;
    };

    void demo(Demo d) {}

    int main(int argc, char** argv) {
        Demo d1 = 10;   // copy init,   direct ctor + copy ctor
        Demo d2(20);    // direct init, direct ctor

        demo(30);       // copy init,   direct ctor + copy ctor
        demo(d1);       // copy init,   copy ctor
    }
    ```
    * for g++, the option `-fno-elide-constructors` will preserve the copies 
    that are normally elided.

* can happen implicitly, e.g. passing or returning by value
    * this incurs 1 **logical** copy
    ```cpp
    Demo smaller(const Demo& d1, const Demo d2) {
        return d1.a < d2.a ? d1 : d2;
    }

    void demo() {
        Demo d1(10);
        Demo d2(20);

        Demo d3 = smaller(d1, d2); // 3 logical copies
        Demo d4(smaller(d1, d2));  // 3 logical copies
    }
    ```

* copy ctors must take a reference, which should almost certainly be const

Section 02: Destruction
=======================
## exception safety
* destructors are called as the stack is unwinding, regardless of the reason...
    * unwinding is a procedure which takes place when an exception is thrown
    * other places destructors can be called
        * reach the end of the block
        * return
* ... it would be a bad time to throw an exception. 
    * you never allow exceptions to escape from a destructor
    * exceptions may occur while the destructor is running, 
    * but they will need to be caught within the dtor and not allowed to escape
    * normally the only viable action here is to log the issue or abort. 

## stack overflow answers on exception safety
* why is it bad for destructor to have exceptions? 
    1. you cannot have a second exception being thrown while the first is active 
    2. Post C++11 destructors are by default `noexcept(true)` and this will (by 
       default) call terminate if an exception is escapes the destructor.
        ```cpp
        // to allow excepetion to escape the destructor
        ~Demo() noexcept(false) {
            throw 20;
        }
        ```
* how do you handle errors in destructor? 
    * So in effect you pass the responsibility onto the user. If the user is in 
    a position to correct exceptions they will manually call the appropriate 
    functions and processes any errors. If the user of the object is not worried
    (as the object will be destroyed) then the destructor is left to take care 
    of business.
    * Anything dangerous (i.e. that could throw an exception) should be done via
    public methods (not necessarily directly). The user of your class can then 
    potentially handle these situations by using the public methods and catching
    any potential exceptions.
    * The destructor will then finish off the object by calling these methods 
    (if the user did not do so explicitly), but any exceptions throw are caught 
    and dropped (after attempting to fix the problem).
    * e.g. `std::fstream` and `close()`

* expand: stack of exceptions
    * you cannot have a second exception being thrown while the first is active 
    ( exception thrown during stack unwinding that hasn't entered a catch block 
    yet), else, the runtime will terminate your program.

* expand: why is it bad for dtor to have exceptions?
    * Throwing out of a destructor can result in a crash, because this 
    destructor might be called as part of "Stack unwinding". Stack unwinding is 
    a procedure which takes place when an exception is thrown. In this 
    procedure, all the objects that were pushed into the stack since the "try" 
    and until the exception was thrown, will be terminated -> their destructors 
    will be called. And during this procedure, another exception throw is not 
    allowed, because it's not possible to handle two exceptions at a time, thus,
    this will provoke a call to abort(), the program will crash and the control 
    will return to the OS.

## compiler generated destructor
* implicitly defaulted vs explicited `= default`ed
    * something to do with `virtual`, deferred
* `= default` vs `{}`
    * `{}` - user provided - the class is no longer `trivial``

Section 03: Assignment
======================
## assignment
```cpp
class Demo {
  public:
    Demo& operator=(Demo _demo) {}
    Demo& operator=(const Demo _demo) {}
    Demo& operator=(Demo& _demo) {}
    Demo& operator=(const Demo& _demo) {}
};
```
* function overloading
    * `Demo` & `const Demo`: redefinition
    * `Demo`/`const Demo`, `Demo&`, `const Demo&` can exist at the same time,
    but function calls may be ambiguous
* what is the signature of a copy assignment operator
    > A user-declared copy assignment operator X::operator= is a non-static 
    > non-template member function of class X with exactly one parameter of type
    > `X`, `X&`, `const X&`, `volatile X&` or `const volatile X&`.
    * return value types don't matter
* which one is generated for you
    * only `Demo&`, `const Demo&` can be `= default`
    * `const Demo&`, since you can pass a `const Demo` as argument

## user-defined copy assignment operator
* ideally strong exception-safe
* `Demo& operator=(Demo other);`
* then use swap for all the members

## side notes
* Assignment obliterates an existing state and gives the object a new one. It is
not initialization. It must clean up existing state first.
* The rule of the big three is a very simple rule that protects you from subtle 
bugs. Embrace it!

Section 04: Exception
=====================
# exception safety
* 4 levels of exception guarantee
    * no-throw exception guarantee
        * handles all possible exceptions that incurs from within
        * e.g. destructor, any function that doesn't throw (`noexcept(true)`)
    * strong exception guarantee
        * no state change, no resource leak
        * e.g. constructor, any function that doesn't change states 
    * basic exception guarantee
        * valid state change, but no resource leak
    * no exception guarantee
* notes to self
    * not for a class, but for a function
    * to be strong exception safe, a function can only call strong exception 
    safe or higher functions within itself
    * destructor: no-throw
    * constructor, copy constructor: 
        * ideally strong exception-safe
        * can throw and let caller handle
    * copy assignment operator: 
        * ideally same as above
* side note: exception-neutral guarantee
    * if an exception is thrown from a template parameter (e.g. from the 
    Compare function object of std::sort or from the constructor of T in 
    std::make_shared), it is propagated, unchanged, to the caller.

Note to Self
============
* move
    * pass by &&
        * normal function
        * move constructor
    * copying: may use move constructor
        * vanilla copy
        * pass by value
        * return by value
            * rvo (return value optimization)

Take-away
=========
## initialization
* initialization syntax + behavior
     type       |local       |behavior         |behavior
    |-----------|------------|-----------------|------------------------------|
     default    |int a;      |no-op            |default ctor
     direct     |int a(1);   |as defined       |direct ctor, copy ctor
     copy       |int a = _a; |as defined       |copy ctor (non-explicit)                                                 
     value      |int();      |zero-initialized |default ctor, OR "value ctor" 
* more syntax
    * member & pointer
    * cast is also direct initialization syntax
        * func-style: `int(1); int a(1, 2, 3); int(_a);`
        * explicit (one-arg only, not copy): `static_cast<int>(1);`
    * pass & return is also copy initialization syntax
* copy initialization recap
    * must use
        * non-explicit
        * copy ctor
    * number of logical copies: pass, return, copy init/assignment = 3
        ``` cpp
        Demo smaller(const Demo& d1, const Demo d2) { 
            return d1.a < d2.a ? d1 : d2; 
        }
        Demo d3 = smaller(d1, d2); // 3 logical copies
        Demo d4(smaller(d1, d2));  // 3 logical copies
        ```

## compiler-generated function
* "value constructor"
    * generated if no implicit or explicit defaulted default constructor
* constructor
    * generated if no user-declared constructor
* copy constructor
    * 2 forms: `Demo&`, `const Demo&`
    * `const Demo&` is generated if none of above is user-declared
    * most things shouldn't be copiable anyways
* destructor
    * `= delete`, `= default` deferred to inheritance
* copy assignment operator
    * 3 forms: `Demo`, `Demo&`, `const Demo&`
    * return type doesn't matter, usually `Demo&`
    * `const Demo&` is generated if none of above is user-declared
    * most things shouldn't be copiable anyways

## exception safety of each
* constructor (incl. copy constructor)
    * okay to throw
    * ideal if no state change, no resource leak
* destructor
    * no-throw by default with `noexcept(true)`
    * b/c there can't be 2 active exceptions at the same time
* copy assignment operator
    * okay to throw
    * ideal if no state change, no resource leak
    * swap: ideally O(1), no-throw

## deferred
* the big 5, move construction, move assignment
* initialization with `{}`
* inheritance
    * `virtual ~Widget() = default; virtual ~Widget() {}`
* array initialization syntax
* and then there's question about how data on different segment is initialized
    * are global var are value initialized? 
    * are vars in dynamic section value initialized? 
* object lifetime
    * compare this to name scope
    * compare this to storage duration
