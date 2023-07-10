#include "minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }

    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

char *ft_strchr(const char *s, int c)
{
    while (*s && *s != (char)c)
        s++;

    if (*s == (char)c)
        return (char *)s;
    else
        return NULL;
}

char *ft_strpbrk(const char *s, const char *charset)
{
    while (*s)
    {
        if (ft_strchr(charset, *s) != NULL)
            return (char *)s;
        s++;
    }

    return NULL;
}
