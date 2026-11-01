#include <stdio.h>
double x, y;
char op;
int main()
{
    while(1)
    {
        scanf("%lf %c %lf", &x, &op, &y);
        if (op == '+')
        {
            printf("%lf + %lf == %lf\n", x , y, x + y);
        }
        else if (op == '-')
        {
            printf("%lf - %lf == %lf\n", x , y, x - y);
        }
        else if (op == '*')
        {
            printf("%lf * %lf == %lf\n", x , y, x * y);
        }
        else if (op == '/')
        {
            printf("%lf / %lf == %lf\n", x , y, x / y);
        }
        else
        {
            printf("Unknown operator!\n");
        }
        printf("Do you want to continue? (y/n)\n");
        char c;
        scanf(" %c", &c);
        if (c == 'n')
        {
            break;
        }
    }
    return 0;
}