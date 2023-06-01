#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/api.h"
#include "../include/others.h"
#include "../include/getters.h"
#include "../include/constantes.h"
#include "../include/request.h"
#include "../include/utils.h"

/*
#define CHUNK_SIZE 16384

int compress_string(const char *input, int input_length, char **output, int *output_length) {
    int ret;
    z_stream strm;
    unsigned char out[CHUNK_SIZE];

    // Allouer un tampon pour les données compressées
    *output = (char *) malloc(CHUNK_SIZE);
    if (*output == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        return Z_MEM_ERROR;
    }

    // Initialiser la structure z_stream
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = (uInt) input_length;
    strm.next_in = (Bytef *) input;
    strm.avail_out = CHUNK_SIZE;
    strm.next_out = out;

    // Initialiser la compression
    ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        free(*output);
        fprintf(stderr, "Erreur lors de l'initialisation de la compression : %s\n", strm.msg);
        return ret;
    }

    // Boucle de compression
    do {
        ret = deflate(&strm, Z_FINISH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            free(*output);
            deflateEnd(&strm);
            fprintf(stderr, "Erreur de compression : %s\n", strm.msg);
            return ret;
        }

        // Copier les données compressées dans le tampon de sortie
        int have = CHUNK_SIZE - strm.avail_out;
        *output_length += have;
        *output = (char *) realloc(*output, *output_length + CHUNK_SIZE);
        memcpy(*output + *output_length - have, out, have);

        // Réinitialiser le tampon de sortie
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = out;
    } while (ret != Z_STREAM_END);

    // Terminer la compression
    deflateEnd(&strm);

    return Z_OK;
}
 */

//char *getEncoding() {
//    _Token *codings = searchTree(root, "codings");
////    int length;
////
////    char *start = getElementValue(codings, &length);
////    char *value = (char *) malloc(sizeof(char) * length + 1);
////    sprintf(value, "%.*s", length, start);
////
//    int flag = 0;
//    int qmax = 1;
//    char *encoding = malloc(50 * sizeof(char));
//    char *index;
//
//    if (codings == NULL) {
//        return NULL;
//    }
//
////    if (strstr(value, "q=0") == NULL) {
////
////        for (int i = 0; i < NB_ACCEPTED_ENCODINGS; i++) {
////
////            if ((strstr(value, ACCEPTED_ENCODINGS[i])) != NULL) {
////                index = strstr(value, "q=");
////                if (index != NULL) {
////
////                    if (value[atoi(index) + 2] > qmax) {
////                        qmax = value[atoi(index) + 2];
////                        strcpy(encoding, ACCEPTED_ENCODINGS[i]);
////                        flag = 1;
////
////                    } else {
////                        if (flag != 1) {
////                            strcpy(encoding, ACCEPTED_ENCODINGS[i]);
////                            flag = 1;
////                        }
////                    }
////
////                } else {
////                    if (flag != 1) {
////                        strcpy(encoding, ACCEPTED_ENCODINGS[i]);
////                        flag = 1;
////                    }
////                }
////            }
////        }
////    }
//
//    while (codings != NULL) {
//        int length;
//        char *start;
//        char qvalue[10];
//
//        start = getElementValue(codings->node, &length);
//        char *value = (char *) malloc(sizeof(char) * length + 1);
//        sprintf(value, "%.*s", length, start);
//
//        Node *weight = getBrother(codings->node);
//
//        if (weight && strcmp(getLabel(weight), "weight") == 0) {
//            start = getElementValue(weight, &length);
//            sprintf(qvalue, "%.*s", length, start);
//        }
//
//        if (strcmp(qvalue, ";q=0")) {
//            for (int i = 0; i < NB_ACCEPTED_ENCODINGS; i++) {
//                if ((strcmp(value, ACCEPTED_ENCODINGS[i])) == 0) {
//                    index = strstr(value, "q=");
//                    if (index != NULL) {
//
//                        if (value[atoi(index) + 2] > qmax) {
//                            qmax = value[atoi(index) + 2];
//                            strcpy(encoding, ACCEPTED_ENCODINGS[i]);
//                            flag = 1;
//
//                        } else {
//                            if (flag != 1) {
//                                strcpy(encoding, ACCEPTED_ENCODINGS[i]);
//                                flag = 1;
//                            }
//                        }
//
//                    } else {
//                        if (flag != 1) {
//                            strcpy(encoding, ACCEPTED_ENCODINGS[i]);
//                            flag = 1;
//                        }
//                    }
//                }
//            }
//        }
//        codings = codings->next;
//    }
//
//    return encoding;
//
//
//}

unsigned char *getFileData(char *path, int *size) {
    printf("Getting file data\n");

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

    printf("path : %s\n", fullPath);

    return fullPath;
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

    return NULL;
}

char *getHostTarget() {
    char *host = getHeaderValue(root, "Host");

    if (!host) {
        return NULL;
    }

    int len = strlen(host);

    char *hostTarget = (char *) malloc(strlen(host) * sizeof(char));
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
    printf("Getting MIME type\n");

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

    return NULL;
}

void getRangeRange(int *start, int *end) {
    Node *rangeHeader = getRangeHeader();

    char *fieldValue = getHeaderValue(rangeHeader, "field_value");
    sscanf(fieldValue, "bytes=%d-%d", start, end);
}
