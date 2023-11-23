Alias
=====
## outline
* using directive
    ```cpp
    using namespace a;      // namespace
    ```
* using declaration
    * namespace
        ```cpp
        using a::x;         // variable
        using a::f;         // function
        using a::A;         // class
        ```
    * class
        ```cpp
        class B : A {
          public:
            using A::x;     // variable
            using A::f;     // function
            using A::Inner; // class
        };
        ```
* type alias
    ```cpp
    using B = a::A;         // class
    typedef a::A B;         // class
    ```
* namespace alias
  ```cpp
  namespace b = a;          // namespace
  ```

## what names can be introduced twice in the same scope
            | variable  | function  | class     | namespace 
------------|-----------|-----------|-----------|-----------
variable    | ok        | -         | -         | -
function    | sym redef | ok        | -         | -
class       | ok        | ok        | ok        | -
namespace   | sym redef | sym redef | sym redef | ok: re-open


* one definition rule
    ```cpp
    namespace a {
        int foo;
        int foo;      // error: redefinition of 'foo'
    }
    namespace a {
        void foo() {}
        void foo() {} // error: redefinition of 'foo'
    }
    namespace a {
        class foo {};
        class foo {}; // error: redefinition of 'foo'
    }
    ```
* restricted by symbol
    ```cpp
    namespace a {
        extern int foo;
        void foo();   // error: redefinition of 'foo' as different kind of symbol
    }
    ```
* ok 
    ```cpp
    namespace a {
        int foo;
        class foo {}; 
        foo f;       // note: class 'foo' is hidden by a non-type declaration of 'foo' here
        class foo f; // ok
    }
    namespace a {
        void foo();
        class foo {}; 
        foo f;       // note: class 'foo' is hidden by a non-type declaration of 'foo' here
        class foo f; // ok
    }
    ```
* namespace is something special, in that you cannot declare without define; 
  let's not overthink about it

