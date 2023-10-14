Take-Away
=========
## storage duration: automatic, static, dynamic
| duration      | kind          | lifetime         | name scope      
|---------------|---------------|------------------|----------------
| stack/auto    | local var     | end of {}        | {}
|               | parameter     | end of {}        | {}
|               | temporary*    | end of expr      | no name
| data/static   | global        | end of prog      | global
|               | class static  | end of prog      | class
|               | local static* | end of prog      | {}
| dynamic       |               | user controlled  | no name

* note to self
    * temporary has storage duration, in the sense that the destructor is called
    * local static seems to have no name outside of the {}, even though it has 
    storage

## static
* variable types: global, class static, local static
    ```cpp
    // local static
    void demo() {
        static int count;
        ++count;
    }
    ```
* initialization in data section
    * zero initialization (cppreference, partial)
        * for every named variable with static storage duration that is not 
        subject to constant initialization, before any other initialization
    * constant-initialization (cppreference, partial)
        * if a static variable is constant-initialized, constant initialization 
        is performed instead of zero initialization before all other 
        initializations
* dynamic initialization
    * they can be interleaved across TUs
    * there’s not even a guarantee that dynamic initialization will have
    happened before main is called
    * ignores dependency
    * note to self
        * load code to text
        * load zero/constant to data
        * perform dynamic initialization on data, no dependency considered
        * main could start running before dynamic initialization finishes
    
## dynamic
* syntax
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
* failures
    * exception
    * memory leak
    * undefined behavior
        |                           | delete        | access         
        |---------------------------|---------------|---------------|
        | nullptr                   | no-op         | undefined
        | stack/data                | undefined     | ok
        | heap, no matching new/[]  | undefined     | undefined
        | deleted                   | undefined     | undefined
* void*; 0, NULL, nullptr
    * `void* <- int*`
    * 0, NULL, nullptr: literals!
        ```cpp
        int* p = 0;
        int* p = NULL; // could be 0, nullptr
        int* p = nullptr; 
        ```
        * prefer nullptr to 0 when using the literals
            ```cpp
            demo(0); demo(nullptr);             // function resolution
            if (res = 0); if (res = nullptr)    // code clarity
            ```
* concept of name
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
* paradigm
    * pointer vs reference
    * heap vs stack

## array 
* size
    * 0 is not allowed in concept, allowed in practice
    * variable-length arrays
* aggregate initialization
    * syntax
        ```cpp
        int a[5] = {1, 2, 3};
        int a[5]{1, 2, 3};

        Demo d = {1, 2, 3.14};
        Demo d{1, 2, 3.14};
        ```
    * effect
        * explicitly initialized elements: copy-initialized
        * implicitly initialized elements: value-initialized

## initialization summary
* syntax
    |type       |auto/static* |dynamic     |array(a./s.) |array(d.)     |member 
    |-----------|-------------|------------|-------------|--------------|-------
    |default    |int a;       |new int;    |int a[5];    |new int[5];   |empty  
    |direct     |int a(1);    |new int(1); |-            |-             |a(1),  
    |copy       |int a = _a;* |-           |-            |-             |-      
    |value      |int();       |new int();  |int a[5]();  |new int[5](); |a(),   
* additional syntax
    * automatic
        * param: `demo(a)`
        * return: `return a;`
        * (both copy initialization, required to be non-explicit)
    * static
        * class static, local static: `static int a;`
        * (`static` optional for global)
     * dynamic & member
        * value initialization is in-place (no additional copy init involved)
        * no syntax for copy initialization
* effects
    |type       |basic      |udts             
    |-----------|-----------|-------------------|
    |default    |no-op      |default            
    |direct     |as defined |direct, copy     
    |copy       |as defined |copy (non-explicit)
    |value      |zero-init  |default, "value"
 
Shortened List
==============
## storage duration: automatic, static, dynamic
* storage duration vs name scope

## static
* variable types: global, class static, local static
* initialization in data section
    * zero initialization
    * constant initialization
* dynamic initialization

## dynamic
* syntax
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
* failures
    * exception
    * memory leak
    * undefined behavior
* void*; 0, NULL, nullptr
    * `void* <- int*`
    * 0, NULL, nullptr: literals!
* concept of name
* paradigm
    * pointer vs reference
    * heap vs stack

## array
* size
    * 0 is not allowed in concept, allowed in practice
    * variable-length arrays
* aggregate initialization

## initialization summary
* syntax: automatic, static, dynamic, array, member
* effects