/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** Main file.
*/

#include "./../include/my_sudo.h"

static int print_help(void)
{
    write(1, "Usage: my_sudo [-u user] [-g group] command\n", 45);
    return EXIT_FAIL;
}

static int parse_flags(int argc, char **argv, args_t *args)
{
    for (int i = 1; i < argc; i++) {
        if (str_cmp(argv[i], "-h") == 0)
            return 0;
        if (str_cmp(argv[i], "-u") == 0 && i + 1 < argc)
            args->target_user = argv[++i];
        if (str_cmp(argv[i], "-g") == 0 && i + 1 < argc)
            args->target_group = argv[++i];
        if (argv[i][0] != '-') {
            args->command = &argv[i];
            return 1;
        }
    }
    return 0;
}

static int set_identity(uid_t uid, gid_t gid)
{
    if (setregid(-1, gid) == -1)
        return 0;
    if (setreuid(-1, uid) == -1)
        return 0;
    return 1;
}

static gid_t get_target_gid(args_t *args, user_t *target_user)
{
    group_t *grp;
    gid_t result;

    if (args->target_group == NULL)
        return target_user->gid;
    grp = parse_group_by_name(args->target_group);
    if (grp == NULL)
        return target_user->gid;
    result = grp->gid;
    free(grp);
    return result;
}

static int verify_permissions(user_t *me)
{
    if (!check_sudoers(me->name, me->gid)) {
        write(2, me->name, str_len(me->name));
        write(2, " is not in the my_sudoers file.\n", 33);
        return 0;
    }
    return 1;
}

int main(int argc, char **argv)
{
    args_t args = {"root", NULL, NULL};
    user_t *me, *target;
    gid_t target_gid;

    if (argc < 2 || !parse_flags(argc, argv, &args))
        return print_help();
    me = parse_passwd_by_uid(getuid());
    if (!verify_permissions(me))
        return (free(me), EXIT_FAIL);
    if (!authenticate(me->name))
        return (free(me), EXIT_FAIL);
    target = parse_passwd_by_name(args.target_user);
    target_gid = get_target_gid(&args, target);
    if (!set_identity(target->uid, target_gid))
        return (free(me), free(target), EXIT_FAIL);
    free(me);
    free(target);
    execvp(args.command[0], args.command);
    return EXIT_FAIL;
}