
Section 01 Static
=================
* automatic
    * local variables, temporaries, function parameters, function return values
* static
    * global variables, class static variables, local static variables
    * local static variable
        * has static storage duration – it lives in the data segment
        * scoped within the block - only the function knows the name of the var
        * initialized the first time the initialization is encountered - not the
        start of the program
        * **shows that an obj’s lifetime is separate to the var name’s scope**
        * often superior to a class static because its scope is more restrictive
* summary
    |location/duration |kind           |lifetime
    |------------------|---------------|---------------------------------------|
    |stack/auto        |auto variable  |initialization - end of block
    |                  |parameter      |function call  - function end
    |                  |temporary      |initialization - end of full expression
    |heap/dynamic      |               |whenever       - whenever
    |data/static       |local static   |initialization - program end
    |                  |class static   |program start  - program end
    |                  |global         |program start  - program end
    * instance variables has the same lifetime as containing object
    * the storage duration dictates the location
* static initialization order
    * dynamic initialization
        * requires code to be executed
        * not zero initialization
        * not initialization from compile time constants
    * local statics
        * initialized when first encountered
    * global variables, class static variables
        * within a single tu
            * guaranteed to be dynamically initialized in definition order
        * across tus
            * they can be interleaved
        * there’s not even a guarantee that dynamic initialization will have
        happened before main is called.
        * the only guarantee is that before actively using an object or function
        from a TU, initialization will be started.

## raw questions
* dynamic initialization
    * not compile time, not runtime, but when loading the program, right?
        - can be after main
    * why is it not considered as part of execution?
        - not part of main
    * why don't we talk about dynamic initialization for other storage duration
        - because they all happen after main for sure
    * why does static initialization order matter
        - dependency
    * how do they call functions?
        - just call it
    * can they call functions that involve static local variables?
        - yes
    * can you device a scenario where 2 global variable depends on each other
      in their initialization?
        - yes, and no loop will be created, because one would be assumed to be 0
    * is built-in type zero initialized in data section
        - yes
    * what is the syntax for default/direct/copy/value init global/class static?
        - same as before

Demystification
===============
## storage duration vs. initialization kinds (default, direct, copy, value) 
* default initialization (cppreference)
    * situations
        * automatic, static: declared with no initializer
        * dynamic: `new int;`
        * member variable / base class: not mentioned in initilizer list
    * effects
        * primitive types: no initialization
        * user defined types: default constructor
        * array: every element is default-initialized
* zero initialization (cppreference, partial)
    * for every named variable with static storage duration that is not subject 
    to constant initialization, before any other initialization
* constant-initialization (cppreference, partial)
    * if a static variable is constant-initialized, constant initialization is 
    performed instead of zero initialization before all other initializations

 type       |auto/static* |dynamic     |array(a./s.) |array(d.)     |member |basic      |udts             
|-----------|-------------|------------|-------------|--------------|-------|-----------|-------------------|
 default    |int a;       |new int;    |int a[5];    |new int[5];   |empty  |no-op      |default            
 direct     |int a(1);    |new int(1); |-            |-             |a(1),  |as defined |direct, copy     
 copy       |int a = _a;* |-           |-            |-             |-      |as defined |copy (non-explicit)
 value      |int();       |new int();  |int a[5]();  |new int[5](); |a(),   |zero-init  |default, "value"
 list/      |             |            |int a[5] = {1, 2, 3}; |     |       |           |
 aggregate  |             |            |int a[5]{1, 2, 3}; |        |       |           |

* syntax
    * additional syntax for automatic
        * param: `demo(a)`
        * return: `return a;`
        * (both copy initialization, required to be non-explicit)
    * additional syntax for static
        * class static, local static: `static int a;`
        * (`static` optional for global)
    * dynamic & member
        * value initialization is in-place (no additional copy init involved)
        * no syntax for copy initialization
