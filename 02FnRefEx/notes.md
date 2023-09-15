Workflow
========
* read notes, think about fundamental questions
* read notes & slides, note down interesting pieces
* demystification, til, take-away

Section 1.0: Objects
====================
## Types
* basic types
    * char 1, bool 1, 
    * short 2, int 4, long 8, 
    * float 4, double 8, long double 8 
* user defined types
    * std::string 24, std::vector 24
* `sizeof`
    ```cpp
    sizeof(int);
    sizeof(a);
    sizeof(4 + 5);
    ```
    * is an operator
    * is evaluated at compile-time only
    * works on the type of its expression
* remarks
    * the fundamental types above do not have default values

## Object
* "an object is a region of `storage`" - identity: 
    * `type` - behavior; 
    * `value` - state; 
    * `name`/`reference`(alias)
* "every expression is either an `lvalue` or `rvalue`"
    * `lvalue`s need to have `storage`; `lvalue`s are `object`s
* objects with basic types
    * in many contexts (e.g. when used as local variables) types like int do not 
    have default values.
    * accessing an uninitialized variable results in undefined behavior
    * C++ is not designed for definite assignment analysis – a compiler 
    technique for ensuring that all variables are assigned to before use. 

Section 1.1: Functions
======================
## Terminology
* signature, return type - signature doesn't include return type
* parameters, arguments - parameters in signature, arguments in callsites
    * args behave as local variables but they're initialized at the call-site
* remarks
    * Functions are not values. You can’t assign a function to a variable. This 
    has to be done through pointers
    * free functions aren't necessarily global. They may be contained with a 
    namespace but not inside a type
    * parameters behave as local variables but they're initialized at call-site

## Control Flow
* switch
    ```cpp
    switch (a) {
        case 1:
            std::cout << a << std::endl;
    }
    ```
    * must take an integral value
    * not necessarily include all scenarios (default not required)

## Default Arguments
* trailing parameters: if a parameter has a default argument, then all of the
parameters to the right must also have default arguments
* default arguments may use expressions (e.g. currentYear()) but these
expressions must not refer to other parameters

## Function Overloading
* The compiler deduces which function to call based on the types of the
arguments at the call-site. This is overload resolution
* If there is more than one 'reasonable choice' at overload resolution, then the
call is ambiguous

Section 2: References
=====================
## stack
* static vs. dynamic typing
    - static: values do not know their own type (polymorphic later). A variable 
    becomes an offset on the stack frame or an address in memory. The code that 
    uses a only uses it in a way that is consistent with its compile time type.
    - dynamic: variables not typed, the values typically are. The values know 
    what operations can be performed on them. At run-time, operations are 
    checked against the type of the value. 

