Section 04: Template Parameters
================================================================================
## running example
```cpp
template<class T>
T accumulate(const ListNode<T>& data) {
    T result = T();
    const ListNode<T>* pos = &data;
    while (pos) {
        result += pos->item();
        pos = pos->next();
    }
    return result;
}
```

## limited accumulate
* **from your tl**: provide a limit so that at most n items are accumulated
```cpp
template<class T>
T accumulate(const ListNode<T>& data, int n) {
    T result = T();
    int count = 0;
    const ListNode<T>* pos = &data;
    while (pos && count++ < n) {
        result += pos->item();
        pos = pos->next();
    }
    return result;
}
```
* if we knew what n was at compile time, and we could communicate that to the 
compiler, then maybe the compiler could make better compilation choices, e.g. 
better loop-unrolling.

## non-type template parameters
* **from your tl**: they always know n at compile time
```cpp
template<int N, class T>
T accumulate(const ListNode<T>& data) {
    T result = T();
    int count = 0;
    const ListNode<T>* pos = &data;
    while (pos && count++ < N) {
        result += pos->item();
        pos = pos->next();
    }
    return result;
}
```
* Non-type parameters must have a type (int in this case) because they convey a 
typed value.
* All template parameters must be known at compile time. 
* The template can only be instantiated once all template parameters are known 
(both type and non-type parameters). 
* Different values of N will generated different functions.

```cpp
ListNode<int> nodeA(5, 0, 0);
ListNode<int> nodeB(6, &nodeA, 0);
ListNode<int> nodeC(7, &nodeB, 0);

accumulate(nodeA, 0); // 0
accumulate(nodeA, 1); // 5
accumulate(nodeA, 2); // 11
accumulate(nodeA, 3); // 18

accumulate<0>(nodeA); // 0
accumulate<1>(nodeA); // 5
accumulate<2>(nodeA); // 11
accumulate<3>(nodeA); // 18

// 4 instantiations
// accumulate<0, int>(const ListNode<int>&)
// accumulate<1, int>(const ListNode<int>&)
// accumulate<2, int>(const ListNode<int>&)
// accumulate<3, int>(const ListNode<int>&)
```
* Templates can contribute to bloat. 
* providing N before T: 
    * When you provide explicit parameters, they match the declaration order. N 
    cannot be deduced in our function so we must provide it. If they’re ordered 
    T, N, then we must explicitly provide T as well. Ordering them N, T allows T to be deduced.

* type template parameter
    * a variable for a type
    * must be known at compile time
* non-type template parameter
    * a variable for a value
    * must be known at compile time
    * templates states the type
    * can be an integral type, enum, or pointer (with restrictions)
        * you cannot have a double non-type parameter
        * The reason for this is that there must be a new instance created for 
        each used combination of parameters. Floating point comparisons are 
        tricky, and you might end up with two numbers which logically are the 
        same but due to rounding do not compare equal. This may lead to multiple 
        instance when there should only be one, and incompatible types where 
        logically they should be compatible.

## separate accumulation type
* **from your tl**: accumulate using a different type
```cpp
template<int N, class T, class A>
A accumulate(const ListNode<T>& data) {
    A result = A();
    int count = 0;
    const ListNode<T>* pos = &data;
    while (pos && count++ < N) {
        result += static_cast<A>(pos->item());
        pos = pos->next();
    }
    return result;
}
```

## default template arguments
* **from your tl**: if T is double, then by default A should be double
```cpp
template<int N, class T, class A = T>
A accumulate(const ListNode<T>& data) {
    A result = A();
    int count = 0;
    const ListNode<T>* pos = &data;
    while (pos && count++ < N) {
        result += static_cast<A>(pos->item());
        pos = pos->next();
    }
    return result;
}
```
* Default values may make use of preceding template parameters, but not 
following ones

