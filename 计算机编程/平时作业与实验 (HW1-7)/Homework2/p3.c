#include <stdio.h>
#include <math.h>
int n;
int x[1005];
int ascending(int x)
{
    int a[4];
    for (int i = 0; i < 4; i++)
    {
        a[i] = x % 10;
        x /= 10;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3 - i; j++)
        {
            if (a[j] > a[j + 1])
            {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
    int ans = 0;
    for (int i = 0; i < 4; i++)
    {
        ans = ans * 10 + a[i];
    }
    return ans;
}
int descending(int x)
{
    int a[4];
    for (int i = 0; i < 4; i++)
    {
        a[i] = x % 10;
        x /= 10;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3 - i; j++)
        {
            if (a[j] < a[j + 1])
            {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
    int ans = 0;
    for (int i = 0; i < 4; i++)
    {
        ans = ans * 10 + a[i];
    }
    return ans;
}
int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &x[i]);
    }
    for (int i = 0; i < n; i++)
    {
        int tmp = x[i];
        if (tmp >= 10000)
        {
            printf("%d contains more than 4 digits.\n", tmp);
            continue;
        }
        int a = descending(tmp);
        int b = ascending(tmp);
        if (a == b)
        {
            printf("%d is a repdigit.\n", tmp);
            continue;
        }
        while (tmp != 6174)
        {
            printf("%d - %d = %d\n", a, b, a - b);
            tmp = a - b;
            a = descending(tmp);
            b = ascending(tmp);
        }
    }
    return 0;
}