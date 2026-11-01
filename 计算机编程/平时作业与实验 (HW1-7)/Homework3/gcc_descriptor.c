#include <stdio.h>
#include <stdlib.h>

int hw3_islower(int ch)
{
    return ch >= 'a' && ch <= 'z';
}
int hw3_isupper(int ch)
{
    return ch >= 'A' && ch <= 'Z';
}
int hw3_isalpha(int ch)
{
    return hw3_islower(ch) || hw3_isupper(ch);
}
int hw3_isdigit(int ch)
{
    return ch >= '0' && ch <= '9';
}
size_t hw3_strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
    {
        len++;
    }
    return len;
}
int hw3_strcmp(const char* lhs, const char* rhs)
{
    while (*lhs && *rhs)
    {
        if (*lhs != *rhs)
        {
            return *lhs - *rhs;
        }
        lhs++;
        rhs++;
    }
    return *lhs - *rhs;
}
int hw3_strncmp(const char* str1, const char* str2, size_t n)
{
    while (n && *str1 && *str2 && *str1 == *str2)
    {
        str1++;
        str2++;
        n--;
    }
    return n ? *str1 - *str2 : 0;
}
int hw3_strmcmp(const char* str1, const char* str2, size_t m)
{
    size_t len1 = hw3_strlen(str1);
    size_t len2 = hw3_strlen(str2);
    while (m && str1[len1-1] && str2[len2-1] && str1[len1-1] == str2[len2-1])
    {
        len1--;
        len2--;
        m--;
    }
    return m ? str1[len1-1] - str2[len2-1] : 0;
}
int main(int argc, char** argv)
{
    char *wall = "-Wall";
    char *wextra = "-Wextra";
    char *werror = "-Werror";
    char *wpedantic = "-Wpedantic";
    char *o = "-o";
    char *I = "-I";

    char *std1 = "-std=c";
    char *std2 = "-std=c++";
    char *std3 = "-std=gnu";
    char *std4 = "-std=gnu++";

    char *other_input1 = ".c";
    char *other_input2 = ".h";
    char *other_input3 = ".cpp";
    char *other_input4 = ".C";
    char *other_input5 = ".cc";
    char *other_input6 = ".cxx";
    char *other_input7 = ".hpp";
    char *other_input8 = ".hxx";
    int n = argc;


    for (int i = 0;i < n;i++)
    {
        
        if (hw3_strcmp(argv[i], wall) == 0)
        {
            printf("-Wall: Enable all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning).\n");
        }
        else if (hw3_strcmp(argv[i], wextra) == 0)
        {
            printf("-Wextra: Enable some extra warning flags that are not enabled by -Wall.\n");
        }
        else if (hw3_strcmp(argv[i], werror) == 0)
        {
            printf("-Werror: Make all warnings into errors.\n");
        }
        else if (hw3_strcmp(argv[i], wpedantic) == 0)
        {
            printf("-Wpedantic: Issue all the warnings demanded by strict ISO C and ISO C++ and reject all programs that use forbidden extensions.\n");
        }
        else if (hw3_strcmp(argv[i], o) == 0)
        {
            printf("-o %s: Place the primary output in file %s.\n", argv[i+1], argv[i+1]);
            i++;
            continue;
        }
        else if (hw3_strcmp(argv[i], I) == 0)
        {
            printf("-I %s: Add the directory %s to the list of directories to be searched for header files during preprocessing.\n", argv[i+1], argv[i+1]);
            i++;
            continue;
        }
        
        else if (hw3_strncmp(argv[i], std1, 6) == 0)
        {
            if (hw3_strncmp(argv[i], std2, 8) == 0 )
            {
                if (hw3_isdigit(argv[i][8]) && hw3_isdigit(argv[i][9]))
                {
                    printf("%s: Set the language standard to ISO C++", argv[i]);
                    for (int j = 8;j < 10;j++)
                    {
                        printf("%c", argv[i][j]);
                    }
                    printf(".\n");
                }
            }
            else
            {
                if (hw3_isdigit(argv[i][6]) && hw3_isdigit(argv[i][7]))
                {
                    printf("%s: Set the language standard to ISO C", argv[i]);
                    for (int j = 6;j < 8;j++)
                    {
                        printf("%c", argv[i][j]);
                    }
                    printf(".\n");
                }
            }
        }
        else if (hw3_strncmp(argv[i], std3, 8) == 0)
        {
            if (hw3_strncmp(argv[i], std4, 10) == 0)
            {
                if (hw3_isdigit(argv[i][10]) && hw3_isdigit(argv[i][11]))
                {
                    printf("%s: Set the language standard to GNU dialect of C++", argv[i]);
                    for (int j = 10;j < 12;j++)
                    {
                        printf("%c", argv[i][j]);
                    }
                    printf(".\n");
                }
            }
            else
            {
                if (hw3_isdigit(argv[i][8]) && hw3_isdigit(argv[i][9]))
                {
                    printf("%s: Set the language standard to GNU dialect of C", argv[i]);
                    for (int j = 8;j < 10;j++)
                    {
                        printf("%c", argv[i][j]);
                    }
                    printf(".\n");
                }
            }
        }
        
        else
        {
            if (hw3_strmcmp(argv[i], other_input1, 2) == 0)
            {
                printf("%s: C source code as input file.\n", argv[i]);
            }
            else if (hw3_strmcmp(argv[i], other_input2, 2) == 0)
            {
                printf("%s: C/C++ header file as input file.\n", argv[i]);
            }
            else if (hw3_strmcmp(argv[i], other_input3, 4) == 0 || hw3_strmcmp(argv[i], other_input4, 2) == 0 || hw3_strmcmp(argv[i], other_input5, 3) == 0 || hw3_strmcmp(argv[i], other_input6, 4) == 0)
            {
                printf("%s: C++ source code as input file.\n", argv[i]);
            }
            else if (hw3_strmcmp(argv[i], other_input7, 4) == 0 || hw3_strmcmp(argv[i], other_input8, 4) == 0)
            {
                printf("%s: C++ header file as input file.\n", argv[i]);
            }
            else
            {
                
            }
        }
        
    }
    return 0;
}