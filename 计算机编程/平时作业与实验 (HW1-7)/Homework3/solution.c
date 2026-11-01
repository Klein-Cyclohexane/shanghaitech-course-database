#include <stdio.h>
#include <stdlib.h>
int n, m;
int main()
{
    scanf("%d", &n);
    while (n--)
    {
        scanf("%d", &m);
        char* str = malloc(sizeof(char) * (m + 1));
        scanf("%s", str);
        for (int i = 0; i < (m + 1) / 2; i++)
        {
            if (str[i] != str[m - i - 1])
            {
                printf("No\n");
                break;
            }
            if (i == (m + 1) / 2 - 1)
            {
                printf("Yes\n");
            }
        }
        free(str);
    }
    return 0;
}