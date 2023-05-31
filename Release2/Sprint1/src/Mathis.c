#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/api.h"
#include "../include/constantes.h"
#include "../include/Josias.h"
#include "../include/Mathis.h"
#include "../include/Nathan.h"
#include "../include/Hugo.h"
#include "../include/request.h"
#include "../include/utils.h"


// Getters

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


// Checkers

int check_Host_Header(int clientId) {
    char *version = getHeaderValue(root, "HTTP_version");

    _Token *result = searchTree(root, "Host_header");
    if ((result && result->next) || (!result && strcmp(version, "HTTP/1.1") == 0)) {  // s'il y a 0 ou plusieurs header host
        sendErrorCode(clientId, 400, "Bad Request");
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

            sendErrorCode(clientId, 400, "Requires HTTP/1.1 or newer");
            return 0;
        }

        free(version);

        if (!getRangeHeader()) {
            sendErrorCode(clientId, 400, "Requires Range Header");
            return 0;
        }
    }

    return 1;
}


// Senders

void send_message_body(int clientId, char *path) {
    printf("Sending body\n");

    int size;
    unsigned char *buffer = getFileData(path, &size);

    if (buffer) {
//        if (size > MAX_SIZE_WITHOUT_CHUNK && isGet()) {
//            free(buffer);
//            return send_message_body_chunked(clientId, path);
//        }

        send_Content_Length_Header(clientId, size);

        if (isGet()) {
            writeDirectClient(clientId, "\r\n", 2);
            writeDirectClient(clientId, (char *) buffer, size);

            free(buffer);
        }
    }
}

void send_message_body_chunked(int clientId, char *path) {
    printf("chunking\n");
    FILE *file = fopen(path, "rb");
    char buffer[MAX_SIZE_WITHOUT_CHUNK];
    int bytesRead;

    send_Transfer_Encoding_Header(clientId, "chunked");
    writeDirectClient(clientId, "\r\n", 2);

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        char chunkSize[16];
        sprintf(chunkSize, "%X\r\n", bytesRead);
        writeDirectClient(clientId, chunkSize, strlen(chunkSize));
        writeDirectClient(clientId, buffer, bytesRead);
        writeDirectClient(clientId, "\r\n", 2);
    }

    writeDirectClient(clientId, "0\r\n\r\n", 5);

    fclose(file);
}

void send_message_body_streaming(int clientId, char *path) {
    printf("streaming\n");
    int start = -1;
    int end = -1;
    getRangeRange(&start, &end);

    int size;
    unsigned char *buffer = getFileData(path, &size);

    if (buffer) {
        if (end == -1) {
            end = start + MAX_SIZE_STREAMING - 1 < size ? start + MAX_SIZE_STREAMING - 1 : size;
        }

        int contentLength = end - start + 1;

        send_Content_Range_Header(clientId, start, end, size);
        send_Content_Length_Header(clientId, contentLength);

        if (isGet()) {
            unsigned char *bufferToSend = (unsigned char *) malloc(sizeof(char) * contentLength);

            for (int i = 0; i < contentLength; i++) {
                bufferToSend[i] = buffer[start + i];
            }

            writeDirectClient(clientId, "\r\n", 2);
            writeDirectClient(clientId, (char *) bufferToSend, contentLength);
        }

        free(buffer);

    }
}

void send_Accept_Ranges_Header(int clientId) {
    char message[25] = "Accept-Ranges: bytes\r\n";

    writeDirectClient(clientId, message, strlen(message));
}

void send_Content_Range_Header(int clientId, int start, int end, int contentSize) {
    char message[50];

    sprintf(message, "Content-Range: bytes %d-%d/%d\r\n", start, end, contentSize);
    writeDirectClient(clientId, message, strlen(message));
}

void sendErrorCode(int clientId, int errorCode, char *errorMessage) {
    char message[150];

    sprintf(message, TEMPLATE_ERROR, errorCode, errorMessage, errorCode, errorMessage);

    send_status_line(clientId, errorCode, errorMessage);
    send_Content_Type_Header(clientId, "text/html");
    send_Date_Header(clientId);
    send_Server_Header(clientId);
    writeDirectClient(clientId, "\r\n", 2);

    if (isGet()) {
        writeDirectClient(clientId, message, strlen(message));
    }
}
