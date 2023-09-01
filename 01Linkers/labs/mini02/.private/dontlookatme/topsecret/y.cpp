#include <iostream>

int iterations = 100;

bool isFizz(int i);
bool isBuzz(int i);

void playFizzbuzz(int iterationCount) {
   for (int i = 1; i <= iterationCount; ++i) {
       if (isFizz(i) && isBuzz(i)) {
           std::cout << "FizzBuzz" << std::endl;
       }
       else if (isFizz(i)) {
           std::cout << "Fizz" << std::endl;
       }
       else if (isBuzz(i)) {
           std::cout << "Buzz" << std::endl;
       }
       else {
           std::cout << i << std::endl;
       }
   }
}
