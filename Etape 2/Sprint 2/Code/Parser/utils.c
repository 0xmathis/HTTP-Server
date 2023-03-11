#include "utils.h"

int startWith(const char *start, const char *ptr) {
    for (int i = 0; start[i] != '\0'; i++) {
        if (start[i] != ptr[i]) {
            return 0;
        }
    }

    return 1;
}
