Take-Away
================================================================================
## Inline
* idea: copy function body to call site (, so from compiler's perspective:)
    1. definition must be visible to compiler => definition in header
    2. multiple definitions across TUs => weak symbol w/ `inline`
* to `inline` a function
    * explicit - free & member, `inline`
    * implicit - member only, put def within class def
    * note: 
        * putting a function in .h without `inline` doesn't make it `inline`
* `inline` keyword is a linker directive, not a compiler directive
    * there could be n def across TUs; still 1 in 1 TU; they are weak symbol
    * doesn't mean compiler will inline the function, & compiler can inline 
    function without `inline` keyword
* performance
    * binary size: may be larger;
    * compile-time: slower; larger number of files to recompile
    * run-time: may be slower because of larger file

## function templates
* syntax
    ```cpp
    template<class T>
    bool allDiffer(T a, T b, T c) {
        return !(a == b || a == c || b == c);
    }

    allDiffer<double>(8.1, 8.2, 8.3);
    allDiffer<>(4, 5, 6);
    allDiffer(4, 4.3, 3.7);
    ```
    * `class` vs `typename`
* template parameter and template argument
    * "type template parameter"
    * "Like function arguments that fill-in function parameters, template 
    arguments fill-in template parameters"
* more syntax
    ```cpp
    template<class T, class U, class V>
    bool allDiffer(const T& a, const U& b, const V& c); 

    template<class T, class U, class V>
    bool allDiffer(const T& a, const U& b, const V& c);
    ```
* compilation
    * (note to self) a template for compiler to generate function code
    1. syntactic analysis       => error
    2. semantic analysis    
        * function resolution
            * name lookup       => error
            * type deduction    => error
            * choose best
        * function generation
            * duck typing       => error
            - it's okay if no function is generated

## class template
* syntax for class definition
    ```cpp
    template<class T>
    class ListNode {
    public:
        ListNode<T>(T item, ListNode<T>* prev, ListNode<T>* next);
    private:
        T _item;
    }
    ```
* syntax for member function definition
    ```cpp
    template<class T>
    ListNode<T>* ListNode<T>::prev() const {
        return _prev;
    }
    ```
    * `template<class T>`: in the emplate world; provide open type param
    * `ListNode<T>`: `ListNode` class doesn't exist; nor can it exist
* syntax for creating object
    ```cpp
    ListNode<int> nodeA(5, 0, 0);
    ```
    * we're not instantiating the class, we're instanciting ctor, dtor, & member
    functions;
    * cannot perform type deduction (can't for ctor, no need fo member funcs)
* syntax for using object
    ```cpp
    nodeA.prev(); 
    ```
    * If a function is never used then the duck never checks it.
* side note
    * no relationship between `ListNode<int>`, `ListNode<double>`

## template parameters
* default tmemplate argument
    ```cpp
    template<int N, class T, class A = T>
    A accumulate(const ListNode<T>& data);
    ```
* non-type template parameter
    ```cpp
    template<int N, class T>
    T accumulate(const ListNode<T>& data); 

    accumulate<3>(nodeA); // 18
    ```
* template template parameter
    ```cpp
    template<int N, class T, template <class> class S, class A = T>
    A accumulate(const S<T>& data);
    ```
* template template parameter can be replaced with just type t.p. if not used
  as a template
    ```cpp
    template<int N, class S, class A = T>
    A accumulate(const S& data);
    ```
* `typename`
    ```cpp
    template<int N, class S>
    typename S::item_type accumulate(const S<T>& data);
    ```

## specialization
* note to self: same template, specialized
* class
    ```cpp
    template<class T, class U, class V>
    struct Toople {};

    template<class T, class U>
    struct Toople<T, U, double> {};

    template<>
    struct Toople<bool, bool, bool> {};

    bool Toople<bool, bool, bool>::allDiffer() const { return false; }
    ```
    * You cannot specialize individual member functions – you must specialize 
    the whole class
    * You can identify a secondary template by the `<` after the class name
* function
    ```cpp
    template<class T, class U, class V>
    bool allDiffer(const T& a, const U& b, const V& c); 

    template<class T, class U>
    bool allDiffer<T, U, double>(T a, U b, double c); 

    template<>
    bool allDiffer<bool, bool, bool>(bool a, bool b, bool c); 
    ```

## compile time polymorphism
* `bool allDiffer(bool a, bool b, bool c);`
* `template<int N, class T> T accumulate(const ListNode<T>& data);`