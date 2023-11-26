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
    ListNode<T>(T item, ListNode<T>* prev, ListNode<T>* next);
    T& item();
    const T& item() const;
    ListNode<T>* prev() const;
    ListNode<T>* next() const;

    T itemTimesFive() const;

  private:
    T _item;
    ListNode<T>* _prev;
    ListNode<T>* _next;
};
```
* a class template is a class with one or more template parameters
* syntax & semantics
    * a class must be placed in the template world using the `template` keyword
    * Template parameters are introduced inside `<>`
    * `T` has the meaning of an unknown type throughout the definition of 
    ListNode; It’s always the same unknown type though
* compilation
    * The compiler will perform a sanity check at this stage to see if it looks 
    like the code could work once we discover what type T actually is.
    * The size of ListNode<T> will depend on the size of T because ListNode<T> 
    is composed of T.

## function implementation
```cpp
template<class T>
ListNode<T>* ListNode<T>::prev() const {
    return _prev;
}
```
* syntax 
    * `ListNode<T>::`
        * Member functions must be scoped to the correct class
        * They cannot live in just `ListNode` because the class doesn’t exist
        * Nor can `ListNode` exist because classes cannot overload
        * we’re providing the definition when the one template param is unknown 
    * `template<class T>`
        * The meaning of `T` finishes after the definition of the function. 
        * The following member function definition would need to restate that 
        it is in the template world, and provide a name for the open type param
* compilation
    * the definition should be in the header file
    * At this stage, the compiler has not generated any code, it has only 
    performed a sanity check to see whether the code has any syntactical errors

## creating objects
```cpp
ListNode<int> nodeA(5, 0, 0);
```
* instantiate constructor & destructor
    * The compiler knows that we need a constructor and destructor for 
    `ListNode<int>` when it creates nodeA. These member functions will be 
    instantiated if they haven’t already been for this translation unit.
* `ListNode<int>` and `ListNode<double>`
    * There are no implicit conversions between them, or anything special 
    involving pointers or references.
* (note to self) cannot perform type deduction

## member function instantiation
```cpp
template<class T>
T ListNode<T>::itemTimesFive() const {
    return item * 5;
}
```
* compilation
    * The compiler sanity checks the definition. The compiler will produce an 
    error if it knows that this code could never compile, e.g. missing `;`
    * function instantiation delayed until the function is used with all the 
      template parameters known
        * Our example has been chosen because if T is, say, an int. Then this 
        code is fine. If it is a string, then it will fail instantiation. The 
        compiler cannot decide that there’s an error until it sees how it used.
        * Duck-typing will then check, at instantiation time, whether the 
        type(s) support the operations. If not, then a compilation error 
        (likely to appear as a template instantiation error) will result
    * If a function is never used then the duck never checks it. Hence the 'no 
    harm, no foul' reference

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

## secondary template - fully specialized
```cpp
template<>
struct Toople<bool, bool, bool> {
    bool first;
    bool second;
    bool third;
    Toople(const bool& _first, const bool& _second, const bool& _third);
    bool allDiffer() const;
};
```
* Classes cannot overload. They can, however, be specialized. 
* This is a full specialization – no unknown type or non-type parameters
* The compiler should use this class definition in the case of T, U, and V all 
being bool.

* Secondary templates specialize primary templates. 
* The specialization may only appear to the compiler after the primary template. 
* You can identify a secondary template by the `<` after the class name

* The definition of the specialization can be completely different to the 
primary template. 
* It could have a different size because it has different member variables and 
it is free to implement, or not, any member function.

```cpp
Toople<bool, bool, bool>::Toople(
    const bool& _first, const bool& _second, const bool& _third) 
    : first(_first),
      second(_second),
      third(_third) { }

inline
bool Toople<bool, bool, bool>::allDiffer() const { return false; }
```
* The angle brackets next to the type name are sufficient to identify this as 
the implementation of a full specialization.

## secondary template - partial specialization
```cpp
template<class T>
struct Toople<T*, T*, T*> {};

template<class T, class U>
struct Toople<T, U, double> {};
```
* A partial specialization is more specialized than the primary template but 
still requires a template parameter to express something that is unknown.
* The compiler will choose the most specialized implementation available once it 
knows what T, U and V are. 
* Ambiguity will cause a compilation error.

## specialization in general
* Specialization allows us to achieve something like function overloading for 
classes.
* Functions can also be fully specialized, but we don’t normally use that – 
overloading is a better choice
* You cannot specialize individual member functions – you must specialize the 
whole class. This can lead to substantial duplicate code.

## traits classes
```cpp
int maxInt = std::numeric_limits<int>::max();
double maxDouble = std::numeric_limits<double>::max();

template <class T>
bool isMax(const T& t) {
    return t == std::numeric_limits<T>::max();
}
```
* traits class – its job is to hold meta information about another type
* `numeric_limits` is a traits class
* it is specialized for different numeric types
* you can specialize for your own types