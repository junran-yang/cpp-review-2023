#include <iostream>

extern int iterations;

void playFizzbuzz(int iterationCount);

int main(int argc, char** argv) {
    std::cout << "Welcome to over engineered fizzbuzz!" << std::endl;
    std::cout
        << "(see https://blog.codinghorror.com/why-cant-programmers-program/)"
        << std::endl;
    std::cout << "Doing " << iterations << " iterations" << std::endl;

    playFizzbuzz(iterations);
}
