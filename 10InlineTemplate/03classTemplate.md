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
