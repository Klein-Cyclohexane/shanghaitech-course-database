#include <iostream>
#include <queue>

struct category {
    long long f, c;
    bool operator < (const category &a) const {
        return f > a.f;
    }
};

category p[100005];

int m;
std::priority_queue<category> q;

int main() {
    std::cin >> m;
    for (int i = 0; i < m; i++) {
        std::cin >> p[i].f >> p[i].c;
        q.push(p[i]);
    }
    long long ans = 0;
    while (q.size() > 1) {
        category a = q.top();
        q.pop();
        if (a.c > 1) {
            if (a.c % 2 == 0) {
                category b = a;
                b.f *= 2;
                b.c /= 2;
                q.push(b);
                ans += (b.f * b.c);
            }
            else {
                category b = a;
                b.c /= 2;
                b.f *= 2;
                q.push(b);
                a.c = 1;
                q.push(a);
                ans += (b.f * b.c);
            }
        } else {
            category b = q.top();
            q.pop();
            category c;
            c.f = a.f + b.f;
            c.c = 1;
            q.push(c);
            if (b.c > 1) {
                b.c --;
                q.push(b);
            }
            ans += c.f;
        }
        //std::cout << ans << std::endl;
    }
    std::cout << ans << std::endl;
    return 0;
}