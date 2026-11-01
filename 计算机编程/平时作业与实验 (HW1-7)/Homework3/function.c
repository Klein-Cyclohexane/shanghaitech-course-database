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
int hw3_tolower(int ch)
{
    return hw3_isupper(ch) ? ch + 'a' - 'A' : ch;
}
int hw3_toupper(int ch)
{
    return hw3_islower(ch) ? ch + 'A' - 'a' : ch;
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

char* hw3_strchr(const char* str, int ch)
{
    while (*str && *str != ch)
    {
        str++;
    }
    return *str == ch ? (char*)str : NULL;
}
char* hw3_strcpy(char* dest, const char* src)
{
    char* ret = dest;
    while (*src)
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return ret;
}
char* hw3_strcat(char* dest, const char* src)
{
    char* ret = dest;
    while (*dest)
    {
        dest++;
    }
    while (*src)
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return ret;
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
int main() {
    const char *myString = "Hello, World!";
    char ch = 'l';
   
    char *result = hw3_strchr(myString, ch);
   
    if (result) {
        printf("The character '%c' is found at position: %ld\n", ch, result - myString);
    } else {
        printf("The character '%c' was not found in the string.\n", ch);
    }
   
    return 0;
}