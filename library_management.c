#include "library_management.h"

void read_users_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening users file");
        exit(1);
    }

    char buffer[MAX_CSV_LINE_LENGTH];
    while (fgets(buffer, MAX_CSV_LINE_LENGTH, file)) {
        User user;
        // Assuming the CSV format is name,surname
        if (sscanf(buffer, "%[^,],%s", user.name, user.surname) == 2) {
            users[user_count++] = user;
        }
    }
    fclose(file);
}

void read_books_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening books file");
        exit(1);
    }

    char buffer[MAX_CSV_LINE_LENGTH];
    while (fgets(buffer, MAX_CSV_LINE_LENGTH, file)) {
        Book book;
        // Assuming the CSV format is title,author,copies
        if (sscanf(buffer, "%[^,],%[^,],%d", book.title, book.author, &book.copies) == 3) {
            books[book_count++] = book;
        }
    }

    fclose(file);
}

void read_loans_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening loans file");
        exit(1);
    }

    char buffer[MAX_CSV_LINE_LENGTH];
    while (fgets(buffer, MAX_CSV_LINE_LENGTH, file)) {
        Loan loan;
        // Assuming the CSV format is username,surname,title,author,copies_borrowed
        if (sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%d",
                   loan.user.name, loan.user.surname,
                   loan.book.title, loan.book.author,
                   &loan.copies_borrowed) == 5) {
            loans[loan_count++] = loan;
        }
    }

    fclose(file);
}

// Function to write books to the 'books.csv' file
void write_books_csv(const char *filename, Book *books, int book_count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < book_count; i++) {
        fprintf(file, "%s,%s,%d\n", books[i].title, books[i].author, books[i].copies);
    }
    fclose(file);
}

// Function to write loans to the 'loans.csv' file
void write_loans_csv(const char *filename, Loan *loans, int loan_count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < loan_count; i++) {
        fprintf(file, "%s,%s,%s,%s,%d\n",
                loans[i].user.name, loans[i].user.surname,
                loans[i].book.title, loans[i].book.author,
                loans[i].copies_borrowed);
    }
    fclose(file);
}

// Function for a user to borrow books
void borrow_books(User user) {
    printf("List of available books:\n");
    for (int i = 0; i < book_count; i++) {
        if (books[i].copies > 0) {
            printf("%d: %s by %s (%d copies available)\n", i + 1, books[i].title, books[i].author, books[i].copies);
        }
    }

    int book_index;
    printf("Enter the number of the book you wish to borrow: ");
    scanf("%d", &book_index);
    book_index--; // Adjust for 0-based index

    if (book_index >= 0 && book_index < book_count && books[book_index].copies > 0) {
        // Decrement the copies available
        books[book_index].copies--;

        // Record the loan
        Loan new_loan = {user, books[book_index], 1};
        loans[loan_count++] = new_loan;

        printf("You have borrowed '%s' by %s.\n", books[book_index].title, books[book_index].author);

        // Update the books.csv and loans.csv files
        write_books_csv("books.csv", books, book_count);
        write_loans_csv("loans.csv", loans, loan_count);
    } else {
        printf("Invalid selection or book not available.\n");
    }
}

// Function for a user to donate books
void donate_books(User user) {
    char title[255], author[255];
    int copies;

    printf("Enter the title of the book you wish to donate: ");
    scanf(" %[^\n]", title);

    printf("Enter the author of the book: ");
    scanf(" %[^\n]", author); // Same as above
    printf("Enter the number of copies you wish to donate: ");
    scanf("%d", &copies);

    // Add the new book to the array
    Book new_book = {"", "", copies};
    strcpy(new_book.title, title);
    strcpy(new_book.author, author);
    books[book_count++] = new_book;

    printf("Thank you for donating '%s' by %s.\n", title, author);

    // Update the books.csv file
    write_books_csv("books.csv", books, book_count);
}
void return_books(User user) {
    printf("List of books you have borrowed:\n");
    int has_loans = 0;
    for (int i = 0; i < loan_count; i++) {
        if (strcmp(loans[i].user.name, user.name) == 0 && strcmp(loans[i].user.surname, user.surname) == 0) {
            printf("%d: %s by %s\n", i + 1, loans[i].book.title, loans[i].book.author);
            has_loans = 1;
        }
    }

    if (!has_loans) {
        printf("You have no borrowed books to return.\n");
        return;
    }

    int loan_index;
    printf("Enter the number of the loan for the book you wish to return: ");
    scanf("%d", &loan_index);
    loan_index--; // Adjust for 0-based index

    if (loan_index >= 0 && loan_index < loan_count && strcmp(loans[loan_index].user.name, user.name) == 0 && strcmp(loans[loan_index].user.surname, user.surname) == 0) {
        // Increment the copies available in the books array
        for (int j = 0; j < book_count; j++) {
            if (strcmp(books[j].title, loans[loan_index].book.title) == 0 && strcmp(books[j].author, loans[loan_index].book.author) == 0) {
                books[j].copies += loans[loan_index].copies_borrowed;
                printf("You have returned '%s' by %s.\n", books[j].title, books[j].author);
                break;
            }
        }

        // Remove the loan entry by shifting the rest of the array down one spot
        for (int k = loan_index; k < loan_count - 1; k++) {
            loans[k] = loans[k + 1];
        }
        loan_count--;

        // Update the books.csv and loans.csv files
        write_books_csv("books.csv", books, book_count);
        write_loans_csv("loans.csv", loans, loan_count);
    } else {
        printf("Invalid selection.\n");
    }
}

