#ifndef LIBRARY_MANAGEMENT_H
#define LIBRARY_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CSV_LINE_LENGTH 1024
#define MAX_BOOKS 100
#define MAX_USERS 100
#define MAX_LOANS 100

#define USERS_CSV "users.csv"
#define BOOKS_CSV "books.csv"
#define LOANS_CSV "loans.csv"

typedef struct {
    char title[255];
    char author[255];
    int copies;
} Book;

typedef struct {
    char name[255];
    char surname[255];
} User;

typedef struct {
    User user;
    Book book;
    int copies_borrowed;
} Loan;

// Global variables to hold the data
Book books[MAX_BOOKS];
int book_count = 0;
User users[MAX_USERS];
int user_count = 0;
Loan loans[MAX_LOANS];
int loan_count = 0;

// Function prototypes
void borrow_books(User user);
void return_books(User user);
void donate_books(User user);
void view_loans(User user);
void search_books(const char *title, const char *author);
void search_books_by_title(const char *title);
void search_books_by_author(const char *author);

// CSV interaction functions
void read_users_csv(const char *filename);
void read_books_csv(const char *filename);
void write_books_csv(const char *filename, Book *books, int book_count);
void read_loans_csv(const char *filename);
void write_loans_csv(const char *filename, Loan *loans, int loan_count);

// Helper functions
int authenticate_user(const char *name, const char *surname);
void add_book_to_inventory(Book new_book);
void add_loan(User user, Book borrowed_book, int copies);
void remove_loan(User user, Book returned_book);
void print_user_loans(User user);
void find_books(const char *search_query);

#endif
