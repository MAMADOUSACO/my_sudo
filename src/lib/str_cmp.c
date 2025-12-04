/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** Compare two strings.
*/

int str_cmp(const char *str1, const char *str2)
{
    int i = 0;

    for (i = 0; str1[i] != 0 && str1[i] == str2[i]; i++);
    return str1[i] - str2[i];
}