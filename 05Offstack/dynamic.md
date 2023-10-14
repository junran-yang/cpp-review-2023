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
*p; // lvalue
&a; // rvalue

int* p = new int[10];
delete[] p;
p[3]; *(p + 3);

```
* new: malloc + ctor
* delete: dtor + free

## failures
* exception
    * memory allocation, constructor
* memory leak
    * new/new[] without delete/delete[]
* undefined behavior
    * `delete`
        * invalid pointer
            * pointer to stack/data
            * pointer without matching new/[] (incl. del in the middle of arr)
            * deleted pointer
        * note: delete nullptr is no-op
    * access
        * invalid pointer
            * note: pointer without new maybe ok
            * deleted pointer
        * nullptr
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

## array size
* Is 0 a valid array size? 
    - In Standard C and C++, zero-size array is not allowed.
    - If you're using g++, compile it with -pedantic option
        ```
        warning: zero size arrays are an extension [-Wzero-length-array]
        ```

* variable-length arrays
    - (cppreference) If expression is not an integer constant expression, the 
    declarator is for an array of variable size.
    - Each time the flow of control passes over the declaration, expression is 
    evaluated (and it must always evaluate to a value greater than zero), and 
    the array is allocated (correspondingly, lifetime of a VLA ends when the 
    declaration goes out of scope). The size of each VLA instance does not 
    change during its lifetime, but on another pass over the same code, it may 
    be allocated with a different size.
    ```cpp
    int a[i] = {};  // error: variable-sized object may not be initialized
    int a[i];       // ok; default initialized
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

## paradigm
* pointer vs. reference
    - So the rule of thumb is to use pointers only if there is no other choice.
    - For example, returning a pointer to an object is a valid option when the 
    function can return nullptr in some cases and it is assumed it will. That 
    said, a better option would be to use something similar to std::optional
    - (cppreference: There are no optional references)
    - Another example is to use pointers to raw memory for specific memory 
    manipulations. That should be hidden and localized in very narrow parts of 
    the code, to help limit the dangerous parts of the whole code base.
* heap vs. stack
    - Use the stack when your variable will not be used after the current 
    function returns. Use the heap when the data in the variable is needed 
    beyond the lifetime of the current function.
    - note to self: e.g. a function creates an object and returns pointer to it
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

## remaining questions
* if you delete a pointer of different type, which ctor will be called?