* behavior
    * static is the only thing that differs
        * zero-initialzation + other initialization
        * unless when there's a handy way of doing constant-initialization
    * member vs containing object
        * the same storage duration 
        * independent initialization method
        * think basic types as leaf nodes, udts as non-leaf nodes
    * array
        * just initialize the components separately all at once

* note to self
    * stick to initialization type - syntax - behavior
    * the only exception is static: zero/constant initialization performed 
    before the regular initialization
    * initialization is a tree: basic type as the leaf, udts as non-leaf

## dynamic initialization
* Why are global variables always initialized to '0', but not local variables?
    * **take the answer with a grain of salt**
    * static variables are initialized at compile-time, since their address is 
    known and fixed. Initializing them to 0 does not incur a runtime cost
    * automatic variables can have different addresses for different calls and 
    would have to be initialized at runtime each time the function is called, 
    incurring a runtime cost that may not be needed. If you do need that 
    initialization, then request it.

* Does global initialization order ignores dependencies?
    * yes
    ```cpp
    // file a.cc
    extern int b;
    int a = initA();
    int initA() { return b + 1; }

    // file b.cc
    extern int a;
    int b = initB();
    int initB() { return a + 1; }

    // possible results
    // a = 1; b = 2;
    // a = 2; b = 1;
    ```

* note to self
    * load code to text
    * load zero/constant to data
    * perform dynamic initialization on data, no dependency considered
    * main could start running before dynamic initialization finishes

## storage duration vs. name scope
* how object lifetime relates to name scope
    * not related
    |           |global  |class s. |local s. |local |param |return  |dynamic
    |-----------|--------|---------|---------|------|------|--------|--------|
    |life time  |program |program  |program  |block |block |expr    |as spec
    |name scope |global  |class    |block    |block |block |no name |no name
    * local s. shows that an obj’s lifetime is separate to the var name’s scope

Section 02|03|04 Dynamic
========================
## single object
* new
    * finds memory
        * if new cannot find memory for object, then an exception will be thrown
    * initializes the object by calling the constructor
    * returns a pointer to the object
* delete
    * calls the destructor
    * returns the memory to the memory allocator
        * delete a pointer to stack memory
            ```cpp
            int a = 20; delete &a;
            // Compiler
            //     warning: 'void operator delete(void*, std::size_t)' called on
            //     unallocated object 'a' [-Wfree-nonheap-object]
            // Program
            //     Program terminated with signal: SIGSEGV
            ```
    * designed to work with pointer
        * expects an rvalue; doesn't make changes even if an lvalue supplied
* member access
    * `*` can be applied to a pointer and yield an lvalue for the object
    * accessing an area of memory after the obj is deleted is undefined behavior
    * objects of dynamic storage duration do not have names
* pointer
    * are both an address and an expectation of what can be found at the address
    * all have the same size
    * default initialization
        * static storage: 0
        * automatic storage: not initialized
    * `0`, `NULL`
        * this value is normally set up in a special way so that accessing the
        object at the end of the pointer will cause an immediate failure
        * if you delete a 0 (NULL) pointer it’s a no-op. You don’t need to check
        whether it is null first.
* dynamic vs automatic
    * unless there is a good reason, favor creating objects on the stack
    * the fundamental reason being that you need to control its lifetime
    * another reason is that the object you want to create is large
* take-away
    ```
    new                             delete
    | finds memory (exception)      ^ release memory (exception)
    | calls constructor             | calls destructor
    v returns pointer               | takes pointer
    ```
    * object
        * no name
        * `*pointer` yields the lvalue of the object
    * pointer
        * delete can take an rvalue pointer

## array
* an array is a block of memory for zero or more contiguous elements
    * 0 is a valid size
    * negative sizes are invalid
    * the size does not need to be known at compile time
* new[]
    * new[] allocates enough memory to store the elements contiguously
        * if new[] cannot find the memory then an exception is thrown
    * initializes all of the elements in the array
        * calls the constructors in sequence (first element first)
        * initialization
            * default initializes all the elements in the array
                * requires the type to have a default constructor
            * they can be value initialized by `new[10]()`
            * you cannot pass arguments
        * exception
            * if thrown during constructing of an element, then the previously
            constructed elements will be destructed, in reverse order
            * the exception will escape the use of new[]
    * returns a pointer to the first element
        * its type is not distinct from a pointer to a single element
