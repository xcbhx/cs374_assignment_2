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
    printf("%s\n", argv[1]);
    FILE *file = fopen( argv[1], "r");

    if (file == NULL) {
        printf("Unable to open file\n");
        return 1;
    }

    return 0;
}