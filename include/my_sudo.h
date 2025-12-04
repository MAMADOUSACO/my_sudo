/*
** EPITECH PROJECT, 2025
** my_sudo
** File description:
** Header file.
*/

#ifndef MY_SUDO_H
#define MY_SUDO_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <crypt.h>

#define EXIT_SUCC 0
#define EXIT_FAIL 84

typedef struct {
    char name[256];
    uid_t uid;
    gid_t gid;
} user_t;

typedef struct {
    char name[256];
    gid_t gid;
} group_t;

typedef struct {
    char *target_user;
    char *target_group;
    char **command;
} args_t;

user_t *parse_passwd_by_uid(uid_t uid);
user_t *parse_passwd_by_name(const char *name);
group_t *parse_group_by_name(const char *name);
char *parse_shadow_hash(const char *username);

int check_sudoers(const char *username, gid_t gid);

int authenticate(const char *username);

int str_cmp(const char *s1, const char *s2);
int str_len(const char *str);
char *str_dup(const char *str);

#endif