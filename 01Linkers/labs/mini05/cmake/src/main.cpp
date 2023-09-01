#include <iostream>

#include "licensing.h"
#include "sales.h"

int main(int argc, char** argv) {
    std::cout << "Revenue from licensing " << licensingRevenue() << std::endl;
    std::cout << "Revenue from sales " << salesRevenue() << std::endl;
}
