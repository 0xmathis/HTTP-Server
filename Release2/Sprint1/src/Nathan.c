#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/api.h"
#include "../include/Josias.h"
#include "../include/Mathis.h"
#include "../include/Nathan.h"
#include "../include/constantes.h"
#include "../include/Hugo.h"
#include "../include/request.h"
#include "../include/utils.h"

char ACCEPTED_ENCODINGS[5][10] = {"*", "deflate", "br", "identity"};
int NB_ACCEPTED_ENCODINGS = 4;

// Getters

char *getEncoding() {
    _Token *codings = searchTree(root, "codings");
//    int length;
//
//    char *start = getElementValue(codings, &length);
//    char *value = (char *) malloc(sizeof(char) * length + 1);
//    sprintf(value, "%.*s", length, start);
//
    int flag = 0;
    int qmax = 1;
    char *encoding = malloc(50 * sizeof(char));
    char *index;

    if (codings == NULL) {
        return NULL;
    }

//    if (strstr(value, "q=0") == NULL) {
//
//        for (int i = 0; i < NB_ACCEPTED_ENCODINGS; i++) {
//
//            if ((strstr(value, ACCEPTED_ENCODINGS[i])) != NULL) {
//                index = strstr(value, "q=");
//                if (index != NULL) {
//
//                    if (value[atoi(index) + 2] > qmax) {
//                        qmax = value[atoi(index) + 2];
//                        strcpy(encoding, ACCEPTED_ENCODINGS[i]);
//                        flag = 1;
//
//                    } else {
//                        if (flag != 1) {
//                            strcpy(encoding, ACCEPTED_ENCODINGS[i]);
//                            flag = 1;
//                        }
//                    }
//
//                } else {
//                    if (flag != 1) {
//                        strcpy(encoding, ACCEPTED_ENCODINGS[i]);
//                        flag = 1;
//                    }
//                }
//            }
//        }
//    }

    while (codings != NULL) {
        int length;
        char *start;
        char qvalue[10];

        start = getElementValue(codings->node, &length);
        char *value = (char *) malloc(sizeof(char) * length + 1);
        sprintf(value, "%.*s", length, start);

        Node *weight = getBrother(codings->node);

        if (weight && strcmp(getLabel(weight), "weight") == 0) {
            start = getElementValue(weight, &length);
            sprintf(qvalue, "%.*s", length, start);
        }

        if (strcmp(qvalue, ";q=0")) {
            for (int i = 0; i < NB_ACCEPTED_ENCODINGS; i++) {
                if ((strcmp(value, ACCEPTED_ENCODINGS[i])) == 0) {
                    index = strstr(value, "q=");
                    if (index != NULL) {

                        if (value[atoi(index) + 2] > qmax) {
                            qmax = value[atoi(index) + 2];
                            strcpy(encoding, ACCEPTED_ENCODINGS[i]);
                            flag = 1;

                        } else {
                            if (flag != 1) {
                                strcpy(encoding, ACCEPTED_ENCODINGS[i]);
                                flag = 1;
                            }
                        }

                    } else {
                        if (flag != 1) {
                            strcpy(encoding, ACCEPTED_ENCODINGS[i]);
                            flag = 1;
                        }
                    }
                }
            }
        }
        codings = codings->next;
    }

    return encoding;


}

void getFileExtension(char *ptr, char *output) {
    for (int i = 0; ptr[i] != '\0'; i++) {
        if (ptr[i] == '.') {
            strcpy(output, &ptr[i]);
        }
    }
}

char *getFilePath() {
    printf("Getting path\n");
    char *absolutePath = getHeaderValue(root, "absolute_path");
    char *partialPath = (char *) malloc(sizeof(char) * 200);
    char *fullPath = (char *) malloc(sizeof(char) * 200);
    char *host = getHostTarget();

    char *sanitizedPath = sanitizePath(absolutePath);

    if (host) {
        strcpy(partialPath, host);
    } else {
        strcpy(partialPath, PATH_DEFAULT);
    }

    strcpy(fullPath, partialPath);
    strcat(fullPath, sanitizedPath);


    if (*sanitizedPath == '/' && *(sanitizedPath + 1) == '\0') {  // si on demande la racine du site
        strcat(fullPath, "index.html");
    }

    free(absolutePath);
    free(partialPath);
    free(sanitizedPath);
    free(host);

    printf("path : %s\n", fullPath);

    return fullPath;
}


