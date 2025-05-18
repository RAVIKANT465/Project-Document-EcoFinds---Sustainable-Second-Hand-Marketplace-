SUMMARISATION OF WHOLE SCRIPT.
    
APP INFO (ecofinds_gui) 
    
This is a simple graphical desktop application created using GTK in C , which allows users to -

Register and log in with a username and password.

Create product listings with details like title, description, category, and price.

View all product listings submitted by any user.

All data is stored in simple text files.

1. User Registration and Login
Users can register by entering a username and password, which are saved in a file called users.txt.

They can log in by entering the same credentials.

If login is successful, the app displays a confirmation dialog.

2. Creating Product Listings
After logging in, users can:

Enter a product title

Write a short description

Choose a category

Set a price

3. Viewing Listings
A "View Listings" tab shows all product listings.

Listings are read from products.txt and displayed in a text area.

Click the "Refresh Listings" button to reloads the latest entries.
    
4.Graphical User Interface (GUI)
The app has a clean GTK window with 3 tabs:

Login/Register

Create Listing

View Listings

Input fields, buttons, and text views are neatly arranged using GTK's grid layout.




#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 100

char currentUser[MAX_LEN] = "";
int loggedIn = 0;



void register_user(const char *username, const char *password) {
    FILE *fp = fopen("users.txt", "a");
    if (fp) {
        fprintf(fp, "%s %s\n", username, password);
        fclose(fp);
    }
}

int login_user(const char *username, const char *password) {
    char u[MAX_LEN], p[MAX_LEN];
    FILE *fp = fopen("users.txt", "r");
    if (!fp) return 0;

    while (fscanf(fp, "%s %s", u, p) != EOF) {
        if (strcmp(username, u) == 0 && strcmp(password, p) == 0) {
            fclose(fp);
            strcpy(currentUser, username);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void create_listing(const char *title, const char *desc, const char *cat, float price) {
    FILE *fp = fopen("products.txt", "a");
    if (fp) {
        fprintf(fp, "%s|%s|%s|%s|%.2f\n", currentUser, title, desc, cat, price);
        fclose(fp);
    }
}

void view_listings(GtkTextBuffer *buffer) {
    FILE *fp = fopen("products.txt", "r");
    if (!fp) return;

    char owner[MAX_LEN], title[MAX_LEN], desc[MAX_LEN], cat[MAX_LEN];
    float price;
    char line[512] = "";

    gtk_text_buffer_set_text(buffer, "", -1);

    while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%f\n", owner, title, desc, cat, &price) != EOF) {
        sprintf(line, "Seller: %s\nTitle: %s\nDesc: %s\nCategory: %s\nPrice: $%.2f\n\n", owner, title, desc, cat, price);
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, line, -1);
    }
    fclose(fp);
}

// ---------- GTK UI Handlers ----------

void on_login_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = (GtkEntry **)data;
    const char *username = gtk_entry_get_text(entries[0]);
    const char *password = gtk_entry_get_text(entries[1]);

    if (login_user(username, password)) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Login successful!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Invalid credentials!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_register_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = (GtkEntry **)data;
    const char *username = gtk_entry_get_text(entries[0]);
    const char *password = gtk_entry_get_text(entries[1]);
    register_user(username, password);

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "User registered!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_create_listing_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = (GtkEntry **)data;

    const char *title = gtk_entry_get_text(entries[0]);
    const char *desc = gtk_entry_get_text(entries[1]);
    const char *cat = gtk_entry_get_text(entries[2]);
    float price = atof(gtk_entry_get_text(entries[3]));

    create_listing(title, desc, cat, price);

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Product listed!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_view_listings(GtkWidget *widget, gpointer data) {
    GtkTextBuffer *buffer = GTK_TEXT_BUFFER(data);
    view_listings(buffer);
}

// main function

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window, *grid, *notebook;
    GtkWidget *entry_username, *entry_password;
    GtkWidget *entry_title, *entry_desc, *entry_cat, *entry_price;
    GtkWidget *text_view;
    GtkTextBuffer *text_buffer;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "EcoFinds GUI");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window), notebook);

    // Login/Register Tab
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    entry_username = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_username), "Username");
    entry_password = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_password), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(entry_password), FALSE);

    GtkEntry *login_entries[] = {GTK_ENTRY(entry_username), GTK_ENTRY(entry_password)};

    GtkWidget *btn_login = gtk_button_new_with_label("Login");
    GtkWidget *btn_register = gtk_button_new_with_label("Register");

    g_signal_connect(btn_login, "clicked", G_CALLBACK(on_login_clicked), login_entries);
    g_signal_connect(btn_register, "clicked", G_CALLBACK(on_register_clicked), login_entries);

    gtk_grid_attach(GTK_GRID(grid), entry_username, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_password, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_login, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_register, 1, 2, 1, 1);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), grid, gtk_label_new("Login/Register"));

    // Create Listing Tab
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    entry_title = gtk_entry_new();
    entry_desc = gtk_entry_new();
    entry_cat = gtk_entry_new();
    entry_price = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_title), "Title");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_desc), "Description");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_cat), "Category");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_price), "Price");

    GtkEntry *listing_entries[] = {GTK_ENTRY(entry_title), GTK_ENTRY(entry_desc), GTK_ENTRY(entry_cat), GTK_ENTRY(entry_price)};
    GtkWidget *btn_create = gtk_button_new_with_label("Create Listing");
    g_signal_connect(btn_create, "clicked", G_CALLBACK(on_create_listing_clicked), listing_entries);

    gtk_grid_attach(GTK_GRID(grid), entry_title, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_desc, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_cat, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_price, 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_create, 0, 4, 2, 1);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), grid, gtk_label_new("Create Listing"));

    // View Listings Tab
    grid = gtk_grid_new();
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    text_view = gtk_text_view_new();
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);

    GtkWidget *btn_view = gtk_button_new_with_label("Refresh Listings");
    g_signal_connect(btn_view, "clicked", G_CALLBACK(on_view_listings), text_buffer);

    gtk_grid_attach(GTK_GRID(grid), btn_view, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), text_view, 0, 1, 1, 1);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), grid, gtk_label_new("View Listings"));

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
