#include <iostream>

struct node {
    int color;
    int l, r;
};

node tree[1000005];
int n;
int cnt = 1;
int ans = 1;

inline bool check(int node1, int node2) {
    if (node1 == -1 && node2 == -1) {
        cnt -= 2;
        return true;
    }
    if (node1 == -1 || node2 == -1) {
        cnt = 1;
        return false;
    }
    if (tree[node1].color != tree[node2].color) {
        cnt = 1;
        return false;
    }
    if (node1 == node2) {
        cnt += 2;
        return check(tree[node1].l, tree[node2].r);
    }
    else
    {
        cnt += 4;
        return check(tree[node1].l, tree[node2].r) && check(tree[node1].r, tree[node2].l);
    }
}

int main() {
    std::cin >> n;
    for (int i = 1; i <= n; i++) {
        std::cin >> tree[i].color;
        tree[i].l = tree[i].r = -1;
    }
    for (int i = 1; i <= n; i++) {
        std::cin >> tree[i].l >> tree[i].r;
    }
    for (int i = 1; i <= n; i++) {
        cnt = 1;
        if (check(i, i))
        {
            ans = std::max(ans, cnt);
        }
    }
    std::cout << ans << std::endl;
    return 0;
}