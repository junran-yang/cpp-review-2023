#include <bsl_iostream.h>

#include <sumof.h>
#include <licensing.h>
#include <sales.h>

int main(int argc, char** argv) {
    roundDown = true;

    bsl::cout << "Revenue from licensing " << licensingRevenue() << bsl::endl;
    bsl::cout << "Revenue from sales " << salesRevenue() << bsl::endl;
}
