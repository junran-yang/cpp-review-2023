#include <iostream>

#include "price.h"
#include "security.h"

class Price {
public:
    double bid = 0.99;
    mutable double ask = 1.01;
};

class Security {
public:
    const Price p1;
    mutable Price p2;
    int size;
    Security(Price _p) : p1(_p), p2(_p) {}
};


int main(int argc, char** argv) {
    Price p;
    const Security s(p);
    s.p1.ask = 0.98;
    s.p2 = p;
}
