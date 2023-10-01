#include <iostream>
#include <string>

#include "price.h"
#include "security.h"

class A { };

class B {
  public:
    operator A&() {
      return a;
    }
    A a;
};

int main() {
  B b;
  const A& a = b;
}