// Checkers

int check_Accept_Encoding_Header(int clientId) {
    printf("Checking Accept encoding\n");

    _Token *codings = searchTree(root, "codings");
    _Token *c = codings;
//    int length;
//    char *start = getElementValue(codings, &length);
//    char *value = (char *) malloc(sizeof(char) * length + 1);
//    sprintf(value, "%.*s", length, start);

    if (codings == NULL) {
        return 1;
    }

    int flag = 0;

//    if (strstr(value, "q=0") == NULL) {
//        for (int i = 0; i < NB_ACCEPTED_ENCODINGS; i++) {
//            if ((strstr(value, ACCEPTED_ENCODINGS[i])) != NULL) {
//                return 0;
//            }
//        }
//    }

    while (codings != NULL && !flag) {
        Node *node = codings->node;

        int length;
        char *start;
        char qvalue[10];

        start = getElementValue(node, &length);
        char *value = (char *) malloc(sizeof(char) * (length + 1));
        sprintf(value, "%.*s", length, start);

        Node *weight = getBrother(node);

        if (weight && strcmp(getLabel(weight), "weight") == 0) {
            start = getElementValue(weight, &length);
            sprintf(qvalue, "%.*s", length, start);
        }

        if (strcmp(qvalue, ";q=0")) {
            for (int i = 0; i < NB_ACCEPTED_ENCODINGS; i++) {
                if ((strcmp(value, ACCEPTED_ENCODINGS[i])) == 0) {
                    flag = 1;
                }
            }
        }

        free(value);
        codings = codings->next;
    }

    purgeElement(&c);

    if (!flag) {
        sendErrorCode(clientId, 406, "Not Acceptable");
        return 0;
    }

    return 1;
}

int check_path(int clientId, char *path) {
//    if (!startWith(PATH1, path) && !startWith(PATH2, path)) {
//        sendErrorCode(clientId, 403, "Forbidden");
//        return 0;
//    }

    FILE *file = fopen(path, "rb");

    if (!file) {
        sendErrorCode(clientId, 404, "Not Found");
        return 0;
    }

    printf("path ok\n");

    fclose(file);

    return 1;
}

// Senders

void send_Content_Encoding_Header(int clientId) {
    char *encoding = getEncoding();

    if (encoding) {
        char message[50];
        sprintf(message, "Content-encoding: %s\r\n", encoding);
        writeDirectClient(clientId, message, strlen(message));
        free(encoding);
    }

}

void send_Transfer_Encoding_Header(int clientId, char *encoding) {
    char message[50];
    sprintf(message, "Transfer-Encoding: %s\r\n", encoding);
    writeDirectClient(clientId, message, strlen(message));
}


// Others

int isStreamable(char *mimeType) {
    return startWith("video", mimeType) || startWith("audio", mimeType);
}

char *percentEncodings(char *path) {
    printf("Removing percent encodings\n");

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

char *sanitizePath(char *path) {
    printf("Sanitizing path\n");
    char *withoutPercents = percentEncodings(path);
    remove_dot_segments(withoutPercents);

    return withoutPercents;
}


/*
int check_accept_language(Node *root, int clientId) {

    _Token *accept_language = searchTree(root, "accept-language");
    int length;

    char *start = getElementValue(accept_language, &length);
    char *value = (char *) malloc(sizeof(char) * length+1);
    sprintf(value, "%.*s", length, start);

    while (accept_language->next != NULL) {

        if (strcmp(value, "fr") == 0 || strcmp(value,'fr-FR') == 0) {
            return 0;
        }
    }

    sendErrorCode(clientId, 406, "Le français n'est pas dans la liste des langages acceptés");
    return 1;
}
*/
