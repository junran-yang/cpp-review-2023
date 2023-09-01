#include "licensing.h"
#include "sales.h"
#include "sumof.h"

#include <iostream>

int main(int argc, char** argv) {
    roundDown = true;
    std::cout << "Revenue from licensing " << licensingRevenue() << std::endl;
    std::cout << "Revenue from sales " << salesRevenue() << std::endl;
}
