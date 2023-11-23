Section 01: Scope; Name Lookup
==============================
## recap: declaration & definition
|           | declaration   | definition     |
|-----------|---------------|----------------|
| variable  | extern int a; | int a;         |
| function  | void demo();  | void demo() {} |
| class     | class Demo;   | class Demo {}  |

## scope
* a place for names to be declared and found by lookup

## scope: namespace
* a namespace defines a scope
    * for names declared within a namespace, their full, qualified name includes
    the namespace name.
    * namespaces are logical only. They can affect name lookup but they do not 
    physically change the code generated
    * namespaces can be opened and closed repeatedly.
* introduce names
    * variable; function; class
    * namespaces may only nest inside namespaces
* provide definition to the names in a namespace
    * variable; function; class
    * two ways to define members that are in a namespace
        * a definition may be placed within the scope of the namespace
        * it can be defined using an explicitly qualified definition
    * subtlety: `void foo::demo(a&)` - the compiler has started the search for 
    `a` within `foo`, because `demo` is in `foo`.
* types of namespaces
    * named namespace
    * unnamed namespace (also called anonymous namespace)
    * global scope is a namespace. it’s one that doesn’t have a name. it’s not 
    an anonymous namespace (they’re something different)
    
## scope: class
* class types create scopes in which things can be named.
    * we have seen this for member variables, and member functions. we can also 
    create a type within the scope of the class.
    * note to self: Inner class's definition can be provided outside of Outter
    class
* nested class semantics
    * using a nested type communicates that the nested type only has meaning in 
    relation to the containing type. It might be an implementation, and would 
    probably be private in this case. 
    * nested types are members of their containing types. member functions of 
      the nested type have access to private members in the containing type. 
      However, an instance of the nested class will need to be given an instance 
      of the containing class to take advantage of this. There is no implicit 
      back reference.
        ```cpp
        class Outter {
            int a = 10;
            class Inner {
                void demo(Outter& o) { 
                    std::cout << a << std::endl;    // error: use of non-static data member 'a' of 'Outter' from nested type 'Inner'
                    std::cout << o.a << std::endl;  // ok
                }
            };
        };
        ```
    * it is possible to instantiate a nested type without having any instance of
    the containing type.

## scope: block
* introduce names
    * variable; function; class
    * function 
        * parameters are considered to be in the block associated with the 
        function definition. If it’s a constructor, then they’re in scope before 
        the body so that they can be used to initialize members.
    * if, while, for, and switch 
        * are permitted to declare variables. These variables are considered to 
        be within their block.
        * the variable declared in the initialization part of the for can be 
        used in the condition and iteration parts.
    * `{}` on its own
        * useful to apply RAII.
* provide definition to the names in a namespace
    * variable; class; 
* note to self: cannot use scope resolution operator to find names in block

## name lookup
* unqualified name lookup
    * start at the tightest scope and work outwards
    * names in outer scopes are hidden by names in inner scopes; The derived 
    class will hide members from the base class. 
    * the search for the name stops once it has been found. this can limit the 
    number if functions that are considered during function resolution. 
    * name lookup fails if we reach global scope but have not found the name
* qualified name lookup
    * the first name (the leftmost) is looked up using the rules for unqualified 
    lookup. 
    * the remaining names are then looked up relatively.
    * all names in a qualified name, except the very last, must be either a 
    class name or a namespace name.
    * special syntax for starting the qualified name at global scope – `::demo`

Section 02: namespace alias; using; typedef; using declaration
==============================================================
## namespace alias
```cpp
namespace foo = demo;
```
* a namespace alias allows you to provide an alternative name for an existing 
namespace.
* this other name is a declaration within its scope.

## using declaration
```cpp
using demo::Demo;
```
* using declaration can declare the name in another scope
* `Demo` is being declared within the function’s block. This isn’t a new type. 
When we refer to `Demo` within this block, it means `demo::Demo`

## using directive
```cpp
using namespace demo;
```
* using directive makes all of the names from a namespace available without 
qualification
* it does not declare the names from the namespace within its scope; It makes 
the names available, but they can still be hidden by the scope. 
* its effects only last to the end of the scope.
* a namespace can contain a using directive. When the compiler finds a using 
directive, it transitively ‘copies’ the other using directives to the same 
location and applies them as well.
* the names are ‘injected’ into the tightest scope that contains both the using 
directive and the used namespace.

