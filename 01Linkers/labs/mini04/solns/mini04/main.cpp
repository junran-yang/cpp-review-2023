#include <bsl_iostream.h>

#include <licensing.h>
#include <sales.h>

int main(int argc, char** argv) {
    bsl::cout << "Revenue from licensing " << licensingRevenue() << bsl::endl;
    bsl::cout << "Revenue from sales " << salesRevenue() << bsl::endl;
}
