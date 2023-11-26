Section 03: Class Template
================================================================================
## running example
```cpp
class IntListNode {
  public:
    IntListNode(int item, IntListNode* prev, IntListNode* next);
    int& item();
    int item() const;
    IntListNode* prev() const;
    IntListNode* next() const;

  private:
    int item_;
    IntListNode* prev_;
    IntListNode* next_;
};
```

## class template
```cpp
template<class T>
class ListNode {
  public:
    IntListNode(T item, ListNode<T>* prev, ListNode<T>* next);
    T& item();
    const T& item() const;
    ListNode<T>* prev() const;
    ListNode<T>* next() const;

    T itemTimesFive() const;

  private:
    T item_;
    ListNode<T>* prev_;
    ListNode<T>* next_;
};
```
* a class template is a class with one or more template parameters
* a class must be placed in the template world using the template keyword
* Template parameters are introduced inside the angle brackets
* T has the meaning of an unknown type throughout the definition of ListNode. 
* It’s always the same unknown type though
* The compiler will perform a sanity check at this stage to see if it looks like
the code could work once we discover what type T actually is.
* d_item is of type T
* The size of ListNode<T> will depend on the size of T because ListNode<T> is 
composed of T.

## function implementations
```cpp
template<class T>
ListNode<T>* ListNode<T>::getPrev() const {
    return prev;
}
```
* Member functions must be scoped to the correct class
    * They cannot live in just ListNode because the class doesn’t exist
        * ListNode cannot exist because classes cannot overload
    * we’re providing the definition when the one template parameter is unknown 
* As with free function templates, the definition should be in the header file
* At this stage, the compiler has not generated any code, it has only performed 
a sanity check to see whether the code has any clear syntactical errors.
* The meaning of T finishes after the definition of the function. The following 
member function definition would need to restate that this is in the template 
world, and provide a name for the open type parameter.

## creating objects
```cpp
ListNode<int> nodeA(5, 0, 0);
ListNode<int> nodeB(8, 0, &nodeA);
ListNode<int> nodeC(10, &nodeB, &nodeA);
```
* The compiler knows that we need a constructor and destructor for ListNode<int> 
when it creates nodeA. These member functions will be instantiated if they 
haven’t already been for this translation unit.
* ListNode<int> is an entirely different type to ListNode<double>, even though 
they have both been generated from the same template. There are no implicit 
conversions between them, or anything special involving pointers or references.

## member function instantiation
```cpp
template<class T>
T ListNode<T>::itemTimesFive() const {
    return item * 5;
}
```
* Member function instantiation is delayed until the function is used with all the
template parameters known.
* The compiler sanity checks the definition. The compiler will produce an error 
if it knows that this code could never compile, e.g.  there is a missing semi-colon.
* Our example has been chosen because if T is, say, an int. Then this code is 
fine. If it is a string, then it will fail instantiation. The compiler cannot 
decide that there’s an error until it sees how it used.
* Using a member function when the template parameters are known will trigger 
instantiation for that function, unless it has already been instantiated for that translation unit.
* Duck-typing will then check, at instantiation time, whether the type(s) support 
the operations. If not, then a compilation error (likely to appear as a template 
instantiation error) will result.
* If a function is never used then the duck never checks it. Hence the ‘no harm, no foul’ reference

## question
* does code generation happen for class? or only for function (incl. member function)?
* member function with both template parameter for class and template parameter 
for the function itself? 

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

## question
* what is trait

Section 05: Specialization
================================================================================
## primary template
```cpp
template<class T, class U, class V>
struct Toople {
    T first;
    U second;
    V third;
    Toople(const T& _first, const U& _second, const V& _third);
    bool allDiffer() const;
};
```
* This is a primary template. 

## specialization
* For functions, that meant that we could provide a non-template function for 
bools that just returned false. Overload resolution would prefer it to the 
template version if it was given three bool values.
* Classes cannot overload. They can, however, be specialized. 
```cpp
template<>
struct Toople<bool, bool, bool> {
    T first;
    U second;
    V third;
    Toople(const T& _first, const U& _second, const V& _third);
    bool allDiffer() const;
};
```
* This is a full specialization
* Secondary templates specialize primary templates.
* The specialization may only appear to the compiler after the primary template. 
* You can identify a secondary template by the < after the class name – primary templates do not have this.
* The definition of the specialization can be completely different to the 
primary template. It could have a different size because it has different member 
variables and it is free to implement, or not, any member function. Just because 
the primary Toople has allDiffer none of the specializations are required to. 
Duck typing will check that a class has the necessary members as and when they’re used.
