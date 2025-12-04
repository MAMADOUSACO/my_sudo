/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** Main file.
*/

#include "./../include/my_sudo.h"

int print_help(void)
{
    write(1, "usage: ./my_sudo -h\n", 20);
    write(1, "usage: ./my_sudo [-ug] [command [args ...]]\n", 45);
    return EXIT_SUCC;
}

static void parse_single_arg(parsed_args_t *args, char **argv, int *i)
{
    if (str_cmp(argv[*i], "-u") == 0 && argv[*i + 1] != NULL) {
        args->target_user = argv[++(*i)];
        return;
    }
    if (str_cmp(argv[*i], "-g") == 0 && argv[*i + 1] != NULL) {
        args->target_group = argv[++(*i)];
        return;
    }
    args->command = &argv[*i];
}

static parsed_args_t *parse_arguments(int argc, char **argv)
{
    parsed_args_t *args = malloc(sizeof(parsed_args_t));
    int i;

    if (args == NULL)
        return NULL;
    args->target_user = NULL;
    args->target_group = NULL;
    args->command = NULL;
    for (i = 1; i < argc; i++) {
        parse_single_arg(args, argv, &i);
        if (args->command != NULL)
            break;
    }
    if (args->target_user == NULL)
        args->target_user = "root";
    return args;
}

static int set_target_identity(uid_t target_uid, gid_t target_gid)
{
    if (setregid(-1, target_gid) != 0)
        return EXIT_FAIL;
    if (setreuid(-1, target_uid) != 0)
        return EXIT_FAIL;
    return EXIT_SUCC;
}

static int verify_permissions(struct passwd *me)
{
    gid_t my_groups[MAX_GROUPS];
    int ngroups = MAX_GROUPS;

    getgrouplist(me->pw_name, me->pw_gid, my_groups, &ngroups);
    if (!check_sudoers_permission(me->pw_name, me->pw_uid,
        my_groups, ngroups)) {
        write(2, me->pw_name, str_len(me->pw_name));
        write(2, " is not in the my_sudoers file.\n", 33);
        return EXIT_FAIL;
    }
    return EXIT_SUCC;
}

static gid_t get_target_gid(parsed_args_t *args, struct passwd *target)
{
    if (args->target_group != NULL)
        return getgrnam(args->target_group)->gr_gid;
    return target->pw_gid;
}

int main(int argc, char **argv)
{
    parsed_args_t *args;
    struct passwd *me = getpwuid(getuid());
    struct passwd *target;
    gid_t target_gid;

    if (argc == 1 || (argc == 2 && str_cmp(argv[1], "-h") == 0))
        return print_help();
    args = parse_arguments(argc, argv);
    if (args == NULL || args->command == NULL)
        return EXIT_FAIL;
    if (verify_permissions(me) != EXIT_SUCC)
        return EXIT_FAIL;
    if (!authenticate_user(me->pw_name))
        return EXIT_FAIL;
    target = getpwnam(args->target_user);
    if (target == NULL)
        return EXIT_FAIL;
    target_gid = get_target_gid(args, target);
    if (set_target_identity(target->pw_uid, target_gid) != EXIT_SUCC)
        return EXIT_FAIL;
    execvp(args->command[0], args->command);
    return EXIT_FAIL;
}
