#include <iostream>

template<class T, class U>
void differ(T a, U b) {
  std::cout << "T, U" << std::endl;
}

template<>
void differ(double a, int b) {
  std::cout << "haha" << std::endl;
}

int main() {
  differ<>(1.2, 5);
}