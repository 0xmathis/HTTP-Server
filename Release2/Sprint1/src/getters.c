#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/api.h"
#include "../include/others.h"
#include "../include/getters.h"
#include "../include/constantes.h"
#include "../include/request.h"
#include "../include/utils.h"


unsigned char *getFileData(char *path, int *size) {
    // printf("Getting file data\n");

    FILE *file = fopen(path, "rb");

    fseek(file, 0L, SEEK_END);
    *size = ftell(file);
    rewind(file);

    unsigned char *buffer = (unsigned char *) malloc(*size);

    fread(buffer, *size, 1, file);
    fclose(file);

    return buffer;
}

void getFileExtension(char *ptr, char *output) {
    for (int i = 0; ptr[i] != '\0' && i < (int) strlen(ptr); i++) {
        if (ptr[i] == '.') {
            strcpy(output, &ptr[i]);
        }
    }
}

char *getFilePath() {
    // printf("Getting path\n");
    char *absolutePath = getHeaderValue(root, "absolute_path");
    char *partialPath = (char *) malloc(sizeof(char) * 200);
    char *fullPath = (char *) malloc(sizeof(char) * 300);
    char *host = getHostTarget();

    char *sanitizedPath = sanitizePath(absolutePath);

    if (host) {
        sprintf(partialPath, "sites/%s", host);
    } else {
        sprintf(partialPath, "sites/%s", PATH_DEFAULT);
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

    // printf("path : %s\n", fullPath);

    return fullPath;
}

int getFilePathLength() {
    _Token *result = searchTree(root, "absolute_path");

    if (result) {
        Node *node = (Node *) result->node;
        int length;
        getElementValue(node, &length);
        purgeElement(&result);

        return length;
    }

    purgeElement(&result);

    return 0;
}

char *getHeaderValue(Node *start, char *headerValue) {
    _Token *result = searchTree(start, headerValue);

    if (result) {
        Node *node = (Node *) result->node;
        int length;
        char *start = getElementValue(node, &length);
        char *value = (char *) malloc(sizeof(char) * (length + 1));
        sprintf(value, "%.*s", length, start);
        purgeElement(&result);

        return value;
    }

    purgeElement(&result);
    return NULL;
}

char *getHostTarget() {
    char *host = getHeaderValue(root, "Host");

    if (!host) {
        return NULL;
    }

    int len = strlen(host);

    char *hostTarget = (char *) malloc(len * sizeof(char));
    int i = 0;
    int j = 0;

    if (startWith("www.", host)) {
        i = 4;
    }

    while (i < len-1 && host[i] != ':') {
        hostTarget[j] = host[i];
        i++;
        j++;
    }

    hostTarget[j] = '\0';

    free(host);
    return hostTarget;
}

char *getMIMEtype(char *path) {
    // printf("Getting MIME type\n");

    char extension[50];
    getFileExtension(path, extension);

    if (startWith(".aac", extension)) {
        return "audio/aac";
    } else if (startWith(".avif", extension)) {
        return "image/avif";
    } else if (startWith(".avi", extension)) {
        return "video/x-msvideo";
    } else if (startWith(".bin", extension)) {
        return "application/octet-stream";
    } else if (startWith(".bmp", extension)) {
        return "image/bmp";
    } else if (startWith(".css", extension)) {
        return "text/css";
    } else if (startWith(".csv", extension)) {
        return "text/csv";
    } else if (startWith(".gif", extension)) {
        return "image/gif";
    } else if (startWith(".html", extension)) {
        return "text/html";
    } else if (startWith(".htm", extension)) {
        return "text/html";
    } else if (startWith(".ico", extension)) {
        return "image/x-icon";
    } else if (startWith(".jpeg", extension)) {
        return "image/jpeg";
    } else if (startWith(".jpg", extension)) {
        return "image/jpeg";
    } else if (startWith(".json", extension)) {
        return "application/json";
    } else if (startWith(".js", extension)) {
        return "text/javascript";
    } else if (startWith(".mp3", extension)) {
        return "audio/mpeg";
    } else if (startWith(".mp4", extension)) {
        return "video/mp4";
    } else if (startWith(".mpeg", extension)) {
        return "video/mpeg";
    } else if (startWith(".php", extension)) {
        return "text/x-php";
    } else if (startWith(".png", extension)) {
        return "image/png";
    } else if (startWith(".sh", extension)) {
        return "application/x-sh";
    } else if (startWith(".txt", extension)) {
        return "text/plain";
    } else if (startWith(".wav", extension)) {
        return "audio/wav";
    }

    return "application/octet-stream";
}

Node *getRangeHeader() {
    _Token *result = searchTree(root, "header_field");

    while (result) {
        if (startWith("Range:", getStart(result->node))) {
            Node *output = result->node;
            purgeElement(&result);
            return output;
        }

        result = result->next;
    }

    purgeElement(&result);

    return NULL;
}

void getRangeRange(int *start, int *end) {
    Node *rangeHeader = getRangeHeader();

    char *fieldValue = getHeaderValue(rangeHeader, "field_value");
    sscanf(fieldValue, "bytes=%d-%d", start, end);
    free(fieldValue);
}
