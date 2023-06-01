#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/api.h"
#include "../include/constantes.h"
#include "../include/senders.h"
#include "../include/checkers.h"
#include "../include/others.h"
#include "../include/getters.h"
#include "../include/request.h"
#include "../include/utils.h"
#include "../include/fastCGI.h"


int check_Host_Header(int clientId) {
    char *version = getHeaderValue(root, "HTTP_version");

    _Token *result = searchTree(root, "Host_header");
    if ((result && result->next) || (!result && strcmp(version, "HTTP/1.1") == 0)) {  // s'il y a 0 ou plusieurs header host
        send_error_code(clientId, 400, "Bad Request");
        return 0;
    }

    free(version);

    return 1;
}

int check_Range_Header(int clientId, char *path) {
    printf("Checking Range\n");
    char *mimeType = getMIMEtype(path);

    if (isStreamable(mimeType)) {
        char *version = getHeaderValue(root, "HTTP_version");

        if (strcmp(version, "HTTP/1.0") == 0) {
            free(version);

            send_error_code(clientId, 400, "Requires HTTP/1.1 or newer");
            return 0;
        }

        free(version);

        if (!getRangeHeader()) {
            send_error_code(clientId, 400, "Requires Range Header");
            return 0;
        }
    }

    return 1;
}

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
//        send_error_code(clientId, 406, "Not accepted media type");
//        return 0;
//    }

    purgeElement(&mr);

    if (!flag) {
        send_error_code(clientId, 406, "Not Acceptable");
        return 0;
    }

    return 1;
}

int check_headers(int clientId, char *path) {
    return check_Host_Header(clientId) && check_Range_Header(clientId, path) && check_Accept_Header(clientId, path) /*&& check_Accept_Encoding_Header(clientId)*/;
}

int check_method(int clientId) {
    char *method = getHeaderValue(root, "method");

    if (strcmp(method, "GET") && strcmp(method, "HEAD") && strcmp(method, "POST")) {
        free(method);
        send_error_code(clientId, 501, "Not Implemented");
        return 0;
    }

    free(method);

    return 1;
}

//int check_Accept_Encoding_Header(int clientId) {
//    printf("Checking Accept encoding\n");
//
//    _Token *codings = searchTree(root, "codings");
//    _Token *c = codings;
////    int length;
////    char *start = getElementValue(codings, &length);
////    char *value = (char *) malloc(sizeof(char) * length + 1);
////    sprintf(value, "%.*s", length, start);
//
//    if (codings == NULL) {
//        return 1;
//    }
//
//    int flag = 0;
//
////    if (strstr(value, "q=0") == NULL) {
////        for (int i = 0; i < NB_ACCEPTED_ENCODINGS; i++) {
////            if ((strstr(value, ACCEPTED_ENCODINGS[i])) != NULL) {
////                return 0;
////            }
////        }
////    }
//
//    while (codings != NULL && !flag) {
//        Node *node = codings->node;
//
//        int length;
//        char *start;
//        char qvalue[10];
//
//        start = getElementValue(node, &length);
//        char *value = (char *) malloc(sizeof(char) * (length + 1));
//        sprintf(value, "%.*s", length, start);
//
//        Node *weight = getBrother(node);
//
//        if (weight && strcmp(getLabel(weight), "weight") == 0) {
//            start = getElementValue(weight, &length);
//            sprintf(qvalue, "%.*s", length, start);
//        }
//
//        if (strcmp(qvalue, ";q=0")) {
//            for (int i = 0; i < NB_ACCEPTED_ENCODINGS; i++) {
//                if ((strcmp(value, ACCEPTED_ENCODINGS[i])) == 0) {
//                    flag = 1;
//                }
//            }
//        }
//
//        free(value);
//        codings = codings->next;
//    }
//
//    purgeElement(&c);
//
//    if (!flag) {
//        send_error_code(clientId, 406, "Not Acceptable");
//        return 0;
//    }
//
//    return 1;
//}

int check_path(int clientId, char *path) {
//    if (!startWith(PATH1, path) && !startWith(PATH2, path)) {
//        send_error_code(clientId, 403, "Forbidden");
//        return 0;
//    }

    FILE *file = fopen(path, "rb");

    if (!file) {
        send_error_code(clientId, 404, "Not Found");
        return 0;
    }

    printf("path ok\n");

    fclose(file);

    return 1;
}

int check_version() {
    char *version = getHeaderValue(root, "HTTP_version");

    int ret = 0;

    if (!strcmp(version, "HTTP/1.1")) {
        ret = 1;
    }

    free(version);

    return ret;
}

int check_Connection_Header() {
    char *connection = getHeaderValue(root, "Connection");
    char *version = getHeaderValue(root, "HTTP_version");

    if ((connection && strcmp(connection, "close") == 0 && strcmp(version, "HTTP/1.1") == 0) || strcmp(version, "HTTP/1.0") == 0) {
        free(connection);
        free(version);
        return 1;
    }

    free(connection);
    free(version);

    return 0;
}

int check_Transfer_Encoding(int clientId) {
    _Token *transfer_coding = searchTree(root, "transfer_coding");

    while (transfer_coding != NULL) {

        char *transfer_coding_value = getElementValue(transfer_coding->node, NULL);

        if (strcmp(transfer_coding_value, "chunked") && strcmp(transfer_coding_value, "compress") && strcmp(transfer_coding_value, "deflate") && strcmp(transfer_coding_value, "gzip") && strcmp(transfer_coding_value, "identity")) {

            free(transfer_coding_value);
            free(transfer_coding);
            send_error_code(clientId, 501, "Unknown transfer coding");
            return 0;
        }

        transfer_coding = transfer_coding->next;
    }

    return 1;
}

int check_request(int clientId) {
    printf("Checking\n");
    char *path = getFilePath();

    if (!check_path(clientId, path) || !check_method(clientId) || !check_headers(clientId, path)) {
        printf("Not checked\n");
        return 0;
    }

    free(path);
    printf("Checked\n");
    return 1;
}