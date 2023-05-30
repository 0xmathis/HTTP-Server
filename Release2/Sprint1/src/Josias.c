#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/api.h"
#include "../include/Josias.h"
#include "../include/Mathis.h"
#include "../include/Nathan.h"
#include "../include/request.h"
#include "../include/utils.h"

// Getters

char *getHostTarget() {
    char *host = getHeaderValue(root, "Host");

    if (!host) {
        return NULL;
    }

    char *hostTarget = (char *) malloc(strlen(host) * sizeof(char));
    int i = 0;
    int j = 0;

    if (startWith("www.", host)) {
        i = 4;
    }

    while (host[i]) {
        hostTarget[j] = host[i];
        i++;
        j++;
    }

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


// Checkers

int check_Accept_Header(int clientId, char *path) {
    printf("Checking Accept\n");

    _Token *mediaRanges = searchTree(root, "media_range");
    _Token *mr = mediaRanges;

    if (!mediaRanges) {  // si pas de media range (donc pas de Accept Header), on accepte tout les types
        return 1;
    }

    char *mimeType = getMIMEtype(path);
    int flag = 0;
//    int flag1 = 0;
//    int flag2 = 0;

    while (mediaRanges && !flag) {
        Node *node = mediaRanges->node;

        char *parameter = getHeaderValue(node, "parameter");

        if (parameter && strcmp(parameter, "q=0") == 0) {
            free(parameter);
            mediaRanges = mediaRanges->next;
            continue;
        }

        char *firstChild = getHeaderValue(node, "case_insensitive_string");

        if (strcmp(firstChild, "*/*") == 0) {
            free(firstChild);
            flag = 1;
        } else {
            char *typeAccepted = getHeaderValue(node, "type");
            char *subtypeAccepted = getHeaderValue(node, "subtype");
            char mediaRange[50];
            sprintf(mediaRange, "%s/%s", typeAccepted, subtypeAccepted);

            // Si on accepte le media type ou qu'on accepte tout
            if ((strcmp(mediaRange, mimeType) == 0 || strcmp(mediaRange, "*/*") == 0) && (!parameter || strcmp(parameter, "q=0"))) {
                flag = 1;
            } else if (startWith(typeAccepted, mimeType)) { //Si les types coïncident
                char shortenMimeType[50];
                strcpy(shortenMimeType, mimeType + strlen(typeAccepted) + 1); //chaine mimeType sans le type

                if ((startWith(subtypeAccepted, shortenMimeType) || *subtypeAccepted == '*') && (!parameter || strcmp(parameter, "q=0"))) { //Si le sous-type est accepté
                    flag = 1;
                }
            }

            free(firstChild);
            free(typeAccepted);
            free(subtypeAccepted);
        }

        if (parameter) {
            free(parameter);
        }

        // //On ecrit dans la variable value la chaine de media-range
        // int length;
        // char *start = getElementValue(node, &length);
        // char *value = (char *) malloc(sizeof(char) * (length + 1));
        // sprintf(value, "%.*s", length, start);
        // printf("\tvalue : %s\n", value);

        // if (strcmp(value, "*/*") == 0) {
        //     flag1 = 1;
        //     flag2 = 1;
        // }

        // if (strcmp(mimeType, value) == 0) { //Si type et sous-type coïncident remplacable par strstr(value, mimeType)
        //     flag1 = 1;
        // }

        // //Stockage du type et du sous-type
        // Node *type = getChild(node);
        // int lenght_type;
        // char *start_type = getElementValue(type, &lenght_type);
        // char *value_type = (char *) malloc(sizeof(char) * (lenght_type + 1));
        // sprintf(value_type, "%.*s", lenght_type, start_type);

        // Node *subtype = getChild(node);
        // int lenght_subtype;
        // char *start_subtype = getElementValue(subtype, &lenght_subtype);
        // char *value_subtype = (char *) malloc(sizeof(char) * (lenght_subtype + 1));
        // sprintf(value_subtype, "%.*s", lenght_subtype, start_subtype);

        // if (startWith(mimeType, value_type)) { //Si les types ne coïncident pas
        //     char *shortenMimeType = strcpy(shortenMimeType, mimeType + lenght_type + 1); //chaine mimeType sans le type
        //     if (startWith(shortenMimeType, value_subtype) || *value_subtype == '*') { //Si le sous-type est accepté
        //         flag1 = 1;
        //     }
        // }

        // if (flag1) { //Type et sous-type sont acceptés
        //     //On verifie que accept_params ne contient pas q = 0
        //     _Token *accept_params = searchTree(root, "accept_params");

        //     while (accept_params != NULL) {
        //         start = getElementValue(accept_params->node, &length);
        //         value = (char *) malloc(sizeof(char) * (length + 1));
        //         sprintf(value, "%.*s", length, start);

        //         if (strstr(value, "q=0") == NULL) {
        //             flag2 = 1;
        //         }
        //         accept_params = accept_params->next;
        //     }
        // }

        mediaRanges = mediaRanges->next;
    }
    // Fin de parcours des media-range

//    if (!flag2) { // Si les mime type n'est pas accepté
//        printf("ici la\n");
//        sendErrorCode(clientId, 406, "Not accepted media type");
//        return 0;
//    }

    purgeElement(&mr);

    if (!flag) {
        sendErrorCode(clientId, 406, "Not Acceptable");
        return 0;
    }

    return 1;
}

int check_headers(int clientId, char *path) {
    return check_Host_Header(clientId) && check_Range_Header(clientId, path) && check_Accept_Header(clientId, path) && check_Accept_Encoding_Header(clientId);
}

int check_method(int clientId) {
    char *method = getHeaderValue(root, "method");

    if (strcmp(method, "GET") && strcmp(method, "HEAD")) {
        free(method);
        sendErrorCode(clientId, 501, "Not Implemented");
        return 0;
    }

    free(method);

    return 1;
}


// Senders

void send_Connection_Header(int clientId) {
    char *connectionOption = getHeaderValue(root, "connection_option");

    if (connectionOption) {
        char message[30];
        sprintf(message, "Connection: %s\r\n", connectionOption);
        writeDirectClient(clientId, message, strlen(message));
        free(connectionOption);
    }
}

void send_Content_Length_Header(int clientId, int size) {
    char message[30];

    sprintf(message, "Content-Length: %d\r\n", size);
    writeDirectClient(clientId, message, strlen(message));
}

void send_Content_Type_Header(int clientId, char *mimeType) {
    char message[50];
    sprintf(message, "Content-type: %s\r\n", mimeType);
    writeDirectClient(clientId, message, strlen(message));
}


// Others

int isGet() {
    char *method = getHeaderValue(root, "method");

    if (strcmp(method, "GET") == 0) {
        free(method);
        return 1;
    }

    free(method);
    return 0;
}

