#include <stdio.h>
int n;
int x;
int maximum1 = -1000, maximum2 = -999;
int minimum1 = 999, minimum2 = 1000;
int main()
{
    scanf("%d", &n);
    while(n--)
    {
        scanf("%d", &x);
        if (x > maximum1)
        {
            maximum2 = maximum1;
            maximum1 = x;
        }
        else if (x >= maximum2 && x != maximum1)
        {
            maximum2 = x;
        }
        if (x < minimum1)
        {
            minimum2 = minimum1;
            minimum1 = x;
        }
        else if (x <= minimum2 && x != minimum1)
        {
            minimum2 = x;
        }
    }
    printf("%d %d", maximum2, minimum2);
    return 0;
}