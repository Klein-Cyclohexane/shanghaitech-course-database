#include "Dynamic_Array_2.0.hpp"
#include <algorithm>
#include <iostream>

int main() {
    int arr[] = {1, 2, 3, 5};
    Dynarray a(arr, arr + 4);
    Dynarray b;
    std::cout << a << '\n' << b << std::endl;
    return 0;
}