Section 02 Function Templates
================================================================================
## non-template function
```cpp
bool allDiffer(int a, int b, int c) {
    return !(a == b || a == c || b == c);
}

allDiffer(3, 4, 5);         // true
allDiffer(8.1, 8.2, 8.3);   // false
```
* we could overload the function for each type, but
    * number of types is unbounded if we include UDTs
    * feels wasteful - the implementation is always the same

## template function
```cpp
template<class T>
bool allDiffer(T a, T b, T c) {
    return !(a == b || a == c || b == c);
}
```
* template world (note to self: name lookup)
    * function templates live in the template world
    * when preceded by the `template` keyword, the item following the angle 
    brackets is being declared or defined in the template world
* sanity check (note to self: syntactical analysis)
    * when the compiler encounters a template function it will perform a sanity
    check, a first stage of compilation where it looks for syntactical errors 
    * whether the function is correct can only be determined once the template 
    arguments have been provided for template parameters
* `T` is a **type** __template parameter__. it is a **type** __variable__
* syntax
    * type parameters are introduced within the angle brackets
    * `class` keyword
        * the template parameter being declared is a type template parameter
        * a little misleading - `T` can be any type, not just class types
        * all else equal, more concise is better
    * `typename`
        * there is a location where only typename may be used, and we are 
        reserving its use for then
* duck typing
    * T can be any type - any type that supports copy construction and equality
    * There is no interface that it must implement, or common base class
    * It just needs to walk like a duck and quack like a duck to be considered a 
    duck.

## calling template function
```cpp
allDiffer<int>(8.1, 8.2, 8.3);      // false
allDiffer<double>(8.1, 8.2, 8.3);   // true
```
* syntax
    * it’s the `<>` again, that open the door to the template world
    * template arguments
        * between the `<>`
        * Like function arguments that fill-in function parameters, template 
        arguments fill-in template parameters
* (note to self: ir generation)
    * The compiler will ensure that there is a version of allDiffer that can be 
    called with three ints, and for three doubles when T = double.

