TIL
================================================================================
## scope
* (note to self) the name of a non-static class member is accessible, but the 
  use is invalid
* namespace can be opened and closed repeatedly; can only be nested in 
  namespaces
* nested class 
    * can exist without containing type
    * can access containing types private members (an instance of the containing 
    type must be given though)
    * semantically closely related to containing type
* argument dependent lookup
    * prerequisite: call site; free; unqualified; no name within the scope
        ```cpp
        void caller() {
            // demo(A, B);
            demo(a, b);
        }
        ```
    * lookup: all arguments; tightest scope; equal
        ```cpp
        namespace a {class A;}
        namespace b {class B;}
        namespace a {void demo(A, b::B);}
        namespace b {void demo(a::A, B);}
        void demo(a::A, b::B);
        ```
    * useful when 
        ```cpp
        namespace a {
            class A;
            std::ostream& operator<<(std::ostream& os, const A& a);
        }
        A x;
        std::cout << x;
        ```

## introduce name
* what names can be introduced twice in the same scope
    * redefinition as a different kind of symbol 
        * var + func
        * namespace + [var, func, class]
    * ok
        * with self
        * class with [var, func]
* when introducing a name to a block
    ```cpp
    namespace a {
        void f() {
            extern int x;       // a::x
            void g();           // a::g
            class A;            // a::f():A
        }
    }
    ```

Take-away
================================================================================
## scope
* what can have a name: namespace, class, function, variable
* what can define a scope: namespace, class, block
* what is to be inside a scope
    * namespace: {}
    * class: {}
    * block: (), {}, initialization list
* to introduce a name
    * the name is scoped to the scope it is in (cannot introduce from outside)
* to use a name
    * can use name both inside and outside of the scope
    * for names defined in
        * namespace, class  : the full qualified name `::a::b::A::x`
        * block             : the name cannot be accessed from outside
    * name lookup
        * unqualified: search outwards
        * qualified: first name follows rules for the unqualified 
        * argument dependent lookup
    * 2 types of use
        * to provide definition
        * to use definition

## alias
* using directive (mostly note to self)
    ```cpp
    using namespace a;
    ```
    * does not introduce a name
        * ok to create identical name
    * changes name lookup behavior
        * behavior
            * search outwards first until hit lowest common ancestor; 
            * then search the namespace as well as the lca; 
            * and then outwards
        * applies to names introduced after `using` as well
    * transitive
    * not allowed in class
* using declaration
    ```cpp
    using a::x;
    using a::f;
    using a::A;
    ```
    * introduce a name
        * just like any other names, e.g. qualified lookup from another ns
        * introduce all member with the same name; later declarations for the 
          same name do not become visible
        * names introduced by using conflict with names introduced in the scope
        * names introduced from 2 diff scope conflict with each other
            * except for functions, but an attempt to call is still ambiguous
* using declaration for class members
    ```cpp
    using A::x;
    using A::f;
    ```
    * base to derived only
    * introduce a name
        * introduce all member with the same name
        * derived class member hides, but doesn't conflict with the member 
        introduced from base
* type alias
    ```cpp
    using B = A;
    typedef A B;
    ```
    * introduce a new name
    * no difference between `using` and `typedef`
* namespace alias
    ```cpp
    namespace b = a;
    ```
    * introduce a new name

## name linkage
#### linking (to understand .o files)
```
.cc name                                                    a::A::x
                                                           
 |           file 1       file 2       file 3                 |   
 V          +----------+ +----------+ +----------+            V   
            | external | | external | | external |  (def)         
.o symbol   | local    | | local    | | local    |          _a_A_x 
            +----------+ +----------+ +----------+                
 |            external     external    external     (use)     |   
 V            local        local       local                  V   
                                                             
bin offset                                                  0x0008
```

#### compiling (to understand .cc files)
* which names have symbols (in c's context)
    * namespace                                     -  
    * function                                      external, internal
    * variable
        * global                                    external, internal
        * local static                              internal
        * local, parameter                          - 
* name mangling
    * scope: `_a_A_demo_count`
        * anonymous namespace: `_anonymous_count`
    * function params: `_demo_bool_A`
    * static: `_count_L`
* steps
    * introduce name -> lookup name -> provide definition
    * introduce name -> lookup name -> use definition
* what are we talking about when we talk about name linkage (note to self) 
    * whether we can access the definition of a symbol from another file
    * mostly think of this in the context of c 
        * (free functions, global variables, local static functions)
    * static & anonymous namespace
        * static global variable / free function: not accessible
        * global variable / free function in an anonymous namespace: not accessible
    * moving towards c++
        * class static functions, class instance functions, class static variables
        * don't over think about it

Shortened Take-away
================================================================================
## scope
* introduce a name - must from within a scope
* use a name - first perform name lookup
    * provide definition
    * use definition
* namelookup 
    * qualified
    * unqualified
    * argument-dependent lookup (free, unqualified function)

## alias
* `using namespace a;` - name lookup only
* `using a::x;` in namespace; in class
* `using B = A; typedef A B;`
* `namespace b = a;`

## name linkage
* linking
    * extenal symbol
    * local symbol
* compiling - producing files for the above to link
    * definition -> symbol
        * in the context of c; don't over think about c++ 
            * (global var, local static var, free func)
            * (class static var, class static func, class instance func)
        * extern/local
            * e.g. `static int a; namespace {int a;}` => local 
            * e.g. `void demo() {static int a;}` => local
            * e.g. `static void demo() {}; namespace {void demo(){} }` => local
        * name mangling (each name correspond to the right symbol)
            * encompassing scope, function args, extern/local
        * introduce - use (find the right name to compile for)