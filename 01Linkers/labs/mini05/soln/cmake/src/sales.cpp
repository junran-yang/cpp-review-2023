#include <sales.h>
#include <sumof.h>
#include <bsl_vector.h>

double salesRevenue() {
    bsl::vector<double> months;
    months.push_back(443.8);
    months.push_back(987.1);
    months.push_back(276.3);
    months.push_back(874.2);
    months.push_back(231.2);
    months.push_back(435.0);
    months.push_back(345.7);
    months.push_back(475.1);
    months.push_back(999.9);
    months.push_back(347.7);
    months.push_back(876.6);
    months.push_back(732.3);

    return sumOf(months);
}
