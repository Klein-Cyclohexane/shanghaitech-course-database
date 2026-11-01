#include <stdio.h>
#include <math.h>
double a, b, c;
int main()
{
    scanf("%lf%lf%lf", &a, &b, &c);
    double delta = b * b - 4 * a * c;
    if (a == 0)
    {
        if (b == 0)
        {
            if (c == 0)
            {
                printf("x\\in\\mathbb{R}\n");
            }
            else
            {
                printf("No solution.\n");
            }
            return 0;
        }
        printf("x = %.3lf\n", -c / b);
        return 0;
    }
    if (delta < 0)
    {
        printf("No solution.\n");
    }
    else if (delta == 0)
    {
        printf("x1 = x2 = %.3lf\n", -b / (2 * a));
    }
    else
    {
        if ((-b - sqrt(delta)) / (2 * a) < (-b + sqrt(delta)) / (2 * a))
            printf("x1 = %.3lf, x2 = %.3lf\n", (-b - sqrt(delta)) / (2 * a), (-b + sqrt(delta)) / (2 * a));
        else
            printf("x1 = %.3lf, x2 = %.3lf\n", (-b + sqrt(delta)) / (2 * a), (-b - sqrt(delta)) / (2 * a));
    }
    return 0;
}