## template parameter for node
* **from your tl**: we need it to work with `TreeNode<T>, TrieNode<T>, etc`
```cpp
template<int N, class T, template <class> class S, class A = T>
A accumulate(const S<T>& data) {
    A result = A();
    int count = 0;
    const S<T>* pos = &data;
    while (pos && count++ < N) {
        result += static_cast<A>(pos->item());
        pos = pos->next();
    }
    return result;
}
```
* when using S<T>, the compiler is unhappy. It’s not convinced that our code 
will work, because what if it later turns out that the type of S does not have a 
template parameter?
* This is not the toughest syntax in C++ but it’s some of the toughest you’re 
likely to encounter.
* S is still a template parameter, but it is constrained to be a type that must 
have a single type parameter.
* That guarantees that S<T> is syntactically correct, and so the ‘sanity check’ 
can pass
* It doesn’t guarantee that the code will actually compile once we know what S 
and T are, just that it will syntactically make sense, and that the < for 
template parameters won’t be confused with less-than.

## a simplification
* **from your tl**: template template parameters scares me. given that we have
A, do we really need to work out T? 
```cpp
template<int N, class S, class A>
A accumulate(const S& data) {
    A result = A();
    int count = 0;
    const S* pos = &data;
    while (pos && count++ < N) {
        result += static_cast<A>(pos->item());
        pos = pos->next();
    }
    return result;
}

int a = accumulate<3, ListNode<int>, int>(nodeA);
```
* Writing generic code is a different skill to writing OO code. It requires us 
to think of the abstract and allow the duck-typing to fill in the blanks.
* we're trusting the duck!
* We just have some container, S. We don’t care if it is template or not, it 
just needs to support the operations we perform on it.

## auto (c++11)
* **from your tl**: that's too ugly. maybe we can work it out from item()'s 
return type?
```cpp
template<int N, class S>
auto accumulate(const S<T>& data) {
    using A = std::decay_t<decltype(data.item())>;
    A result = A();
    int count = 0;
    const S* pos = &data;
    while (pos && count++ < N) {
        result += static_cast<A>(pos->item());
        pos = pos->next();
    }
    return result;
}
```
* The auto keyword used as a return type will allow C++ to deduce the return 
type, in this case it will be deduced from the type of result.
* The type of A is taken from the return type of data.item using decltype. 
decltype (C++11 not 03) yields a type. 
    * It is similar to sizeof in that it does not evaluate the expression
* decay_t (C++14) removes qualifiers such as const and &.
* using is a C++11 way to set up an alias

## trait
* **from your tl** can you do it with c++03? 
```cpp
template <class T>
class ListNode {
  public:
    typedef T item_type;
}
```
* We ask, or assume, that all of the containers that will be used also provide 
a typedef for item_type that matches the type returned by item. 
* This increases the requirements from a duck typing view, but we already 
expected a function called item. We’re just extending those requirements. 
* The implementation of accumulate can now ask for S::item_type. 
* item_type is a trait of the container.

```cpp
template<int N, class S>
typename S::item_type accumulate(const S<T>& data) {
    typedef typename S::item_type A;
    A result = A();
    int count = 0;
    const S* pos = &data;
    while (pos && count++ < N) {
        result += static_cast<A>(pos->item());
        pos = pos->next();
    }
    return result;
}
```
* The return type and typedef aren’t just S::item_type, they use typename S::item_type
* Without the typename, the compiler would be uncertain what kind of thing 
S::item_type was when it performed the first stage of compilation (sanity check).
* item_type depends on S, and we don’t yet know what S is. item_type could turn 
out to be a static member variable, or a static member function in which case 
the syntax of using it as a type would be incorrect. The compiler needs 
convincing that it is a type, and hence you must use typename. If it turns out 
not to be a type at template instantiation time, the compiler will issue an error immediately.
* You can only use typename for this purpose, you cannot use class.

## return of A
```cpp
template<int N, class S, class A = typename S::item_type>
A accumulate(const S<T>& data) {
    A result = A();
    int count = 0;
    const S* pos = &data;
    while (pos && count++ < N) {
        result += static_cast<A>(pos->item());
        pos = pos->next();
    }
    return result;
}
```
* The ability to explicitly provide A was missing in our previous solution, 
we’ve reintroduced that with a minor change.
