#include <iostream>
#include <algorithm>

int n, k;
int b[500005];
long long a[500005];
long long ans = 0;

int main()
{
    std::cin >> n >> k;
    for (int i = 0; i < n; i++)
    {
        std::cin >> a[i];
        a[i] *= std::min(std::min(i + 1, std::min(n - i, k)), n - k + 1);
    }
    for (int i = 0; i < n; i++)
    {
        std::cin >> b[i];
    }
    std::sort(a, a + n);
    std::sort(b, b + n);
    std::reverse(b, b + n);
    for (int i = 0; i < n; i++)
    {
        ans += a[i] * b[i];
    }
    std::cout << ans << std::endl;
    return 0;
}