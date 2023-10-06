#include <iostream>
#include <string>
#include <type_traits>

#include "price.h"
#include "security.h"

// std::cout << "B()" << std::endl;

class A { 
  public: 
};

class B { 
  public: 
};

class C : public A, public B { 
  public: 
};


int main() {
  A a;
  static_cast<B&>(a);
}