#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <list>
#include <algorithm>

const int N = 100;
const int MAX_INSERTION = 5;
std::mt19937 engine;
int array[N+5];
int index[N + 5];
void hack(int pivotStrategy, int seed) {
    engine.seed(seed);
    if (pivotStrategy == 1) {
        for (int i = 0; i < N; ++i)
            array[i] = N - i;
    } else if (pivotStrategy == 2) {
        for (int i = 0; i < N; ++i)
            array[i] = index[i] = i;
        for (int i = 0; i < N; ++i) {
            int pos = std::uniform_int_distribution<int>(0, N - i - 1)(engine);
            array[index[pos + i]] = i;
            std::swap(index[pos + i], index[i]);
        }
    }
    else if (pivotStrategy == 3) {
        for (int i = 0; i < N; ++i)
            array[i] = index[i] = i;
        for (int i = 0; i < N - 3; i += 2) {
            int pos1 = std::uniform_int_distribution<int>(0, N - i - 1)(engine);
            int pos2 = std::uniform_int_distribution<int>(0, N - i - 2)(engine);
            int pos3 = std::uniform_int_distribution<int>(0, N - i - 3)(engine);
            pos3 += pos3 >= pos2;
            pos3 += pos3 >= pos1;
            pos2 += pos2 >= pos1;
            array[index[pos1 + i]] = i;
            array[index[pos2 + i]] = i + 1;
            array[index[pos3 + i]] = i + 2;
            std::swap(index[pos2 + i], index[i]);
            std::swap(index[pos1 + i], index[i + 1]);
            std::swap(index[i], index[i + 1]);
        }
    }
}

int main() {
    int pivotStrategy;
    int seed;

    std::cin >> pivotStrategy >> seed;
    hack(pivotStrategy, seed);
    for (int i = 0; i < N; ++i)
        std::cout << array[i] << " ";
    return 0;
}