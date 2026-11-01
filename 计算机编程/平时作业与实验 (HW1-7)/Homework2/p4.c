#include <stdio.h>

int n;
int x[10]={0};
void hexToBinary(char hex[], char binary[])
{
    int i = 2, j = 0;
    while (hex[i])
    {
        switch(hex[i])
        {
            case '0':
                binary[j++] = '0';
                binary[j++] = '0';
                binary[j++] = '0';
                binary[j++] = '0';
                break;
            case '1':
                binary[j++] = '0';
                binary[j++] = '0';
                binary[j++] = '0';
                binary[j++] = '1';
                break;
            case '2':
                binary[j++] = '0';
                binary[j++] = '0';
                binary[j++] = '1';
                binary[j++] = '0';
                break;
            case '3':
                binary[j++] = '0';
                binary[j++] = '0';
                binary[j++] = '1';
                binary[j++] = '1';
                break;
            case '4':
                binary[j++] = '0';
                binary[j++] = '1';
                binary[j++] = '0';
                binary[j++] = '0';
                break;
            case '5':
                binary[j++] = '0';
                binary[j++] = '1';
                binary[j++] = '0';
                binary[j++] = '1';
                break;
            case '6':
                binary[j++] = '0';
                binary[j++] = '1';
                binary[j++] = '1';
                binary[j++] = '0';
                break;
            case '7':
                binary[j++] = '0';
                binary[j++] = '1';
                binary[j++] = '1';
                binary[j++] = '1';
                break;
            case '8':
                binary[j++] = '1';
                binary[j++] = '0';
                binary[j++] = '0';
                binary[j++] = '0';
                break;
            case '9':
                binary[j++] = '1';
                binary[j++] = '0';
                binary[j++] = '0';
                binary[j++] = '1';
                break;
            case 'A':
                binary[j++] = '1';
                binary[j++] = '0';
                binary[j++] = '1';
                binary[j++] = '0';
                break;
            case 'B':
                binary[j++] = '1';
                binary[j++] = '0';
                binary[j++] = '1';
                binary[j++] = '1';
                break;
            case 'C':
                binary[j++] = '1';
                binary[j++] = '1';
                binary[j++] = '0';
                binary[j++] = '0';
                break;
            case 'D':
                binary[j++] = '1';
                binary[j++] = '1';
                binary[j++] = '0';
                binary[j++] = '1';
                break;
            case 'E':
                binary[j++] = '1';
                binary[j++] = '1';
                binary[j++] = '1';
                binary[j++] = '0';
                break;
            case 'F':
                binary[j++] = '1';
                binary[j++] = '1';
                binary[j++] = '1';
                binary[j++] = '1';
                break;
        }
        i++;
    }
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        char hex[16];
        char binary[16] = "";
        scanf("%s", hex);
        hexToBinary(hex, binary);
        int r1 = (binary[3] - '0') * 4 + (binary[4] - '0') * 2 + (binary[5] - '0');
        int r2 = (binary[6] - '0') * 4 + (binary[7] - '0') * 2 + (binary[8] - '0');
        int imm = (binary[9] - '0') * 64 + (binary[10] - '0') * 32 + (binary[11] - '0') * 16 + (binary[12] - '0')*8 + (binary[13] - '0') * 4 + (binary[14] - '0') * 2 + (binary[15] - '0') * 1;
        if (binary[0] == '0' && binary[1] == '0' && binary[2] == '0')
        {
            if (r1 == 0)
            {
                continue;
            }
            x[r1] += x[r2];
        }
        else if (binary[0] == '0' && binary[1] == '0' && binary[2] == '1')
        {
            if (r1 == 0)
            {
                continue;
            }
            x[r1] -= x[r2];
        }
        else if (binary[0] == '0' && binary[1] == '1' && binary[2] == '0')
        {
            if (r1 == 0)
            {
                continue;
            }
            x[r1] *= x[r2];
        }
        else if (binary[0] == '0' && binary[1] == '1' && binary[2] == '1')
        {
            if (r1 == 0)
            {
                continue;
            }
            x[r1] /= x[r2];
        }
        else if (binary[0] == '1' && binary[1] == '0' && binary[2] == '0')
        {
            if (r1 == 0)
            {
                continue;
            }
            x[r1] = imm;
        }
        else if (binary[0] == '1' && binary[1] == '0' && binary[2] == '1')
        {
            printf("x%d = %d\n", r1, x[r1]);
        }
    }
    return 0;
}
