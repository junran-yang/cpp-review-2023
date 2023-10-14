#include <iostream>
#include <string>
#include <type_traits>

#include "price.h"
#include "security.h"

// std::cout << "B()" << std::endl;

class A {
  public:
    int a = 10;
};

int main() {
  int* a = new int[5]{1, 2, 3};
  for (int i = 0; i < 5; i++) {
    std::cout << a[i] << std::endl;
  }
}