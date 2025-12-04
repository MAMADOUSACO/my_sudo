/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** Duplicate a string.
*/

#include "./../../include/my_sudo.h"

char *str_dup(const char *src)
{
    char *dest;
    int len = str_len(src);
    int i;

    dest = malloc(sizeof(char) * (len + 1));
    if (dest == NULL)
        return NULL;
    for (i = 0; src[i] != 0; i++) {
        dest[i] = src[i];
    }
    dest[i] = 0;
    return dest;
}