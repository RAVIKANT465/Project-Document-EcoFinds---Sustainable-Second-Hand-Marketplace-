#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100
#define USERS_FILE "users.txt"
#define PRODUCTS_FILE "products.txt"

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
    FILE *fp = fopen(USERS_FILE, "a");

    if (!fp) {
        perror("Failed to open users file");
        return;
    }

    printf("Choose a username: ");
    scanf("%s", user.username);
    printf("Choose a password: ");
    scanf("%s", user.password);

    fprintf(fp, "%s %s\n", user.username, user.password);
    fclose(fp);
    printf("Registration successful!\n");
}

int loginUser(char *username) {
    char u[MAX_LEN], p[MAX_LEN], inputPass[MAX_LEN];
    FILE *fp = fopen(USERS_FILE, "r");

    if (!fp) {
        perror("Failed to open users file");
        return 0;
    }

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", inputPass);

    while (fscanf(fp, "%s %s", u, p) != EOF) {
        if (strcmp(username, u) == 0 && strcmp(inputPass, p) == 0) {
            fclose(fp);
            printf("Login successful!\n");
            return 1;
        }
    }

    fclose(fp);
    printf("Invalid credentials.\n");
    return 0;
}

void createListing(const char *owner) {
    Product product;
    FILE *fp = fopen(PRODUCTS_FILE, "a");

    if (!fp) {
        perror("Failed to open products file");
        return;
    }

    strcpy(product.owner, owner);

    printf("Title: ");
    getchar();  // clear buffer
    fgets(product.title, MAX_LEN, stdin);
    product.title[strcspn(product.title, "\n")] = 0;

    printf("Description: ");
    fgets(product.description, MAX_LEN, stdin);
    product.description[strcspn(product.description, "\n")] = 0;

    printf("Category: ");
    fgets(product.category, MAX_LEN, stdin);
    product.category[strcspn(product.category, "\n")] = 0;

    printf("Price: ");
    scanf("%f", &product.price);

    fprintf(fp, "%s|%s|%s|%s|%.2f\n", product.owner, product.title, product.description, product.category, product.price);
    fclose(fp);
    printf("Product listed successfully!\n");
}

void viewListings() {
    Product product;
    FILE *fp = fopen(PRODUCTS_FILE, "r");

    if (!fp) {
        printf("No listings yet.\n");
        return;
    }

    printf("\n--- All Listings ---\n");
    while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%f\n", product.owner, product.title, product.description, product.category, &product.price) != EOF) {
        printf("\nSeller: %s\nTitle: %s\nDescription: %s\nCategory: %s\nPrice: $%.2f\n",
               product.owner, product.title, product.description, product.category, product.price);
    }

    fclose(fp);
}

void searchListings() {
    char keyword[MAX_LEN];
    Product product;
    FILE *fp = fopen(PRODUCTS_FILE, "r");

    if (!fp) {
        printf("No listings found.\n");
        return;
    }

    printf("Enter keyword to search: ");
    getchar();  // clear buffer
    fgets(keyword, MAX_LEN, stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    printf("\n--- Search Results ---\n");
    int found = 0;
    while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%f\n", product.owner, product.title, product.description, product.category, &product.price) != EOF) {
        if (strstr(product.title, keyword) || strstr(product.description, keyword)) {
            printf("\nSeller: %s\nTitle: %s\nDescription: %s\nCategory: %s\nPrice: $%.2f\n",
                   product.owner, product.title, product.description, product.category, product.price);
            found = 1;
        }
    }

    if (!found) {
        printf("No listings match your search.\n");
    }

    fclose(fp);
}

int main() {
    int choice, loggedIn = 0;
    char currentUser[MAX_LEN];

    while (1) {
        printf("\n=== EcoFinds Terminal Marketplace ===\n");
        printf("1. Register\n2. Login\n3. Create Listing\n4. View Listings\n5. Search Listings\n6. Exit\n");
        printf("Select option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loggedIn = loginUser(currentUser);
                break;
            case 3:
                if (loggedIn)
                    createListing(currentUser);
                else
                    printf("Please log in first.\n");
                break;
            case 4:
                viewListings();
                break;
            case 5:
                searchListings();
                break;
            case 6:
                printf("Exiting EcoFinds. Bye!\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