* delete[]
    * destructs all of the objects in the array, in reverse order
    * automatically deallocates the memory that had been allocated
    * any pointer variables are left with their value
    * vs delete
        * if you call delete instead of delete[] with an array, then the dtors
        will not be called for all of the elements. It is undefined behavior.
        * in a hidden and probably non-portable way, delete[] knows how to find
        the size of the array so that it can call the correct number of dtors
* element access
    * [] yields an lvalue – the element itself
    * There is no bounds checking, and offsets can be negative.
* pointer
    * `&a` yields a pointer value. It’s an rvalue (temporary)
    * const syntax
    * pointer arithmetic
        * a new address is calculated using three inputs: the offset, the
        address held in the pointer, and `sizeof(object)`
        * You can subtract an offset from a pointer. Subtraction is not
        commutative, and therefore you cannot subtract a pointer from an int.
            ```
            T* + int => T*
            int + T* => T*
            T* - int => T*
            ```
        * `p[n] = *(p + n)`
    * `void*`
        * void* can point to anything.
        * There is no expectation of what to find at the address.
        * Therefore, you are unable to dereference a void*.
        * You cannot perform pointer arithmetic with a void*.
        * In order to use the pointee, you will need to cast from the void* into
        some other pointer type.

## misc
* pointer vs references
    * If a reference meets your semantic needs, then use a reference.
    * Otherwise use a pointer.
* new & delete
    * new without delete => memory leak
    * delete without new => undefined behavior
    * double delete => undefined behavior
    * new[] with delete => undefined behavior
    * new with delete[] => undefined behavior
    * deleting invalid pointer => undefined behavior
    * deleting something on stack => undefined behavior

demystification
===============
## syntax
    ```cpp
    int* p = new int;
    delete p;
    *p; // lvalue;
    &a; // rvalue

    int* p = new int[10];
    delete[] p;
    p[3]; *(p + 3);

    ```
    * new: malloc + ctor
    * delete: dtor + free

## failures
* by operation
    * new
        * exception: emory allocation, constructor
    * delete
        * ok: null pointer: no-op
        * undefined: invalid pointer, e.g. pointer to stack, un`new`'d pointer,
        deleted pointer
    * access
        * undefined: null pointer(likely immediate failure)
        * undefined: invalid pointer, e.g. deleted pointer
* new & delete failure summary
    * new/new[]
        * no delete/no delete[] - memory leak
    * delete/delete[]
        * no matching new/new[] - undefined, e.g.
            * new/new[] with delete[]/delete
            * pointer to stack
        * double delete/delete[] - undefined
* why is dereferencing a null pointer undefined
    - According to ISO C++, dereferencing a null pointer is undefined behavior
    - Defining consistent behavior for dereferencing a NULL pointer would
    require the compiler to check for NULL pointers before each dereference on
    most CPU architectures. This is an unacceptable burden for a language that
    is designed for speed.
    - It also only fixes a small part of a larger problem - there are many ways
    to have an invalid pointer beyond a NULL pointer.
* can you delete[] in the middle of an array?
    ```cpp
    int* p1 = new int[10];
    int* p2 = p1 + 5;
    delete[] p2;
    // free(): invalid pointer
    // Program terminated with signal: SIGSEGV
    ```

## the concept of name, again
* name "compiled away"
    * my mental model of c++ abstract machine: with name
        ```
            heap ->                           <- stack
                    obj            |a  p
        |----------|----|----------|--|--|--|--|--|
        ```
    * my mental model of running memory
        ```
            heap ->                           <- stack
                    obj            |x  x
        |----------|----|----------|--|--|--|--|--|
        ```
        * slight modification on top of c++ abstract machine
        * names compiled away
