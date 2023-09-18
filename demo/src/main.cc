#include <iostream>
#include <string>

#include "price.h"
#include "security.h"


 class Demo {
  public:
    Demo() {
      std::cout << "default ctor" << std::endl;
    }

    Demo(int _a) : a(_a) {
      std::cout << "direct ctor " << a << std::endl;
    }

    Demo(const Demo& other) : a(other.a) {
      std::cout << "copy ctor " << a << std::endl;
    }

    int a;
};

int main(int argc, char** argv) {
  nullptr_t p = nullptr;
}