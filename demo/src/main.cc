#include <iostream>

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
    T item_;
    ListNode<T>* prev_;
    ListNode<T>* next_;
};

template<class T>
ListNode<T>* ListNode<T>::prev() const {
    return prev;
}

int main() {
}