* l/rvalue-ness and name
    * static/automatic obj: storage, name
    * dynamic obj:          storage, no name, identified by address
    * temporaries:          no storage, no name

## void*, 0, NULL, nullptr
* void*
    * just a memory address, no expectation
    * unknown size, unusable pointer arithmetic
        ```cpp
        int* p1 = new int[10]; // 0x1cab2b0
        int* p2 = p1 + 1;      // 0x1cab2b4
        void* p3 = p1;         // 0x1cab2b0
        void* p4 = p3 + 1;     // 0x1cab2b1
        ```
    * implicit conversion: T* -> void*, not the other way around
* 0
    ```cpp
    int* a1 = 0;
    int* a2 = 19; // error: cannot initialize a variable of type 'int *' with an rvalue of type 'int'
    ```
* NULL
    - (cppreference) The macro NULL is an implementation-defined null pointer
      constant, which may be
        - an integral constant expression rvalue of integer type that evaluates
        to zero (until C++11)
        - an integer literal with value zero, or a prvalue of type
        std::nullptr_t (since C++11)
    - A null pointer constant may be implicitly converted to any pointer and
    pointer to member type; such conversion results in the null pointer value of
    that type.
* nullptr
    * not an integral type.
    * not a pointer type.
    * `std::nullptr_t`, you can think of it as a pointer of all types.
    * implicit convertion
        * `std::nullptr_t` -> `void*`
        * `std::nullptr_t` -> `T*`
    * advantages
        * neither 0 nor NULL has a pointer type.
        * overloading
            ```cpp
            void demo(int) { std::cout << "int" << std::endl; }
            void demo(bool) { std::cout << "bool" << std::endl; }
            void demo(void *) { std::cout << "void*" << std::endl; }

            demo(0);
            demo(NULL);
            ```
        * code clarity
            ```cpp
            auto res = demo();
            if (res == 0) {}
            ```
        * templates

## when to use pointer
* vs. reference
    - So the rule of thumb is to use pointers only if there is no other choice.
    - For example, returning a pointer to an object is a valid option when the 
    function can return nullptr in some cases and it is assumed it will. That 
    said, a better option would be to use something similar to std::optional
    - (cppreference: There are no optional references)
    - Another example is to use pointers to raw memory for specific memory 
    manipulations. That should be hidden and localized in very narrow parts of 
    the code, to help limit the dangerous parts of the whole code base.
* vs. stack
    - Use the stack when your variable will not be used after the current 
    function returns. Use the heap when the data in the variable is needed 
    beyond the lifetime of the current function.
    - another reason is that the object you want to create is large

## initialization
* types (per cppreference on initialization)
    * default initialization
    * value initialization
    * direct initialization
    * copy initialization
    * list initialization
    * aggregate initialization
    * reference initialization
* other classification
    * zero initialization
    * const initialization
    * array initialization

## aggregate initialization
* syntax
    ```cpp
    int a[5] = {1, 2, 3};
    int a[5]{1, 2, 3};

    Demo d = {1, 2, 3.14};
    Demo d{1, 2, 3.14};
    ```
* aggregate
    * array type
    * class type that (until c++11)
        * no user-declared constructors
        * no private/protected non-static data members
        * no base class
        * no virtual member functions
* effect (until c++11)
    1. reject the ill-formed cases
    2. determine the explicitly initialized elements of the aggregate
    3. Initialize each element of the aggregate
    * For each explicitly initialized element:
        * the element is copy-initialized from the corresponding initializer
          clause of the initializer list
        * If the initializer clause is an expression, implicit conversions are
        allowed as per copy-initialization
        * If the initializer clause is a nested braced-init-list (which is not
        an expression), recursively apply the rule if the corresponding element
        is a subaggregate.
    * For a non-union aggregate, each element that is not an explicitly
      initialized element is initialized as follows:
        * if the element is not a reference, the element is copy-initialized
        from an empty initializer list.
        * Otherwise, the program is ill-formed.

