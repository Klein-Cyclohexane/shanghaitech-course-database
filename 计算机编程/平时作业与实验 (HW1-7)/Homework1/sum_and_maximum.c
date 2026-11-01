#include <stdio.h>
int s, sum=0, maximum=-1000;
int main()
{
    while(scanf("%d", &s))
    {
        if(s==0)
        {
            break;
        }
        sum+=s;
        if(s>maximum)
        {
            maximum=s;
        }
    }
    printf("sum: %d\n", sum);
    printf("maximum: %d\n", maximum);
    return 0;
}