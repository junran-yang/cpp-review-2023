#include <iostream>

template <class T> void allDiffer(T a, T b) {
  a.demo();
}

void allDiffer(double a, double b) {}

int main() {
  allDiffer(1, 2);
}