#include <iostream>

#include "price.h"
#include "security.h"


class Price {
  public:
    // Price() : bid(0.99), ask(1.01) {}
    double bid;
    double ask;
};

class Security {
  public:
    Price p;
};

int main(int argc, char** argv) {
  Security s;
  std::cout << s.p.bid << ", " << s.p.ask << std::endl;
}