/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** Authentication.
*/

#include "./../include/my_sudo.h"

static char *read_password(void)
{
    char *pwd = malloc(256);
    int i = 0, r;

    if (pwd == NULL)
        return NULL;
    while ((r = read(0, &pwd[i], 1)) > 0 && pwd[i] != '\n' && i < 255)
        i++;
    pwd[i] = '\0';
    return pwd;
}

static int try_auth(const char *hash)
{
    char *input = read_password();
    char *result;
    int match;

    if (input == NULL)
        return 0;
    result = crypt(input, hash);
    match = str_cmp(result, hash) == 0;
    free(input);
    return match;
}

int authenticate(const char *username)
{
    char *hash = parse_shadow_hash(username);
    char prompt[256];
    int attempt;

    if (hash == NULL)
        return 0;
    for (attempt = 0; attempt < 3; attempt++) {
        sprintf(prompt, "[my_sudo] password for %s: ", username);
        write(1, prompt, str_len(prompt));
        if (try_auth(hash)) {
            free(hash);
            return 1;
        }
        if (attempt < 2)
            write(1, "Sorry, try again.\n", 18);
    }
    free(hash);
    return 0;
}