## typedef
```cpp
typedef demo::CmdyFuture CommodityFuture;
```
* A typedef introduces a new name for an existing type.
* It’s only an alias, it does not create a new type.

## use `using` carefully
* a using directive has no place being in a header file
    * If we do that, then we have just increased the chance of name collisions 
    and subtle naming errors. Header files should always use qualified names.
* in an implementation file you have more freedom to choose because your choices
will effect the compilation of that file only. 
* consider placing the using in the tightest scope that it remains useful, or 
just using qualified names.

Section 03: name linkage
========================
## static
* The linkage of a name controls if, and how, it is seen across the TUs
* external linkage
    * Variables and functions with external linkage can be seen by other TU
    * non-static definitions have external linkage (omitting `static`)
* internal linkage
    * internal linkage restricts the name to within the translation unit
    * static variables and functions cannot be found by other TU
* no linkage
    * local variables have no linkage.
    * they're never linked against because they can only exist when the owning 
    function is running
* side notes
    * static cannot be applied to types or namespaces
    * The C++03 standard deprecated static in favor of unnamed namespaces

## unnamed namespace
* unnamed namespaces is also known as anonymous namespaces
* The unnamed namespace injects its members into the containing namespace for 
that translation unit. It does not make the names available outside of the 
translation unit.
* (**take this with a grain of salt** )
  The names are expected to have external linkage, but the standard requires 
that they’re placed within a unique name (implemented by the compiler). This is 
a name that cannot be guessed between translation units. 
* Unnamed namespaces have the advantage of applying uniformly to all namespace 
members whereas static only applies to functions and variables.

Section 03.02 argument dependent lookup
=======================================
## Argument Dependent Lookup
```cpp
void swap(demo::Demo a, demo::Demo b) {}

namespace demo {
    class Demo {};
    void swap(Demo a, Demo b) {}
}

void argumentDependentLookup() {
    demo::Demo a;
    demo::Demo b;
    swap(a, b);
}
```
* which `swap` is used? 
    * `::swap`
    * `demo::swap` <- this!
* when an unqualified free function is called, the compiler performs regular unqualified lookup from the context of the use of the name. 
    * if the name is found within the scope of the function, then name lookup completes
    * if lookup leaves the function, then 
        * c++ continues to look in ever widening scope until global scope is reached. 
        * the compiler also looks in the namespaces of the types of the arguments. 
        * The containing namespace of the call-site, and the namespaces of the argument’s types are considered equal; one does not hide the names of the other.
* benefits of adl
    * Operator overloading would not work very well if all function calls had to be qualified
    * ADL fits well with the idea that the interface to a type includes related free functions.

Appendix
========
## original questions on name linkage
* isn't linking only related to symbols? 
    - right
* what is the static keyword in global scope? 
    - internal linkage
* what if you have static definition in one TU, but non-static definition in another TU? 
    - within this TU 1 name can only map to 1 symbol
* error: static declaration of 'demo' follows non-static declaration
    - extern and static are both storage class specifier
* can i have 2 `demo()`, one static one non-static? 
    - not in 1 TU
    - ok in 2 TUs
* why do you want class and namespace to have internal linkage anyways? they're compiled away anyways
    - class: functions and static variables
* why applying unnamed namespace to types and namespace? 
    - see above
* can a non-static function use a static variable? 
    - yes
* "local" "temporary"
    - "local" in a object file vs src file
    - local vs temporary
* namespace + static
    - hmm
* can unnamed namespace be nested? 
    - yes
* name linkage vs name scope
    * how does name linkage have anything to do with name scope? 
        - part of the name
    * unnamed namespace vs named namespace
        - hmm
    * namespace and static/internal linkage are fundamentally different, right? 
        - right
* are global variables in global scope? or just a variable that's not in any class? 
    - i guess the latter
* does static local has symbol? 
    - yes
* class inside a function: does it produce a symbol? does it have linkage? 
    - class don't produce symbol
* can we have an anonymous scope inside another anonymous scope? 
    - yes
