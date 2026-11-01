#include <stdio.h>
int cnt_p,cnt_s=0;
int out, in;
char cpeek()
{
    char ans = getc(stdin);
    ungetc(ans, stdin);
    return ans;
}
int main()
{
    scanf("%d", &cnt_p);
    while(cpeek() != 's' && cpeek() != 'p')
    {
        int check=scanf("%d %d", &out, &in);
        if(check==0)
        {
            break;
        }
        if(cnt_p < out)
        {
            printf("Impossible\n");
            return 0;
        }
        cnt_p += in;
        cnt_p -= out;
        
        cnt_s += 1;
        printf("%d %d\n", cnt_p, cnt_s);
    }
    if (getchar() == 'p')
    {
        printf("%d\n", cnt_p);
    }
    else
    {
        printf("%d\n", cnt_s);
    }
    return 0;
}