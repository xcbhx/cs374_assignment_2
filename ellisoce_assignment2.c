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

int main(int argc, char *argv[]) {
    FILE *file = fopen( argv[1], "r");

    if (file == NULL) {
        printf("Unable to open file\n");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;

    int movieCount = 0;

    // Read and skip the header
    getline(&line, &len, file);

    while (getline(&line, &len, file) != -1) {
        printf("%s", line);

        movieCount++;
    }
    printf("%d\n", movieCount);

    free(line);
    fclose(file);

    return 0;
}