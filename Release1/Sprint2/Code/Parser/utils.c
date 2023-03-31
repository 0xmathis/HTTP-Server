#include "utils.h"

int startWith(const char *start, const char *ptr) {
    for (int i = 0; start[i] != '\0'; i++) {
        if (start[i] != ptr[i]) {
            return 0;
        }
    }

    return 1;
}

int isIn(const char *string, const char ptr) {
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == ptr) {
            return 1;
        }
    }

    return 0;
}
