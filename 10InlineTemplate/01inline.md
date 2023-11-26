Section 01 Inline
================================================================================
* the cost of encapsulation
    * encapsulation is achieved through functions that provide controlled access
    * function call has a cost

* function call cost
    * requires preparation at call site
    * requires housekeeping in function
    * requires transfer to function location
    * requires clean up at function end

* could we copy function body to call site?
    * there is no need to jump to another page so there is no risk of thrashing
    * the compiler does this with the source code, not modifying machine code.

* function inlining
    * to inline: compiler must be able to see function definition at call-site
    * move to header: allows all call-sites to see definition
    * violates ODR: function definition may only appear in one TU
    * C++ offers modified ODR - inline 
    * (separate compilation: compilation of a very large program would be 
    prohibitively expensive if all of the code needed to be inspected during
    each compilation)

* inline functions
    * the definition is allowed to appear in multiple translation units, as long
      as it is the same definition
        * By ‘same definition’ the standard expects the same set of lexical 
        tokens
    * the definition is still not permitted to appear more than once in a single
    translation unit though.
    * member functions can be made implicitly inline by including their 
    definition within the class definition
    * all functions can be made inline explicitly with the inline keyword. 
    * the compiler needs to be able to see the definition’s body in order to 
    take advantage of it. Therefore, inline functions are typically found 
    within header files

* inline functions (cont.)
    * The compiler is never required to perform optimizations. You’re just 
    making it (more) possible
    * There is a trade-off between run-time performance and compile-time 
    performance. Using inline probably causes slower compilation, and probably 
    increases the number of files that will need to be recompiled if the 
    implementation changes.
    * Copying the body to the call site may actually hurt run-time performance. 
    This could be because the object files are now larger. More code means that 
    less of your program fits within the processor’s cache.
    * So far we have seen strong symbols with the linker. It is possible to use 
    weak symbols. Having multiple definitions of weak symbols is not considered 
    an error. This typically is needed when an inline function’s address is 
    taken because it must be the same address across all translation units

* header only libraries
    * Inline functions allow header only libraries. e.g. boost
    * Because the library is provided as a header you know that it is being 
    compiled with the same toolset as the project that uses it. This avoids 
    issues with the application binary interface 
    * Because the source code must be distributed a header only library is not 
    well suited to proprietary code.

## questions
* you've forgotten all about function call, again :)
    * revisit function call cost
    * why was there risk of thrasing? 
* the compiler does this with the source code? 
* address of inline functions? 

questions
================================================================================
* ultimate goal 
    * is to be able to write one by yourself
    * and also to navigate thru code quickly
* how is template different from macro
* when is templated function generated? 
    * running? i suspect not
    * linking? i suspect not
    * comipling
        * then how to avoid 2 generated in the same TU
        * in diff TU
* what's the ODR rule for inline again? 