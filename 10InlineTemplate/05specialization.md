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