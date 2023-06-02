#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/api.h"
#include "../include/others.h"
#include "../include/getters.h"
#include "../include/request.h"
#include "../include/utils.h"
#include <unistd.h>

int isGet() {
    char *method = getHeaderValue(root, "method");

    if (strcmp(method, "GET") == 0) {
        free(method);
        return 1;
    }

    free(method);
    return 0;
}

int isHead() {
    char *method = getHeaderValue(root, "method");

    if (strcmp(method, "HEAD") == 0) {
        free(method);
        return 1;
    }

    free(method);
    return 0;
}

int isPost() {
    char *method = getHeaderValue(root, "method");

    if (strcmp(method, "POST") == 0) {
        free(method);
        return 1;
    }

    free(method);
    return 0;
}

int isPHP() {
    char extension[50];

    getFileExtension(getFilePath(), extension);

    if (strcmp(extension, ".php") == 0) {
        return 1;
    }

    return 0;
}

int isStreamable(char *mimeType) {
    return startWith("video", mimeType) || startWith("audio", mimeType);
}

char *percentEncodings(char *path) {
    // printf("Removing percent encodings\n");

    int len = strlen(path);
    char *newpath = (char *) malloc(len * sizeof(char));
    int j = 0;

    for (int i = 0; i < len + 1; i++) {
        if (path[i] == '%' && i + 2 < len) {
            char hex[3];
            hex[0] = path[i + 1];
            hex[1] = path[i + 2];
            hex[2] = '\0';

            int asciiCode;
            sscanf(hex, "%x", &asciiCode);

            newpath[j] = (char) asciiCode;
            i += 2;
        } else {
            newpath[j] = path[i];
        }
        j++;
    }

    return newpath;
}

char *getPWD() {
    char *pwd = (char *) malloc(sizeof(char) * 100);

    if (getcwd(pwd, 100) != NULL) {
        return pwd;
    }

    free(pwd);
    return NULL;
}

void remove_dot_segments(char *path) {
    // printf("Removing dot segments\n");

    for (int i = 0; i < (int) strlen(path) - 1; i++) {
        // Si on rencontre /./ on l'enlève
        if (path[i] == '/' && path[i + 1] == '.' && (i + 2 >= (int) strlen(path) || path[i + 2] == '/')) {
            for (int j = i; j < (int) strlen(path) - 2; j++) {
                path[j] = path[j + 2];
            }

            i--;
        }

        // Si on rencontre /../ on l'enleve si il est pas au début on enlève le répertoire précédent
        if (path[i] == '/' && path[i + 1] == '.' && path[i + 2] == '.' && (i + 3 >= (int) strlen(path) || path[i + 3] == '/')) {
            int temp = i;

            if (i > 0) {
                i--;
            }

            while (path[i] != '/' && i != 0) {
                i--;
            }

            for (int j = i; j < (int) strlen(path) - 1; j++) {
                path[j] = path[j + 3 + (temp - i)];
            }

            i--;
        }
    }
}

char *sanitizePath(char *path) {
    // printf("Sanitizing path\n");
    char *withoutPercents = percentEncodings(path);
    remove_dot_segments(withoutPercents);

    return withoutPercents;
}
