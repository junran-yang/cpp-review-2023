#include "licensing.h"

#include "sumof.h"

#include <vector>

double licensingRevenue() {
    std::vector<double> quarters;
    quarters.push_back(32.7);
    quarters.push_back(43.8);
    quarters.push_back(17.2);
    quarters.push_back(21.9);

    return sumOf(quarters);
}
