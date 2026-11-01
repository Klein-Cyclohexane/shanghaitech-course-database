#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char** argv)
{
    int n = atoi(argv[1]);
    int maxlen = atoi(argv[2]);
    printf("%d\n", n);
    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        int check = rand() % 2;
        if (check)
        {
            int m = (rand() % maxlen + 2) / 2;
            char* str = malloc(sizeof(char) * m * 2);
            for (int j = 0; j < m; j++)
            {
                str[j] = rand() % 26 + 'a';
                str[m * 2 - j - 1] = str[j];
            }
            printf("%d\n", m * 2);
            for (int j = 0; j < m * 2; j++)
            {
                printf("%c", str[j]);
            }
            printf("\n");
            free(str);
            continue;
        }
        int m = rand() % maxlen + 1;
        printf("%d\n", m);
        for (int j = 0; j < m; j++)
        {
            printf("%c", rand() % 26 + 'a');
        }
        printf("\n");
    }
    return 0;
}
