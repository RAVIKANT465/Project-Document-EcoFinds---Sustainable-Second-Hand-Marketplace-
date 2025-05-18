#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

typedef struct {
    char username[MAX_LEN];
    char password[MAX_LEN];
} User;

typedef struct {
    char owner[MAX_LEN];
    char title[MAX_LEN];
    char description[MAX_LEN];
    char category[MAX_LEN];
    float price;
} Product;

void registerUser() {
    User user;
    FILE *fp = fopen("users.txt", "a");

    printf("Choose a username: ");
    scanf("%s", user.username);
    printf("Choose a password: ");
    scanf("%s", user.password);

    fprintf(fp, "%s %s\n", user.username, user.password);
    fclose(fp);
    printf("Registration successful!\n");
