#include <iostream>
#include <vector>
#include <climits>

std::vector<int> v;
std::vector<int> parent;
int n;

bool build(int& idx, int minVal, int maxVal, int par) {
    if (idx < 0) return true;
    int val = v[idx];
    if (val < minVal || val > maxVal) return true;
    int curr = idx;
    idx--;
    if (!build(idx, val, maxVal, curr)) return false;
    if (!build(idx, minVal, val, curr)) return false;
    parent[curr] = par;
    return true;
}

int main() {
    std::cin >> n;
    v.resize(n);
    parent.resize(n, -1);
    for (int i = 0; i < n; ++i) std::cin >> v[i];
    int idx = n - 1;
    if (build(idx, INT_MIN, INT_MAX, -1) && idx == -1) {
        for (int i = 0; i < n - 1; ++i) {
            std::cout << parent[i] + 1 << " ";
        }
    } else {
        std::cout << "Not exist" ;
    }
    return 0;
}