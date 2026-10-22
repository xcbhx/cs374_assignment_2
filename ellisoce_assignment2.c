#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct movie {
    struct movie *next;
    char *title;
    int year;
    char language[5][21];
    float rating;
};

// Create a movie structure from one CSV line
struct movie *createMovie(char *line) {
    struct movie *newMovie = malloc(sizeof(struct movie));

    if (newMovie == NULL) {
        return NULL;
    }

    newMovie->next = NULL;
    newMovie->title = NULL;
    newMovie->year = 0;
    newMovie->rating = 0.0f;

    for (int i = 0; i < 5; i++) {
        newMovie->language[i][0] = '\0';
    }

    // Extract the title
    char *title = strtok(line, ",");

    // Extract the year
    char *year = strtok(NULL, ",");

    // Extract the languages
    char *languages = strtok(NULL, ",");

    // Extract the rating
    char *rating = strtok(NULL, ",");

    if (title == NULL || year == NULL ||
        languages == NULL || rating == NULL) {
        free(newMovie);
        return NULL;
    }

    // Store the title in separate memory
    newMovie->title = strdup(title);

    if (newMovie->title == NULL) {
        free(newMovie);
        return NULL;
    }

    newMovie->year = atoi(year);
    newMovie->rating = atof(rating);

    // Remove the opening and closing brackets
    char *start = strchr(languages, '[');
    char *end = strchr(languages, ']');

    if (start == NULL || end == NULL || end < start) {
        free(newMovie->title);
        free(newMovie);
        return NULL;
    }

    *end = '\0';
    start++;

    // Separate individual languages using semicolons
    char *language = strtok(start, ";");
    int index = 0;

    while (language != NULL && index < 5) {
        strncpy(newMovie->language[index], language, 20);
        newMovie->language[index][20] = '\0';

        index++;
        language = strtok(NULL, ";");
    }

    return newMovie;
}

// Read the CSV file and build the linked list
struct movie *processFile(FILE *file, int *movieCount) {
    char *line = NULL;
    size_t len = 0;

    struct movie *head = NULL;
    struct movie *tail = NULL;

    *movieCount = 0;

    // Skip the CSV header
    getline(&line, &len, file);

    while (getline(&line, &len, file) != -1) {
        struct movie *newMovie = createMovie(line);

        if (newMovie == NULL) {
            continue;
        }

        // First movie in the linked list
        if (head == NULL) {
            head = newMovie;
            tail = newMovie;
        } else {
            // Connect the new movie to the last movie
            tail->next = newMovie;

            // Update tail to the newest movie
            tail = newMovie;
        }

        (*movieCount)++;
    }

    free(line);

    return head;
}

// Display all movies released in a given year
void moviesByYear(struct movie *head) {
    int year;
    int found = 0;

    printf("Enter the year for which you want to see movies: ");
    if (scanf("%d", &year) != 1) {
        return;
    }

    struct movie *current = head;

    while (current != NULL) {
        if (current->year == year) {
            printf("%s\n", current->title);
            found = 1;
        }

        current = current->next;
    }

    if (!found) {
        printf("No data about movies released in the year %d\n", year);
    }
}

// Display the highest-rated movie for each year
void highestRatedByYear(struct movie *head) {
    struct movie *current = head;

    while (current != NULL) {
        int alreadyPrinted = 0;

        // Check whether this year appeared earlier
        struct movie *previous = head;

        while (previous != current) {
            if (previous->year == current->year) {
                alreadyPrinted = 1;
                break;
            }

            previous = previous->next;
        }

        if (!alreadyPrinted) {
            struct movie *highest = current;
            struct movie *search = current->next;

            // Find the highest-rated movie for this year
            while (search != NULL) {
                if (search->year == current->year &&
                    search->rating > highest->rating) {
                    highest = search;
                }

                search = search->next;
            }

            printf("%d %.1f %s\n",
                   highest->year,
                   highest->rating,
                   highest->title);
        }

        current = current->next;
    }
}

// Display movies available in a specified language
void moviesByLanguage(struct movie *head) {
    char language[100];
    int found = 0;

    printf("Enter the language for which you want to see movies: ");

    if (scanf("%99s", language) != 1) {
        return;
    }

    struct movie *current = head;

    while (current != NULL) {
        for (int i = 0; i < 5; i++) {
            if (strcmp(current->language[i], language) == 0) {
                printf("%d %s\n", current->year, current->title);
                found = 1;
                break;
            }
        }

        current = current->next;
    }

    if (!found) {
        printf("No data about movies released in %s\n", language);
    }
}

// Release memory allocated for the linked list
void freeMovies(struct movie *head) {
    struct movie *current = head;

    while (current != NULL) {
        struct movie *nextMovie = current->next;

        free(current->title);
        free(current);

        current = nextMovie;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("Unable to open file\n");
        return 1;
    }

    int movieCount = 0;

    struct movie *head = processFile(file, &movieCount);

    fclose(file);

    printf("Processed file %s and parsed data for %d movies\n",
           argv[1], movieCount);

    int choice = 0;

    do {
        printf("\n");
        printf("1. Show movies released in the specified year\n");
        printf("2. Show highest rated movie for each year\n");
        printf("3. Show the title and year of release of all movies in a specific language\n");
        printf("4. Exit from the program\n");
        printf("\nEnter a choice from 1 to 4: ");

        if (scanf("%d", &choice) != 1) {
            printf("You entered an incorrect choice. Try again.\n");

            // Clear invalid input
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }

            choice = 0;
            continue;
        }

        switch (choice) {
            case 1:
                moviesByYear(head);
                break;

            case 2:
                highestRatedByYear(head);
                break;

            case 3:
                moviesByLanguage(head);
                break;

            case 4:
                break;

            default:
                printf("You entered an incorrect choice. Try again.\n");
                break;
        }

    } while (choice != 4);

    freeMovies(head);

    return 0;
}