## reference
* reference
    * "references are aliases"
    * aliases may occupy no space (they're not an object)
    * once a reference has been initialized, it semantically is the referent. 
    * sizeof shows that the references truly are alternative names for the 
    original objects. 
* pass by
    * value
        * pass by value makes copies of the args for the function being called
        * to reiterate, args behave as local variables but they're initialized 
        at the call-site
        * number of copies: 1
    * reference
* return by
    * value
    * reference
        * **never** return a reference to a local variable. When the function 
        returns, its stack frame has become invalid.  The reference will be to 
        an object that is no longer valid. **The behavior is undefined** (aka 
        world of pain).

## lvalues and rvalues
* moved to Section 1.0

Section 3: Exceptions
=====================
* communicating failures
    * set a global var, e.g. `errno`
    * return a special bit pattern, e.g. negative return val
    * result code, and move the return val to be an output parameter
    * throwing exceptions

* exeptions
    * throw is typically much more expensive than a regular return
    * when you catch, you should catch by reference.
        * slicing is the reason for always catching by reference
    * you cannot have a finally at all
        * C++ has something better than finally – deterministic destruction. 
    * catch all; throw as-is
        ```cpp
        catch (...) {
            // do something, e.g. log
            throw;
        }
        ```

* asssertions
    * An assertion checks a condition and causes the program to abort. It 
    provides no possibility for recovery
    * If an assertion is violated it always indicates a programming error
    * Aborting may seem harsh, but it is often better than a program continuing 
    in an invalid state
    * **Conditions Must Not Have Side Effects**
        * Assertion conditions can have side effects. This is risky because 
        the behavior depends on whether assertions are on or off. This is 
        usually not intentional, and can lead to bugs where the production 
        version differs from the version tested during development.
        * Generally, you'll want to perform the op with the side effect before 
        the assertion, and then assert that the result was what you expected

Side Notes
==========
* can we have a vector of references
    - The component type of containers like vectors must be assignable. 
    References are not assignable (you can only initialize them once when they 
    are declared, and you cannot make them reference something else later). 
    Other non-assignable types are also not allowed as components of containers, 
    e.g. vector<const int> is not allowed.

Demystification:Exception
=========================
## a look at the stack
* running example
    ```cpp
    void a() {
        throw 10;
    }

    void b() {
        try {
            a();
        } catch (int& e) {
            e = 20;
            throw;
        }
    }
    ```
* throw
    ```cpp
    throw 10;                   // movl    $4, %edi
                                // call    __cxa_allocate_exception
                                // movl    $10, (%rax)
                                // movl    $0, %edx
                                // movl    $_ZTIi, %esi
                                // movq    %rax, %rdi
                                // call    __cxa_throw
    ```
    * allocate memory for the exception
    * copy the value over to the allocated space
    * "return"/throw
* try-catch
    ```cpp
    try {
        a();                    // call    a()
                                // jmp     .L8

    } catch (                   // cmpq    $1, %rdx
                                // je      .L4
                                // movq    %rax, %rdi
                                // call    _Unwind_Resume
        int& e) {       // .L4
        throw;
    }
    ```
    * try
        * if no exception, jump directly to the stuff after catch
    * catch
        * if exception wasn't caught: continue to unwind
        * otherwise, start handing exception
* catch by reference vs catch by value
    ```cpp
    catch(int& e) {     // .L4:                                    
                                // movq    %rax, %rdi              
                                // call    __cxa_begin_catch       
                                // movq    %rax, -24(%rbp)         

        e = 20;                 // movq    -24(%rbp), %rax
                                // movl    $20, (%rax)
                                // movl    $4, %edi
    }
    catch(int e) {      // .L4:
                                // movq    %rax, %rdi
                                // call    __cxa_begin_catch
                                // movl    (%rax), %eax
                                // movl    %eax, -20(%rbp)

        e = 20;                 // movl    $20, -20(%rbp)
    }
    ```
    * catch by reference: modifies the original exception object
    * catch by value: make a copy of the exception object
    * another motivating example
        ```cpp
        void a() { throw 10; }
        void b() {
            try { a(); } 
            catch (int e) { e = 20; throw; } // throws the original value 10
            catch (int& e) { e = 20; throw; } // throws the modified value 20
        }
        ```
* `throw;` vs. `throw e;`
    ```cpp
    catch(int& e) {              
        throw;                  // call    __cxa_rethrow

        throw e;                // movl    $4, %edi
                                // call    __cxa_allocate_exception
                                // movq    -24(%rbp), %rdx
                                // movl    (%rdx), %edx
                                // movl    %edx, (%rax)
                                // movl    $0, %edx
                                // movl    $_ZTIi, %esi
                                // movq    %rax, %rdi
                                // call    __cxa_throw
    }
    ```
    * `throw`: throw the original exception object
    * `throw e`: allocate space for a new exception object
    * the above is true regardless of catch by value/reference

## throw by value, catch by (const) reference
* throw by value
    * There's no such thing as "throwing by reference". It is simply 
    impossible. Every time you try to "throw a reference", a copy of the 
    referenced object is actually thrown.
    * Avoid throwing a pointer, because if you throw a pointer, you need to 
    deal with memory management issues: You can’t throw a pointer to a 
    stack-allocated value because the stack will be unwound before the 
    pointer reaches the call site. You could throw a pointer to dynamically 
    allocated memory (if the error you’re reporting isn’t “out of memory” to
    begin with), but you’ve put the burden on the catch site to deallocate 
    the memory. If you feel you really must throw a pointer, consider 
    throwing a value-like smart pointer such as a `shared_ptr<T>` instead 
    of a plain `T*`
* catch by reference (preferably const reference)
    * You need:
        * a reference so you can access the exception polymorphically
        * a const to increase performance, and tell the compiler you're not 
        going to modify the object
    * The latter is not as much important as the former, but the only real 
    reason to drop const would be to signal that you want to do changes to 
    the exception (usually useful only if you want to re-throw it with added
    context into a higher level).

## best practice: my mental model
* each function: return type, exception types
* for each exception caller should decide whether to
    * catch
    * throw
        * let it propagate
        * handle and throw
    * but don't swallow
* when we look at the entire call chain
    * we'd hope it stops somewhere: it's for communicating failures
    * otherwise use assert
        * "Assert liberally to document internal assumptions and invariants"

## best practices
* exception is better than return code - colavita
    * For exceptions, that means that you should let the exception bubble up. 
    Then, have a try / catch block at the place where you’ll handle it. 
    * For error values, you’ll have to manually return them until they reach 
    the correct place in the call stack.

* handle errors at the first appropriate place
    * Sometimes, you can’t handle errors immediately. You might need to 
    propagate them to higher level code.
    * e.g., You have a function that searches the file system for a file. If 
    it finds it, it reads its contents. Otherwise, it throws an exception. How 
    should the code handle this exception? The file system code doesn’t know. 
    Instead, the exception should be handled at a more appropriate place. The 
    place that can decide what to do about it. 
    * Having said that, you should handle errors at the first appropriate place. 

* don't handle unless you know what to do with it
    * only use them when you expect an exception to occur and you need to handle
    it in a specific way.
    * If you think about it, then there's most always just one place which is 
    good to handle an occuring exception. 
    * My basic rule is : Unless you can fix the problem which caused the 
    exception, do not catch it, let it bubble up to a level where it can be 
    dealt with.

* be careful of how much code you put in your try blocks
    * It’s considered best practice to put as little code as possible in each 
    try / catch block. This means that you may need multiple try / catch blocks,
    instead of just one.
    * benefits
        * it’s easy to see which code raises which exceptions (and which code 
        doesn’t raise exceptions)
        * it "separates concerns" more clearly. Each try / catch block is a 
        separate piece of functionality. This makes it easier to refactor it 
        into a separate function.
        * it prevents the accidental swallowing of exceptions. This happens if 
        you put some code in try without thinking. That code might throw an 
        exception that you weren’t prepared to handle. However, it will be 
        caught in catch (and potentially handled incorrectly). Then, the program
        will continue executing, potentially producing the wrong result.
        * It’s also a solution for when certain lines of code can throw the same
        type of exception, but each case needs to be handled differently.
    * drawbacks
        * make the code more verbose

* capture-all try-catch
    * If you're going to handle random exceptions, handle them in only one place
      the very top of the application, for the purposes of:
        * presenting a friendly message to the user, and 
        * saving the diagnostics.
    * For everything else, you want the most immediate, location-specific crash 
    possible, so that you catch these things as early as possible

Demystification: Pass & Return
==============================
* value
    * pass by value 
        * conceptually: make a copy; think of it as `int a = _a;`
        * in reality: 
            * it can be loaded to registers;
            * if big, it can be copied to someplace on the caller's stack frame
    * return by value 
        * conceptually: 
            * local var in callee is local
            * return value is a temporary
            * if the caller assign the return value to a variable, then call
            copy/assign 
        * in reality: 
            * This is related to the common Return Value Optimization for C++ 
            that can avoid doing an actual copy operation in the case you have 
            described.
            * The end result is (or should be) the same as if a copy were done, 
            but you should be aware of the optimization. 
* lvalue reference
    * pass by reference
    * return by reference
         * **never** return a reference to a local variable.

TIL
===
## object 
* `sizeof`
    * is an operator
    * is evaluated at compile-time only
    * works on the type of its expression
* accessing an uninitialized variable results in undefined behavior

## function
* switch
    * not necessarily include all scenarios (default not required)
* if a parameter has a default argument, then all of the parameters to the right
must also have default arguments

## reference
* static vs. dynamic typing

## exception
* communicating failures
    * set a global var, e.g. `errno`
    * return a special bit pattern, e.g. negative return val
    * result code, and move the return val to be an output parameter
    * throwing exceptions
* throw is typically much more expensive than a regular return
* you cannot have a finally at all
* catch all
    ```cpp
    catch (...) {
        // do something, e.g. log
        throw;
    }

## side notes
* we cannot have a vector of references

Take-away
=========
## object
* "an object is a region of `storage`" - identity: 
    * `type` - behavior; 
    * `value` - state; 
    * `name`/`reference`(alias)
* "every expression is either an `lvalue` or `rvalue`"
    * `lvalue`s need to have `storage`; `lvalue`s are `object`s

## pass & return by value
* pass: make a copy `int a = _a`
* return: 
    * local of callee is local; return value is temporary; 
    * copy when caller use the return value to create a local
    * Return Value Optimization (RVO)

## exception
* stack
    * throw allocates memory for the exception object
    * catch by reference to avoid slicing & copying
    * no such thing as throw by reference; avoid throw by pointer
    * `throw;` throws the original exception object
* best practice
    * exception is for communicating failures; it's better than error code
    * handle errors at the first appropriate place
    * don't handle unless you know what to do with it
    * it’s considered best practice to put as little code as possible
    * if you're going to handle random exceptions, handle them in only one place
      the very top of the application

## **deferred**
* obj, pass & ret is also worth revisiting
* rvalue referrence; return by value revisit
* function overloading
* where mustn't we throw exception, e.g. destructor
