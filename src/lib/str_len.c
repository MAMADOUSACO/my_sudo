/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** Get string length.
*/

int str_len(const char *str)
{
    int i = 0;

    for (i = 0; str[i] != 0; i++);
    return i;
}