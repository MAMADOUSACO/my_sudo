/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** Permissions module.
*/

#include "./../include/my_sudo.h"

static int is_gid_in_groups(gid_t target, gid_t *groups, int count)
{
    for (int i = 0; i < count; i++) {
        if (groups[i] == target)
            return 1;
    }
    return 0;
}

static int check_group_alias(char *alias, gid_t *groups, int ngroups)
{
    struct group *grp = getgrnam(alias + 1);

    if (grp == NULL)
        return 0;
    return is_gid_in_groups(grp->gr_gid, groups, ngroups);
}

static int check_gid_alias(char *alias, gid_t *groups, int ngroups)
{
    gid_t target_gid = atoi(alias + 2);

    return is_gid_in_groups(target_gid, groups, ngroups);
}

static int check_sudoers_line(char *line, char *username, uid_t uid,
    gid_t *groups, int ngroups)
{
    char user_alias[256];

    if (line[0] == '#' || line[0] == '\n')
        return 0;
    sscanf(line, "%s", user_alias);
    if (str_cmp(user_alias, username) == 0)
        return 1;
    if (user_alias[0] == '%' && user_alias[1] != '#')
        return check_group_alias(user_alias, groups, ngroups);
    if (user_alias[0] == '#' && user_alias[1] != '%')
        return ((uid_t)atoi(user_alias + 1) == uid);
    if (user_alias[0] == '%' && user_alias[1] == '#')
        return check_gid_alias(user_alias, groups, ngroups);
    return 0;
}

int check_sudoers_permission(char *username, uid_t uid,
    gid_t *groups, int ngroups)
{
    FILE *f = fopen("/etc/sudoers", "r");
    char line[1024];

    if (f == NULL)
        return 0;
    while (fgets(line, sizeof(line), f)) {
        if (check_sudoers_line(line, username, uid, groups, ngroups)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}