* non-explicitly initialized elements
- cppreference (for C) with Array initialization has a clear example for this:
    ```cpp
    int a[3] = {0}; // valid C and C++ way to zero-out a block-scope array
    int a[3] = {}; // invalid C but valid C++ way to zero-out a block-scope array
    ```
    - All array elements that aren't initialized explicitly are zero-initialized
- http://eel.is/c++draft/dcl.init#general-16.5:
    - The remaining elements are initialized with their default member
    initializers, if any, and otherwise are value-initialized.

 * aggregate initialization
     type          |syntax
    |--------------|-----------------------------------------|
     array         |int a[5] = {1, 2, 3};
     nested array  |int a[3][4] = {{1, 2}, {}, {3, 4, 5, 6}};
     class         |Demo a = {1, 2, 3.14};
     nested class  |Demo d = {1, {2, "hello"}, 3.14};

* if we only use aggregate initialization, do we still need default constructor 
for array?
    - no

## array size
* variable-length arrays
    - (cppreference) If expression is not an integer constant expression, the 
    declarator is for an array of variable size.
    - Each time the flow of control passes over the declaration, expression is 
    evaluated (and it must always evaluate to a value greater than zero), and 
    the array is allocated (correspondingly, lifetime of a VLA ends when the 
    declaration goes out of scope). The size of each VLA instance does not 
    change during its lifetime, but on another pass over the same code, it may 
    be allocated with a different size.

* Is 0 a valid array size? 
    - In Standard C and C++, zero-size array is not allowed.
    - If you're using g++, compile it with -pedantic option
        ```
        warning: zero size arrays are an extension [-Wzero-length-array]
        ```

## remaining questions
* if you delete a pointer of different type, which ctor will be called?

Take-Away
=========
@todo: did my best this time. you're welcome to come back to this any time

## static
* initialization summary
     type       |auto/static* |dynamic     |array(a./s.) |array(d.)     |member |basic      |udts             
    |-----------|-------------|------------|-------------|--------------|-------|-----------|-------------------|
     default    |int a;       |new int;    |int a[5];    |new int[5];   |empty  |no-op      |default            
     direct     |int a(1);    |new int(1); |-            |-             |a(1),  |as defined |direct, copy     
     copy       |int a = _a;* |-           |-            |-             |-      |as defined |copy (non-explicit)
     value      |int();       |new int();  |int a[5]();  |new int[5](); |a(),   |zero-init  |default, "value"
     list/      |             |            |int a[5] = {1, 2, 3}; |     |       |           |
     aggregate  |             |            |int a[5]{1, 2, 3}; |        |       |           |
* dynamic initialization
    * note to self
        * load code to text
        * load zero/constant to data
        * perform dynamic initialization on data, no dependency considered
        * main could start running before dynamic initialization finishes
* storage duration vs name scope
    |           |global  |class s. |local s. |local |param |return  |dynamic
    |-----------|--------|---------|---------|------|------|--------|--------|
    |life time  |program |program  |program  |block |block |expr    |as spec
    |name scope |global  |class    |block    |block |block |no name |no name

## dynamic
* syntax
    ```cpp
    int* p = new int; // exception: memory, ctor; leak: new without delete
    delete p; // undefined: invalid pointer (stack, new[], deleted); ok: nullptr
    *p; // undefined: invalid pointer, nullptr

    int* p = new int[10];
    delete[] p;
    p[3]; *(p + 3);
    ```
* void*, 0, NULL, nullptr
    ```cpp
    void* p; <- int* p; <- nullptr_t p;
    ```
    * 0, NULL, nullptr are all literals
* name
    * stack - storage, name
    * heap - storage, no name
    * temporary - no storage, no name
* when to use pointer
    * vs reference (not necessarily pointer to heap)
        * nullptr
        * raw memory management
    * vs stack
        * outlive current scope
        * large

## array
* initialization
    * `Demo d[5] = {Demo(0), Demo(1)};` - direct/default followed by copy init
    * aggre
* array size
    * can be variable length
    * 0 is not valid