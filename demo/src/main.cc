<<<<<<< Updated upstream
int main(int argc, char** argv) {
    return 0;
=======
#include <iostream>
#include <vector>

enum Flavor {
    Pistacchio = 5,
    Vanilla,
    Chocolate = 4,
    Strawberry
};

int main(int argc, char** argv) {
    int a = 5;
    int* const ap = &a;
    *ap = 3;
    std::cout << a << std::endl;
>>>>>>> Stashed changes
}

