
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 10

char *safestr(char str[]);

typedef struct {
    char *name;
    char *author;
    int status;
} books;

typedef struct {
    int books_count;
    int books_read_count;
} libraries;

int add_books(books **library, int total);
int menu_logic(int menu_option, books **library, int total, libraries *general_library);

int main() {
    int total = 0;
    int menu_option = 0;

    books *library = calloc(DEFAULT_SIZE, sizeof(books));
    libraries general_library = {0};

    if (library == NULL) {
        return 1;
    }

    general_library.books_count = total;

    while (1) {
        printf("\nmenu:\n");
        printf("[0] exit\n");
        printf("[1] add books\n");
        printf("[2] see books\n");
        printf("[3] see how many books you got\n");
        printf("[4] change book status\n");
        printf("[5] Books read\n");
        printf("[6] books not read\n");
        printf("[7] see how many books you have read so far\n");
        printf("[8]search for a book just type the book name\n");
        printf("\n");

        if (scanf("%d", &menu_option) != 1) {
            break;
        }

        if (menu_option == -1) {
            break;
        }

        total = menu_logic(menu_option, &library, total, &general_library);
    }

    for (int i = 0; i < total; i++) {
        free(library[i].name);
        free(library[i].author);
    }

    free(library);

    return 0;
}

int menu_logic(int menu_option, books **library, int total, libraries *general_library) {

    switch (menu_option) {

        case 1:
            total = add_books(library, total);
            general_library->books_count = total;
            break;

        case 2:
            for (int i = 0; i < total; i++) {
                printf("[%d] %s by %s\n",
                       i + 1, (*library)[i].name, (*library)[i].author);
            }
            break;

        case 3:
            printf("you have %d books\n", general_library->books_count);
            break;

        case 4: {
            int book_selected = 0;
            int book_status = 0;

            printf("what book have you read pick it by the book number to see the books number enter -1\n press enter then 2: ");
            scanf("%d", &book_selected);

            if (book_selected == -1) {
                break;
            }

            printf("enter 1 if the book is read and 0 if its not: ");
            scanf("%d", &book_status);

            book_selected--;

            // Update the read count only when the status changes
            if (book_status == 1 && (*library)[book_selected].status == 0) {
                general_library->books_read_count++;

            } else if (book_status == 0 && (*library)[book_selected].status == 1) {
                general_library->books_read_count--;
            }

            (*library)[book_selected].status = book_status;

            printf("you have changed [%d] %s by %s status successfully\n",
                   book_selected + 1,
                   (*library)[book_selected].name,
                   (*library)[book_selected].author);

            break;
        }

        case 5:
            for (int i = 0; i < total; i++) {

                if ((*library)[i].status == 1) {
                    printf("you have read [%d] %s by %s\n",
                           i + 1, (*library)[i].name, (*library)[i].author);

                } else {
                    printf("you have not read any book\n");
                }
            }
            break;

        case 6:
            for (int i = 0; i < total; i++) {

                if ((*library)[i].status == 0) {
                    printf("you have not read [%d] %s by %s\n",
                           i + 1, (*library)[i].name, (*library)[i].author);
                }
            }
            break;

        case 7:
            printf("you have read %d books so far\n", general_library->books_read_count);
            break;

        case 8: {
            int size = 100;
            char *bookname = calloc(size, sizeof(char));
            char onechar;

            if (bookname == NULL) {
                printf("calloc failed\n");
                break;
            }

            printf("enter the book name: ");

            for (int i = 0;; i++) {
                scanf("%c", &onechar);

                if (onechar == '\n') {
                    bookname[i] = '\0';
                    break;

                } else {
                    if (i >= size - 1) {
                        char *temp = realloc(bookname, size + 10);

                        if (temp == NULL) {
                            printf("realloc failed\n");
                            break;
                        }

                        bookname = temp;
                        size += 10;
                    }

                    bookname[i] = onechar;
                    bookname[i + 1] = '\0';
                }
            }

            for (int i = 0; i < total; i++) {
                if (strcmp(bookname, (*library)[i].name) == 0) {
                    printf("your book is here at this number [%d] %s by %s ",
                           i + 1, (*library)[i].name, (*library)[i].author);

                    if ((*library)[i].status == 1) {
                        printf("you have read it already\n");
                    } else {
                        printf("you havent read it yet\n");
                    }
                }
            }
            free(bookname);
            break;
        }
    }

    return total;
}

int add_books(books **library, int total) {
    int many_books = 0;
    int control = 0;
    int capacity = DEFAULT_SIZE;
    char choice;

    while (1) {
        printf("\nhow many books do you wanna add: ");

        if (scanf("%d", &many_books) != 1 || many_books <= 0) {
            break;
        }

        int ch;

        while ((ch = getchar()) != '\n' && ch != EOF) {
        }

        for (control = 0; control < many_books; control++) {

            if (total >= capacity) {
                books *temp = realloc(*library, (capacity + 1) * sizeof(books));

                if (temp == NULL) {
                    printf("realloc failed\n");
                    return total;
                }

                *library = temp;
                capacity++;
            }

            // Allocate memory for the book name
            (*library)[total].name = calloc(DEFAULT_SIZE + 1, sizeof(char));

            if ((*library)[total].name == NULL) {
                return total;
            }

            printf("enter the book name: ");
            (*library)[total].name = safestr((*library)[total].name);

            // Allocate memory for the author
            (*library)[total].author = calloc(DEFAULT_SIZE + 1, sizeof(char));

            if ((*library)[total].author == NULL) {
                free((*library)[total].name);
                (*library)[total].name = NULL;
                return total;
            }

            printf("enter the book author: ");
            (*library)[total].author = safestr((*library)[total].author);

            total++;
        }

        printf("wanna add more books? (y/q): ");

        if (scanf(" %c", &choice) != 1) {
            break;
        }

        if (choice == 'q') {
            break;

        } else if (choice == 'y') {
            continue;

        } else {
            break;
        }
    }

    return total;
}

char *safestr(char *str) {
    char c;
    int i = 0;
    int size = DEFAULT_SIZE + 1;

    while (scanf("%c", &c) == 1) {

        if (i >= size - 1) {
            char *temp = realloc(str, size + DEFAULT_SIZE);

            if (temp == NULL) {
                str[i] = '\0';
                return str;
            }

            str = temp;
            size += DEFAULT_SIZE;
        }

        if (c == '\n') {
            str[i] = '\0';
            break;

        } else if (i < size - 1) {
            str[i] = c;
            i++;
        }
    }

    str[i] = '\0';
    return str;
}

