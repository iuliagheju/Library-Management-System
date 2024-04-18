#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library_management.h"
#include "library_management.c"
int main() {
    char name[255];
    char surname[255];

    // Prompt the user for their name and surname
    printf("Please enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // Remove newline character


    printf("Please enter your surname: ");
    fgets(surname, sizeof(surname), stdin);
    surname[strcspn(surname, "\n")] = 0;  // Remove newline character

    // Load data from CSV files
    read_users_csv(USERS_CSV);
    read_books_csv(BOOKS_CSV);
    read_loans_csv(LOANS_CSV);

    // Authenticate user
    if (authenticate_user(name, surname)) {
        //printf("Login failed. User not found.\n");
        return 1;  // Exit program if user not authenticated
    }
    // Authenticate user
    printf("Welcome %s %s\n", name, surname);
    User current_user;
    strcpy(current_user.name, name);
    strcpy(current_user.surname, surname);


    //printf("Welcome %s %s\n", current_user.name, current_user.surname);

    int choice;
    do {
        printf("\nLibrary Management System\n");
        printf("1. Borrow Books\n");
        printf("2. Return Books\n");
        printf("3. Donate Books\n");
        printf("4. View Loans\n");
        printf("5. Search Books\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                borrow_books(current_user);
                write_books_csv(BOOKS_CSV, books, book_count);
                write_loans_csv(LOANS_CSV, loans, loan_count);
                break;
            case 2:
                return_books(current_user);
                write_books_csv(BOOKS_CSV, books, book_count);
                write_loans_csv(LOANS_CSV, loans, loan_count);
                break;
            case 3:
                donate_books(current_user);
                write_books_csv(BOOKS_CSV, books, book_count);
                break;
            case 4:
                view_loans(current_user);
                break;
            case 5:
            {
                char title_search[255] = {0};
                char author_search[255] = {0};
                int skip_count = 0;

                printf("Enter book title to search (type 'skip' to skip): ");
                scanf(" %[^\n]", title_search);
                if (strcmp(title_search, "skip") == 0) {
                    skip_count++;
                }

                printf("Enter author name to search (type 'skip' to skip): ");
                scanf(" %[^\n]", author_search);
                if (strcmp(author_search, "skip") == 0) {
                    skip_count++;
                }

                if (skip_count == 2) {
                    printf("Search skipped by the user!\n");
                } else if (skip_count==0){
                    search_books(title_search, author_search);
                }else if (strcmp(title_search, "skip") == 0)
                    search_books_by_author(author_search);
                else
                    search_books_by_title(title_search);
            }
                break;

            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}
