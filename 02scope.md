Scope (mostly note to self)
===========================
## outline
* what can have name
    * namespace, class, function, variable
* what can define scope
    * namespace, class, block
        * block incl: function, if|while|for|switch; {}

* what it is to be inside a scope
    * namespace: inside {}
    * class: inside {}
    * block
        * function: parameters, initialization list (for ctor), inside {}
        * if|while|for|switch: e.g. `for (int = 0; ...)`, inside {}
        * {}: inside {}

* when inside a scope
    * to introduce name
        * the name is scoped to this scope
            * namespace, class  : the full qualified name `::a::b::A::x`
            * block             : the name cannot be accessed from outside
        * what names can be introduced to the scope
            |               | introduce name                        |
            |---------------|---------------------------------------|
            | namespace     | variable, function, class, namespace  |
            | class         | variable, function, class             |
            | block         | variable, function, class             |
        * name cannot be introduced outside a namespace       
            * e.g. `int a::x = 10;` is not allowed
    * to use a name 
        * can use name both inside and outside of the scope
        * name lookup
            * name lookup starts at the scope
            * unqualified: search outwards
            * qualified:
                * first name: search outwards
                * except `::foo` search from global
            * argument dependent lookup
        * 2 types of use
            * to provide definition
            * to use definition

## specifically
* scope
    * (note to self) the name of a non-static class member is accessible, but 
      the use is invalid
        ```cpp
        class A { public: int x; };
        A::x++; // invalid use of non-static data member 'x'
        ```
* namespace
    * can be opened and closed repeatedly
    * can only be nested in namespaces
* class
    * nested class
        * can exist without containing type
        * can access containing types private members (an instance of the 
        containing type must be given though)
        * semantically closely related to containing type

## to provide definition
    * what can be defined in the scope
        |               | define                                |
        |---------------|---------------------------------------|
        | namespace     | variable, function, class, namespace  |
        | class         | variable, function, class             |
        | block         | variable, -       , class             |
    * case study
        ```cpp
        a::A a::demo(A a, B b) {
            return a;
        }
        ```
        * `a::A`, `a::demo` not in scope `a`, thus need qualifier
        * `A`, `B` in scope `a::demo`, thud don't need qualifier

## argument dependent lookup
* cppreference
    * ADL is the set of rules for looking up the unqualified function names
    in function-call expressions, including implicit function calls to 
    overloaded operators.
    * These function names are looked up in the namespaces of their 
    arguments in addition to the scopes and namespaces considered 
    * Argument-dependent lookup makes it possible to use 
* slides
    * when an unqualified free function is called, the compiler performs 
        regular unqualified lookup from the context of the use of the name
        * if the name is found within the scope of the function, then name
        lookup completes
        * if lookup leaves the function, then
            * c++ continues to look in ever widening scope until global
            scope is reached
            * the compiler also looks in the namespace of the types of the
            arguments
            * the containing namespace of the call-site, and the namespaces
            of the arguments' types are considered equal; one does not hide
            the names of the other
* to build intuition
    * prerequisite
        * function call expression
        * free function
        * unqualified function name
        * no name within the scope of the function call
        ```cpp
        void caller() {
            // demo(A, B);
            demo(a, b);
        }
        ```
    * lookup
        * regular lookup
        * the namespace of the types of its arguments
            * all arguments
            * the tightest scope of the type
        * they are considered equal
        ```cpp
        namespace a {class A;}
        namespace b {class B;}

        namespace a {void demo(A, b::B);}
        namespace b {void demo(a::A, B);}
        void demo(a::A, b::B);

        void caller() {
            demo(A, B);
        }
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
