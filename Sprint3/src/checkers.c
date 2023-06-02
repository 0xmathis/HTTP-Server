#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/api.h"
#include "../include/checkers.h"
#include "../include/fastCGI.h"
#include "../include/getters.h"
#include "../include/others.h"
#include "../include/request.h"
#include "../include/senders.h"
#include "../include/utils.h"


int check_Accept_Header(int clientId, char *path) {
    _Token *mediaRanges = searchTree(root, "media_range");
    _Token *mr = mediaRanges;

    if (!mediaRanges) {  // si pas de media range (donc pas de Accept Header), on accepte tout les types
        return 1;
    }

    char *mimeType = getMIMEtype(path);
    int flag = 0;

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
            flag = 1;
        } else {
            char *typeAccepted = getHeaderValue(node, "type");
            char *subtypeAccepted = getHeaderValue(node, "subtype");
            char mediaRange[50];
            snprintf(mediaRange, 50, "%s/%s", typeAccepted, subtypeAccepted);

            // Si on accepte le media type ou qu'on accepte tout
            if ((strcmp(mediaRange, mimeType) == 0 || strcmp(mediaRange, "*/*") == 0) && (!parameter || strcmp(parameter, "q=0"))) {
                flag = 1;
            } else if (startWith(typeAccepted, mimeType)) { //Si les types coïncident
                char shortenMimeType[50];
                snprintf(shortenMimeType, 50, "%s", mimeType + strlen(typeAccepted) + 1); //chaine mimeType sans le type

                if ((startWith(subtypeAccepted, shortenMimeType) || *subtypeAccepted == '*') && (!parameter || strcmp(parameter, "q=0"))) { //Si le sous-type est accepté
                    flag = 1;
                }
            }

            free(typeAccepted);
            free(subtypeAccepted);
        }

        free(firstChild);

        if (parameter) {
            free(parameter);
        }

        mediaRanges = mediaRanges->next;
    }

    purgeElement(&mr);

    if (!flag) {
        send_error_code(clientId, 406, "Not Acceptable");
        return 0;
    }

    return 1;
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

int check_Content_Length_Header(int clientId) {
    if (!isPost()) {
        return 1;
    }

    _Token *result = searchTree(root, "message_body");

    char *contentLength = getHeaderValue(root, "Content_Length");
    int messageBodyLength = getLength(result->node);

    purgeElement(&result);

    if ((contentLength && atoi(contentLength) == messageBodyLength)) {
        free(contentLength);
        return 1;
    }

    if (contentLength) {
        free(contentLength);
    }

    send_error_code(clientId, 400, "Bad Request");
    return 0;
}

int check_headers(int clientId, char *path) {
    return check_Host_Header(clientId) && check_Range_Header(clientId, path) && check_Accept_Header(clientId, path) && check_Content_Length_Header(clientId) /*&& check_Accept_Encoding_Header(clientId)*/;
}

int check_Host_Header(int clientId) {
    char *version = getHeaderValue(root, "HTTP_version");

    _Token *result = searchTree(root, "Host_header");
    _Token *r = result;

    if ((result && result->next) || (!result && strcmp(version, "HTTP/1.1") == 0)) {  // s'il y a 0 ou plusieurs header host
        free(version);
        purgeElement(&r);
        send_error_code(clientId, 400, "Bad Request");
        return 0;
    }

    free(version);
    purgeElement(&r);

    return 1;
}

int check_method(int clientId, char *path) {
    char *method = getHeaderValue(root, "method");

    if (strcmp(method, "GET") && strcmp(method, "HEAD") && strcmp(method, "POST")) {
        free(method);
        send_error_code(clientId, 501, "Not Implemented");
        return 0;
    }

    if (strcmp(method, "POST") == 0) {
        char extension[50];
        getFileExtension(path, extension);

        if (strcmp(extension, ".php")) {
            send_error_code(clientId, 400, "Bad Request");
            return 0;
        }
    }

    free(method);

    return 1;
}

int check_path(int clientId, char *path) {
    FILE *file = fopen(path, "rb");

    if (!file) {
        send_error_code(clientId, 404, "Not Found");
        return 0;
    }

    fclose(file);

    return 1;
}

int check_Range_Header(int clientId, char *path) {
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

int check_request(int clientId) {
    int size;
    char *path = getFilePath(&size);

    if (size > 300) {
        send_error_code(clientId, 413, "Path Too Large");
        return 0;
    }

    if (!check_path(clientId, path) || !check_method(clientId, path) || !check_headers(clientId, path)) {
        free(path);
        return 0;
    }

    free(path);
    return 1;
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

int check_version() {
    char *version = getHeaderValue(root, "HTTP_version");
    int ret = 0;

    if (!strcmp(version, "HTTP/1.1")) {
        ret = 1;
    }

    free(version);

    return ret;
}