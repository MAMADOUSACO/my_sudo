/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** Authentication module.
*/

#include "./../include/my_sudo.h"

static char *get_shadow_hash(const char *username)
{
    FILE *f = fopen("/etc/shadow", "r");
    char line[1024];
    char file_user[256];
    char hash[512];

    if (f == NULL)
        return NULL;
    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%[^:]:%[^:]:", file_user, hash);
        if (str_cmp(file_user, username) == 0) {
            fclose(f);
            return str_dup(hash);
        }
    }
    fclose(f);
    return NULL;
}

static char *read_password(void)
{
    char *password = malloc(256);
    char buffer;
    int i = 0;

    if (password == NULL)
        return NULL;
    while (read(0, &buffer, 1) == 1 && buffer != '\n' && i < 255) {
        password[i++] = buffer;
    }
    password[i] = 0;
    return password;
}

static void prompt_password(char *username)
{
    write(1, "[my_sudo] password for ", 23);
    write(1, username, str_len(username));
    write(1, ": ", 2);
}

static int try_authenticate(char *shadow_hash)
{
    char *input_password = read_password();
    char *hashed;

    if (input_password == NULL)
        return 0;
    hashed = crypt(input_password, shadow_hash);
    free(input_password);
    if (str_cmp(hashed, shadow_hash) == 0)
        return 1;
    return 0;
}

int authenticate_user(char *username)
{
    char *shadow_hash = get_shadow_hash(username);
    int attempt;

    if (shadow_hash == NULL)
        return 0;
    for (attempt = 0; attempt < MAX_PASSWORD_ATTEMPTS; attempt++) {
        prompt_password(username);
        if (try_authenticate(shadow_hash)) {
            free(shadow_hash);
            return 1;
        }
        if (attempt < MAX_PASSWORD_ATTEMPTS - 1)
            write(2, "Sorry, try again.\n", 18);
    }
    free(shadow_hash);
    return 0;
}