## template instantiation
* like a class is a blueprint for objects, a function template is a blueprint 
for functions
* (note to self: ir generation)
    * when does instantiation happen
        * the compiler cannot know which types to compile allDiffer for until it 
        sees us using `allDiffer` with a specific `T`
        * the compiler revisits the function template and instantiates it for 
        those types
    * ODR
        * instantiation occurs once for each unique combination of template 
        parameters per translation unit
        * function templates follow the same ODR rules as inline functions, so 
        there is no problem with multiple definitions
    * inline
        * Function templates are not implicitly inlined so the compiler might 
        be less inclined to optimize them. 
        * You can make them inline as well
    * compile - link
        * the compiler must be able to see the definition of a template function 
        in order to perform instantiation. 
        * for this reason, function templates are often in header files 
        * it’s not an error to call a function template without the definition 
          being visible, as long as a definition becomes available to the linker
            * (note to self: here this TU won't instantiate the template)
    
## type deduction
```cpp
allDiffer<>(4, 5, 6);
```
* if n template parameters are provided, then they are matched against the first
n template parameters of the function template. Type deduction is performed for 
the remaining parameters.
* We’re still using the angle brackets to make sure that the compiler looks 
within the template world

## template & non-template function resolution
```cpp
bool allDiffer(int a, int b, int c);
template<class T>
bool allDiffer(T a, T b, T c);

allDiffer(4, 4.3, 3.7); 
```
* calling a function without the angle brackets will allow name lookup in both 
the template world and non-template world
* steps 
    * if a function template is found that is suitable, then type deduction is 
    performed
    * if type deduction succeeds, the template functions are ready to be ranked 
      against non-template to determine which is the best
        * if a template function and non-template function are found to be 
        equally best, then favor the non-template function
    * if type deduction fails, then the function template is removed from 
    consideration 
* e.g. in `allDiffer(4, 4.3, 3.7);`
    * The first argument implies that T should be an int. 
    * The second argument implies it should be a double. 
    * The compiler gives up with type deduction at this point.

## multiple template parameters
```cpp
template<class T, class U, class V>
bool allDiffer(T a, U b, V c) {
    return !(a == b || a == c || b == c);
}
```

## reference
```cpp
template<class T, class U, class V>
bool allDiffer(const T& a, const U& b, const V& c) {
    return !(a == b || a == c || b == c);
}
```
* the function parameters are references that are const qualified; type 
parameters have no such qualification
* Type deduction deduces ‘around’ references and cv-qualifiers in a way that 
  preserves const-correctness
    * The rules are, of course, more involved than we making it sound here
    * but 99% of the time type deduction will just work as you expect

## compile time polymorphism
```cpp
inline
bool allDiffer(bool a, bool b, bool c) {
    return false;
}
```
* allDiffer for three bools can never return true. It must always return false.
Therefore we can provide an implementation that doesn’t even check the values. 
* If we make it inline as well, then the compiler can see that it’s always false
* If the function is called to provide a decision on which branch to take (maybe 
as the condition for an if statement), then the compiler can eliminate one of 
the branches. 
* It knows at compile time which of the branches will always be taken!

Demystification
================================================================================
## generation
```cpp
template<typename T>
void allDiffer(T a, T b) {}             // _Z9allDifferIiEvT_S0_
                                        // _Z9allDifferIdEvT_S0_

void allDiffer(int a, int b) {}         // _Z9allDifferii

int demo() {
    allDiffer<int>(1, 2);
    allDiffer<double>(1, 2);
}
```

## a summary of compilation from the notes
* compiling
    1. syntactical analysis
        * when the compiler encounters a template function it will perform a 
        sanity check, a first stage of compilation where it looks for 
        syntactical errors 
    2. semantic analysis
        * (when in semantic analysis does instantiation happen, def or use?)
            * the compiler cannot know which types to compile allDiffer for 
            until it sees us using `allDiffer` with a specific `T`
            * the compiler revisits the function template and instantiates it 
            for those typest 
        1. name lookup
            * function templates live in the template world
            * using `<>` makes sure the compiler looks within the template world
            * calling without `<>` will allow name lookup in both the template 
            world and non-template world
        2. type deduction
            * if n template parameters are provided, then they are matched 
            against the first n template parameters of the function template.
            Type deduction is performed for the remaining parameters
            * in `allDiffer(4, 4.3, 3.7);`, 1st arg -> int, 2nd arg -> 
            double, the compiler gives up with type deduction at this point
        3. function resolution
            * for function call without `<>`
                * if type deduction succeeds, the template functions are ready 
                to be ranked against non-template to determine which is the 
                best; favor the non-template function if eqaully best
                * if type deduction fails, then the function template is removed 
                from consideration 
        4. ir generation
            * the compiler must be able to see the definition of a template 
            function in order to perform instantiation. for this reason, 
            function templates are often in header files 
            * duck typing
                * T can be any type - any type that supports copy construction 
                and equality
                * There is no interface that it must implement, or common base 
                class
                * It just needs to walk like a duck and quack like a duck to be 
                considered a duck.
            * instantiation occurs once for each unique combination of template 
            parameters per translation unit
    3. optimization
        * inline
            * Function templates are not implicitly inlined so the compiler 
            might be less inclined to optimize them
            * You can make them inline as well
* linking
    * function templates follow the same ODR rules as inline functions, so 
    there is no problem with multiple definitions
    * it’s not an error to call a function template without the definition 
      being visible, as long as a definition becomes available to the linker
        * (note to self: here this TU won't instantiate the template)

## code generation is not considered in function resolution
```cpp
template <class T> void allDiffer(T a, T b) {
  a.demo();
}

void allDiffer(double a, double b) {}

int main() {
  allDiffer(1, 2);
}

// error: member reference base type 'int' is not a structure or union
//     a.demo();
```

TakeAway
================================================================================
## declaration & definition
```cpp
// non-template function
bool allDiffer(int a, int b, int c) {
    return !(a == b || a == c || b == c);
}

// template function
template<class T>
bool allDiffer(T a, T b, T c) {
    return !(a == b || a == c || b == c);
}
```
* type template parameter, type variable
* `class`, `typename`

## calling
```cpp
// calling w/ type template arguments
allDiffer<int>(8.1, 8.2, 8.3);      // false
allDiffer<double>(8.1, 8.2, 8.3);   // true

// calling w/ <> - type deduction
allDiffer<>(4, 5, 6);

// calling w/o <> - search in both worlds
allDiffer<>(4, 5, 6);
```
* template arguments fill-in template parameters
* type deduction
    * if n template parameters are provided, then they are matched against the 
    first n template parameters
    * in `allDiffer(4, 4.3, 3.7);`, 1st arg -> int, 2nd arg -> double, the 
    compiler gives up with type deduction at this point
* calling a function without the angle brackets will allow name lookup in both 
  the template world and non-template world
    * if a template function and non-template function are found to be equally 
    best, then favor the non-template function

## more syntax
```cpp
// multiple template parameters
template<class T, class U, class V>
bool allDiffer(T a, U b, V c) {
    return !(a == b || a == c || b == c);
}

// reference
template<class T, class U, class V>
bool allDiffer(const T& a, const U& b, const V& c) {
    return !(a == b || a == c || b == c);
}
```

## compiling
* compiling
    * syntactical analysis
    * semantic analysis
        * function resolution (at call site)
            * name lookup
                * template world vs non-template world
            * type deduction
            * choose function
        * function generation (at call site)
            * duck typing
            * once per TU
    * optimization
        * template function can be inlined
* linking
    * no problem with multiple definitions
    * it’s not an error to call a function template without the definition 
      being visible, as long as a definition becomes available to the linker