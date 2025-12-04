/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** Permissions checking.
*/

#include "./../include/my_sudo.h"

static int check_group_alias(char *alias, gid_t my_gid)
{
    group_t *grp = parse_group_by_name(alias + 1);
    int result;

    if (grp == NULL)
        return 0;
    result = (grp->gid == my_gid);
    free(grp);
    return result;
}

int check_sudoers(const char *username, gid_t gid)
{
    FILE *f = fopen("/etc/sudoers", "r");
    char line[1024], alias[256];

    if (f == NULL)
        return 0;
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n')
            continue;
        sscanf(line, "%s", alias);
        if (str_cmp(alias, username) == 0) {
            fclose(f);
            return 1;
        }
        if (alias[0] == '%' && check_group_alias(alias, gid)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}