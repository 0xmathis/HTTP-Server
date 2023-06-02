#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/api.h"
#include "../include/constantes.h"
#include "../include/getters.h"
#include "../include/others.h"
#include "../include/request.h"
#include "../include/utils.h"


unsigned char *getFileData(char *path, int *size) {
    FILE *file = fopen(path, "rb");

    fseek(file, 0L, SEEK_END);
    *size = ftell(file);
    rewind(file);

    unsigned char *buffer = (unsigned char *) malloc(sizeof(unsigned char) * (*size + 1));

    fread(buffer, *size, 1, file);
    fclose(file);

    return buffer;
}

void getFileExtension(char *path, char *output) {
    int len = strlen(path);

    for (int i = 0; path[i] != '\0' && i < len ; i++) {  // on cherche le dernier point avant la fin du path
        if (path[i] == '.') {
            snprintf(output, 50, "%s", path + i);
        }
    }
}

char *getFilePath(int *size) {
    char *absolutePath = getHeaderValue(root, "absolute_path");
    char *partialPath = (char *) malloc(sizeof(char) * 200);
    char *fullPath = (char *) malloc(sizeof(char) * 300);
    char *host = getHostTarget();

    char *sanitizedPath = sanitizePath(absolutePath);

    if (host) {
        snprintf(partialPath, 200, "sites/%s", host);
    } else {
        snprintf(partialPath, 200, "sites/%s", PATH_DEFAULT);
    }

    snprintf(fullPath, 300, "%s%s", partialPath, sanitizedPath);

    if (*sanitizedPath == '/' && *(sanitizedPath + 1) == '\0') {  // si on demande la racine du site
        strncat(fullPath, "index.html", 300);
    }

    free(absolutePath);
    free(partialPath);
    free(sanitizedPath);
    free(host);

    if (size) {
        *size = strlen((fullPath));
    }

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
        snprintf(value, length + 1, "%.*s", length, start);
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

    char *hostTarget = (char *) malloc(sizeof(char) * (len + 1));
    int i = 0;
    int j = 0;

    if (startWith("www.", host)) {
        i = 4;
    }

    while (i < len && host[i] != ':') {
        hostTarget[j] = host[i];
        i++;
        j++;
    }

    hostTarget[j] = '\0';

    free(host);
    return hostTarget;
}

char *getMIMEtype(char *path) {
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
