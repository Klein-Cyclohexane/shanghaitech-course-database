#include <stdio.h>
int n,m;
double a[31];
int main()
{
    scanf("%d",&n);
    for (int i = 0; i <= n; i++)
    {
        scanf("%lf",&a[i]);
    }
    scanf("%d",&m);
    for (int i = 0; i < m; i++)
    {
        double x;
        scanf("%lf",&x);
        double ans = a[0];
        double cnt = x;
        for (int j = 1; j <= n; j++)
        {
            double tmp = a[j]*cnt;
            cnt *= x;
            ans += tmp;
        }
        printf("%.3lf\n",ans);
    }
    return 0;
}
