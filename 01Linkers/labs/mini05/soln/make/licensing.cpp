#include <licensing.h>
#include <sumof.h>
#include <bsl_vector.h>

double licensingRevenue() {
    bsl::vector<double> quarters;
    quarters.push_back(32.7);
    quarters.push_back(43.8);
    quarters.push_back(17.2);
    quarters.push_back(21.9);

    return sumOf(quarters);
}
