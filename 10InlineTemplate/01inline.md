Section 01: Inline
================================================================================
## motivatin example
* design choice: encapsulation vs performance
    ```cpp
    class CmdyFuture {
    public:
        int contractSize() const;
    private:
        int _contractSize;
    }

    class CmdyFuture {
    public:
        int contractSize;
    }
    ```

* function call cost
    ```cpp
    CmdyFuture cf;
    d.contractSize();               // leaq     -80(%rbp), %rax
                                    // movq     %rax, %rdi
                                    // call     contractSize

    CmdyFuture::contractSize        // pushq    %rbp
                                    // movq     %rsp, %rbp
                                    // movq     %rdi, -8(%rbp)
                                    // movq     -8(%rbp), %rax
                                    // movl     24(%rax), %eax
                                    // leave
                                    // ret
    ```
    * caller: passing `this` pointer
    * control is transferred to the `contractSize` function. this may be located 
    on a different page of memory. if virtual memory currently has that paged 
    out, then this could be a really expensive operation
    * callee: setting up the stack frame

* idea: copy function body to call site
    ```cpp
    CmdyFuture cf;
    cf.contractSize();              // leaq     -80(%rbp), %rax         
                                    // movq     %rax, %rdi          x
                                    // call     contractSize        x

    CmdyFuture::contractSize        // pushq    %rbp                x
                                    // movq     %rsp, %rbp          x
                                    // movq     %rdi, -8(%rbp)      x
                                    // movq     -8(%rbp), %rax      x
                                    // movl     24(%rax), %eax
                                    // leave                        x
                                    // ret                          x

    cf.contractSize();              // leaq     -80(%rbp), %rax         
                                    // movl     24(%rax), %eax

    cf.contractSize();              // movl     -56(%rbp), %eax
    ```
    * If the compiler knows what is inside the function’s body when it compiles 
    the function call, then it can replace the function call with the function’s 
    body
    * There is no need to jump to another page so there is no risk of thrashing
    * The compiler does this with the source code, not modifying machine code

## `inline` keyword
* in order to inline function
    * => the compiler must be able to see the function’s definition
    * => we could selectively make our function definition visible by moving it 
    into the header file
    * => multiple TUs have the same definition
    * => `inline` functions are subject to different constraints

* modified ODR for `inline` 
    * The definition is allowed to appear in multiple translation units, as long 
    as it is the same definition 
    * The definition is still not permitted to appear more than once in a single 
    translation unit though.

* strong symbols and weak symbols (linker)
    * Having multiple definitions of weak symbols is not considered an error
    * This typically is needed when an inline function’s address is taken 
    because it must be the same address across all translation units

* to `inline` a function
    * All functions can be made inline explicitly with the `inline` keyword
    * Member functions can be made implicitly inline by including their 
    definition within the class definition.

## optimization
* `inline` keyword and optimization
    * The compiler is never required to perform optimizations. You’re just 
    making it (more) possible. 
    * The compiler may inline functions that are not marked inline, but it’s 
    harder , since the compiler often doesn’t have access to a non-inline 
    function body, and without the inline specifier, the compiler has to avoid 
    multiple definitions

* compile-time performance when using inline 
    * probably causes slower compilation, and 
    * probably increases the number of files that will need to be recompiled 
    if the implementation changes.
    
* run-time perfoamance when using inline
    * may actually hurt run-time performance
    * This could be because the object files are now larger. More code means 
    that less of your program fits within the processor’s cache

## header only libraries
* Inline functions allow header only libraries. 
    * e.g. Boost
* pros
    * Because the library is provided as a header you know that it is being 
    compiled with the same toolset as the project that uses it
* cons
    * Because the source code must be distributed a header only library is not 
    well suited to proprietary code.