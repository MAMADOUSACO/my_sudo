/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** Main header for my_sudo.
*/

#ifndef MY_SUDO_H
    #define MY_SUDO_H

    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <pwd.h>
    #include <grp.h>
    #include <shadow.h>
    #include <crypt.h>

    #define EXIT_SUCC 0
    #define EXIT_FAIL 84
    
    #define MAX_GROUPS 32
    #define MAX_PASSWORD_ATTEMPTS 3

typedef struct parsed_args {
    char *target_user;
    char *target_group;
    char **command;
} parsed_args_t;

int str_len(const char *str);
int str_cmp(const char *str1, const char *str2);
char *str_dup(const char *src);

int authenticate_user(char *username);
int check_sudoers_permission(char *username, uid_t uid,
    gid_t *groups, int ngroups);

#endif