## when introducing a name to a block
```cpp
namespace a {
  void f() {
    extern int x;
    x++;    // (linker) undefined reference to `a::x'
  }
}
namespace a {
  void f() {
    void g();
    g();    // (linker) undefined reference to `a::g()'
  }
}
namespace a {
  void f() {
    class A;
    A a;    // error: aggregate 'a::f()::A a' has incomplete type and cannot be defined
  }
}
```

## using-directives
* Using-directives are allowed only in namespace scope and in block scope.
    ```cpp
    namespace a {
    int x = 10;
    }

    namespace b {
    class B {
        using namespace a; // error: 'using namespace' is not allowed in classes
        void demo() { using namespace a; } // ok
    };
    } // namespace b
    ```
* From the point of view of unqualified name lookup of any name after a
  using-directive and until the end of the scope in which it appears, every
  name from namespace-name is visible as if it were declared in the nearest
  enclosing namespace which contains both the using-directive and
  namespace-name.
    ```cpp
    namespace a {
    int x = 10;
    } // namespace a

    namespace b {
    int x = 20;
    namespace c {
    using namespace a;
    void demo() {
        std::cout << x << std::endl; // b::x hides a::x
    }
    } // namespace c
    } // namespace b
    ```
* Using-directive does not add any names to the declarative region in which
  it appears (unlike the using-declaration), and thus does not prevent
  identical names from being declared.
    ```cpp
    namespace a {
    int x = 10;
    } // namespace a

    namespace b {
    using namespace a;
    int x = 20; // ok
    } // namespace b
    ```
* Using-directives are transitive for the purposes of unqualified lookup: 
  if a scope contains a using-directive that nominates a namespace-name, 
  which itself contains using-directive for some namespace-name-2, the 
  effect is as if the using directives from the second namespace appear 
  within the first. The order in which these transitive namespaces occur 
  does not influence name lookup.
    ```cpp
    namespace a {
        int x = 10;
    }
    
    namespace b {
        using namespace a;
    }

    namespace c {
        using namespace b;
        // as if `using namespace a;` appears here
    }
    ```
* If, after a using-directive was used to nominate some namespace, the 
  namespace is extended and additional members and/or using-directives are 
  added to it, those additional members and the additional namespaces are 
  visible through the using-directive (in contrast with using-declaration)

## using-declaration for namespace members
```cpp
using a::x; // variable
using a::f; // function
using a::A; // class
```
* introduces a name that is defined elsewhere into the declarative region where 
  this using-declaration appears
* names introduced into a namespace scope by a using-declaration can be used 
  just like any other names, including qualified lookup from other scopes
    ```cpp
    namespace a { void f(); }
    namespace b { using a::f; } // a::f is now visible as ::b::f
    void demo() { b::f(); } // calls a::f
    ```
* If, after the using-declaration was used to take a member from a namespace, 
  the namespace is extended and additional declarations for the same name are 
  introduced, those additional declarations do not become visible through the 
  using-declaration
    ```cpp
    namespace a { void f(int); }
    using a::f; // ::f is now a synonym for a::f
    namespace a { void f(char); } // does not change what ::f means
    void foo() { f('a'); } // calls f(int)

    void bar() {
        using a::f; // this f is a synonym for both A::f(int) and A::f(char)
        f('a');     // calls f(char)
    }
    ```
* note to self: name introduced by using can conflict with name declared in the 
  scope
    ```cpp
    namespace a {
    int x;
    void f() {}
    class A {};
    } // namespace a

    namespace b {
    using a::x;
    using a::f;
    using a::A;

    extern int x;   // error: ‘int b::x’ conflicts with a previous declaration
    void f();       // error: ‘void b::f()’ conflicts with a previous declaration
    class A;        // error: declaration conflicts with target of using declaration already in scope
                    // some compiler ok;

    void f(int);    // ok
    } // namespace b

    namespace c {
    extern int x;
    void f();
    class A;

    using a::x;     // error: ‘int a::x’ conflicts with a previous declaration
    using a::f;     // error: ‘void a::f()’ conflicts with a previous declaration
    using a::A;     // error: ‘class a::A’ conflicts with a previous declaration
    } // namespace c
    ```
* note to self: for class and variables, name introduced from 2 different scope 
  can conflict; for function, 2 using-declarations can introduce functions with 
  the same name and parameter list, but if a call to that function is attempted, 
  the program is ill-formed.
    ```cpp
    namespace a {
    int x;
    void f() {}
    class A {};
    } // namespace a

    namespace b {
    extern int x;
    void f();
    void f(int);
    class A;
    } // namespace b

    namespace c {
    using a::A;
    using a::f;
    using a::x;

    using b::A; // error: ‘class b::A’ conflicts with a previous declaration
    using b::f; // ok
    using b::x; // error: ‘int b::x’ conflicts with a previous declaration

    void demo() {
        f();    // error: call of overloaded ‘f()’ is ambiguous
        f(10);  // ok
    }
    } // namespace c
    ```

## using-declaration for class members
* Using-declaration introduces a member of a base class into the derived class 
  definition, such as to expose a protected member of base as public member of 
  derived.
* In this case, nested-name-specifier must name a base class of the one being 
  defined.
    ```cpp
    class A {
    public:
        static int x;
        static void f();
    };

    void demo() {
        using A::f; // error: using-declaration for member at non-class scope
        using A::x; // error: using-declaration for member at non-class scope
    }
    ```
* If the name is the name of an overloaded member function of the base class, 
  all base class member functions with that name are introduced.
* If the derived class already has a member with the same name, parameter list, 
  and qualifications, the derived class member hides or overrides (doesn't 
  conflict with) the member that is introduced from the base class.
    ```cpp
    class A {
    public:
        int x = 10;
        void f() { std::cout << "A" << std::endl; }
    };

    class B : public A {
    public:
        int x = 20;
        void f() { std::cout << "B" << std::endl; }
        using A::f;
        void demo() { f(); } // B
    };

    class C : public B {
    public:
        using A::f;
        using A::x;
        void demo() { f(); } // A
    };
    ```

## type alias
```cpp
using B = A;
typedef A B;
```
* A type alias declaration introduces a name which can be used as a synonym for 
  the type denoted by type-id. 
* It does not introduce a new type and it cannot change the meaning of an 
  existing type name. 
    ```cpp
    namespace a {
    class A { };
    }

    namespace b {
    using A = b::A; 
    class A { }; // error: definition of type 'A' conflicts with type alias of the same name
    }

    namespace c {
    class A { }; 
    using A = b::A; // error: typedef redefinition with different types ('a::A' vs 'b::A')
    }
    ```
* There is no difference between a type alias declaration and typedef declaration. 
* This declaration may appear in block scope, class scope, or namespace scope.
