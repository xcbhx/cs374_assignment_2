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