int authenticate_user(const char *name, const char *surname) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].name, name) == 0 && strcmp(users[i].surname, surname) == 0) {
            // Debug print to confirm which user is authenticated
            printf("Authenticated User: %s %s\n", users[i].name, users[i].surname);
            return 1; // User found
        }
    }
    // If no user found, print this for debugging purposes
    //printf("No matching user found for: %s %s\n", name, surname);
    return 0; // User not found
}

void add_book_to_inventory(Book new_book) {
    if (book_count < MAX_BOOKS) {
        books[book_count++] = new_book;
    } else {
        printf("Inventory full, cannot add more books.\n");
    }
}

void add_loan(User user, Book borrowed_book, int copies) {
    if (loan_count < MAX_LOANS) {
        Loan new_loan = {user, borrowed_book, copies};
        loans[loan_count++] = new_loan;
    } else {
        printf("Loan system full, cannot process more loans.\n");
    }
}

void print_user_loans(User user) {
    int found = 0;
    for (int i = 0; i < loan_count; i++) {
        if (strcmp(loans[i].user.name, user.name) == 0 && strcmp(loans[i].user.surname, user.surname) == 0) {
            printf("%d: %s by %s - Copies borrowed: %d\n", i + 1, loans[i].book.title, loans[i].book.author, loans[i].copies_borrowed);
            found = 1;
        }
    }
    if (!found) {
        printf("No loans found for %s %s.\n", user.name, user.surname);
    }
}

void find_books(const char *search_query) {
    int found = 0;
    for (int i = 0; i < book_count; i++) {
        if (strstr(books[i].title, search_query) != NULL || strstr(books[i].author, search_query) != NULL) {
            printf("%d: %s by %s - Copies available: %d\n", i + 1, books[i].title, books[i].author, books[i].copies);
            found = 1;
        }
    }
    if (!found) {
        printf("No books found matching '%s'.\n", search_query);
    }
}

// Function to view all loans made by a user
void view_loans(User user) {
    int found = 0;
    printf("Loans for %s %s:\n", user.name, user.surname);
    for (int i = 0; i < loan_count; i++) {
        if (strcmp(loans[i].user.name, user.name) == 0 && strcmp(loans[i].user.surname, user.surname) == 0) {
            printf("Book: %s by %s, Copies Borrowed: %d\n",
                   loans[i].book.title, loans[i].book.author, loans[i].copies_borrowed);
            found = 1;
        }
    }
    if (!found) {
        printf("No loans found for this user.\n");
    }
}

// Function to search books by title and/or author
void search_books(const char *title, const char *author) {
    int found = 0;
    printf("Search results:\n");
    for (int i = 0; i < book_count; i++) {
        if ((title == NULL || strstr(books[i].title, title) != NULL) &&
            (author == NULL || strstr(books[i].author, author) != NULL)) {
            printf("Book: %s by %s, Copies Available: %d\n",
                   books[i].title, books[i].author, books[i].copies);
            found = 1;
        }
    }
    if (!found) {
        printf("No books found matching the search criteria.\n");
    }
}

void search_books_by_title(const char *title) {
        int found = 0;
        printf("Search results for title '%s':\n", title);
        for (int i = 0; i < book_count; i++) {
        if (strstr(books[i].title, title) != NULL) {
            printf("Book: %s by %s, Copies Available: %d\n",
                   books[i].title, books[i].author, books[i].copies);
            found = 1;
        }
    }
    if (!found) {
        printf("No books found matching the title.\n");
    }
}

void search_books_by_author(const char *author) {
    int found = 0;
    printf("Search results for author '%s':\n", author);
    for (int i = 0; i < book_count; i++) {
        if (strstr(books[i].author, author) != NULL) {
            printf("Book: %s by %s, Copies Available: %d\n",
                   books[i].title, books[i].author, books[i].copies);
            found = 1;
        }
    }
    if (!found) {
        printf("No books found matching the author.\n");
    }
}

void remove_loan(User user, Book returned_book) {
    for (int i = 0; i < loan_count; i++) {
        if (strcmp(loans[i].user.name, user.name) == 0 &&
            strcmp(loans[i].user.surname, user.surname) == 0 &&
            strcmp(loans[i].book.title, returned_book.title) == 0 &&
            strcmp(loans[i].book.author, returned_book.author) == 0) {

            // Found the loan, remove it by shifting the rest of the array up
            for (int j = i; j < loan_count - 1; j++) {
                loans[j] = loans[j + 1];
            }
            loan_count--;  // Decrement the total number of loans

            printf("Loan removed for '%s' by %s.\n", returned_book.title, returned_book.author);
            return;
        }
    }
    printf("Loan not found for '%s' by %s.\n", returned_book.title, returned_book.author);
}
