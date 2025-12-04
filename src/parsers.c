/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** File parsers.
*/

#include "./../include/my_sudo.h"

user_t *parse_passwd_by_uid(uid_t uid)
{
    FILE *f = fopen("/etc/passwd", "r");
    char line[1024];
    user_t *user = malloc(sizeof(user_t));
    int file_uid;

    if (f == NULL || user == NULL)
        return NULL;
    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%[^:]:%*[^:]:%d:%d:", user->name, &file_uid, &user->gid);
        if ((uid_t)file_uid == uid) {
            user->uid = uid;
            fclose(f);
            return user;
        }
    }
    fclose(f);
    free(user);
    return NULL;
}

user_t *parse_passwd_by_name(const char *name)
{
    FILE *f = fopen("/etc/passwd", "r");
    char line[1024], file_name[256];
    user_t *user = malloc(sizeof(user_t));

    if (f == NULL || user == NULL)
        return NULL;
    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%[^:]:%*[^:]:%d:%d:", file_name, &user->uid, &user->gid);
        if (str_cmp(file_name, name) == 0) {
            for (int i = 0; name[i]; i++)
                user->name[i] = name[i], user->name[i + 1] = '\0';
            fclose(f);
            return user;
        }
    }
    fclose(f);
    free(user);
    return NULL;
}

group_t *parse_group_by_name(const char *name)
{
    FILE *f = fopen("/etc/group", "r");
    char line[1024], file_name[256];
    group_t *grp = malloc(sizeof(group_t));

    if (f == NULL || grp == NULL)
        return NULL;
    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%[^:]:%*[^:]:%d:", file_name, &grp->gid);
        if (str_cmp(file_name, name) == 0) {
            for (int i = 0; name[i]; i++)
                grp->name[i] = name[i], grp->name[i + 1] = '\0';
            fclose(f);
            return grp;
        }
    }
    fclose(f);
    free(grp);
    return NULL;
}

char *parse_shadow_hash(const char *username)
{
    FILE *f = fopen("/etc/shadow", "r");
    char line[1024], file_user[256], hash[256];

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