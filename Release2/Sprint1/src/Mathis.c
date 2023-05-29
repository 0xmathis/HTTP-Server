#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/api.h"
#include "../include/constantes.h"
#include "../include/Josias.h"
#include "../include/Mathis.h"
#include "../include/Hugo.h"
#include "../include/request.h"
#include "../include/utils.h"

int check_request(Node *root, int clientId) {
    if (!check_method(root, clientId) || !check_headers(root, clientId) || !check_path(root, clientId)) {
        printf("laaaaaa\n");
        return 0;
    }

    return 1;
}

int check_method(Node *root, int clientId) {
    char *method = getHeaderValue(root, "method");

    if (strcmp(method, "GET") && strcmp(method, "HEAD")) {
        sendErrorCode(root, clientId, 501, "Not Implemented");
        return 0;
    }

    free(method);

    return 1;
}

// int check_path(Node *root, int clientId) {
//     char *path = getFilePath(root);
//     FILE *file = fopen(path, "rb");
//     printf("path : %s\n", path);

//     for (int i = 0; i < (int) strlen(path) - 1; i++) {

//         if (path[i] == '/' && path[i + 1] == '.' && (i + 2 >= strlen(path) || path[i + 2] == '/')) {
            
//             for(int j = i+3; j < (int) strlen(path) - 1; j++){
//                 path[j-3] = path[j];
//             }
//         }

//         if (path[i] == '/' && path[i + 1] == '.' && path[i + 2] == '.' && (i + 3 >= strlen(path) || path[i + 3] == '/')) {
//             i --;
//             while(path[i] != '/'){
//                 i --;
//             }
            
//             for(int j = i+3; j < (int) strlen(path) - 1; j++){
//                 path[j-3] = path[j];
//             }
//         }
//     }

//     if (!file) {
//         printf("Problème fichier\n");
//         sendErrorCode(root, clientId, 404, "Not Found");
//         return 0;
//     }

//     fclose(file);
//     free(path);

//     return 1;
// }

int isGet(Node *root) {
    char *method = getHeaderValue(root, "method");

    if (strcmp(method, "GET") == 0) {
        free(method);
        return 1;
    }

    free(method);
    return 0;
}

int isRange(Node *root) {
    return getRangeHeader(root) != NULL;
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

char *getHostTarget(Node *root) {
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

    while (host[i] != ':') {
        hostTarget[j] = host[i];
        i++;
        j++;
    }

    free(host);
    return hostTarget;
}

Node *getRangeHeader(Node *root) {
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

void getRangeRange(Node *root, int *start, int *end) {
    Node *rangeHeader = getRangeHeader(root);

    char *fieldValue = getHeaderValue(rangeHeader, "field_value");
    sscanf(fieldValue, "bytes=%d-%d", start, end);
}

char *getFilePath(Node *root) {
    char *partialPath = (char *) malloc(sizeof(char) * 200);
    char *fullPath = (char *) malloc(sizeof(char) * 200);
    char *path = getHeaderValue(root, "request_target");
    char *host = getHostTarget(root);

    if (host) {
        printf("host : %s\n", host);
        if (strcmp(host, HOST1) == 0) {
            sprintf(partialPath, "%s", PATH1);
        } else if (strcmp(host, HOST2) == 0) {
            sprintf(partialPath, "%s", PATH2);
        }
    } else {
        sprintf(partialPath, "%s", PATH_DEFAULT);
    }

    if (strlen(path) == 1 && *path == '/') {  // si on demande la racine du site
        sprintf(fullPath, "%s%sindex.html", partialPath, path);
    } else {
        sprintf(fullPath, "%s%s", partialPath, path);
    }

    free(path);
    free(partialPath);
    free(host);

    return fullPath;
}

int check_Host_Header(Node *root, int clientId) {
    char *version = getHeaderValue(root, "HTTP_version");

    _Token *result = searchTree(root, "Host_header");
    if ((result && result->next) || (!result && strcmp(version, "HTTP/1.1") == 0)) {  // s'il y a 0 ou plusieurs header host
        printf("iciii\n");
        sendErrorCode(root, clientId, 400, "Bad Request");
        return 0;
    }

    char *host = getHostTarget(root);

    if (host && strcmp(host, HOST1) && strcmp(host, HOST2)) {
        printf("laaaa\n");
        sendErrorCode(root, clientId, 400, "Bad Request");
        return 0;
    }

    free(version);
    free(host);

    return 1;
}

int check_Range_Header(Node *root, int clientId) {
    char *path = getFilePath(root);

    if (isVideoContent(path)) {
        char *version = getHeaderValue(root, "HTTP_version");

        if (strcmp(version, "HTTP/1.0") == 0) {
            free(version);
            free(path);

            sendErrorCode(root, clientId, 400, "Requires HTTP/1.1 or newer");
            return 0;
        }

        free(version);
        free(path);

        if (!isRange(root)) {
            sendErrorCode(root, clientId, 400, "Requires Range Header");
            return 0;
        }
    }

    return 1;
}

int check_headers(Node *root, int clientId) {
    return check_Host_Header(root, clientId) && check_Range_Header(root, clientId);
}

void getFileExtension(char *ptr, char *output) {
    for (int i = 0; ptr[i] != '\0'; i++) {
        if (ptr[i] == '.') {
            strcpy(output, &ptr[i]);
            break;
        }
    }
}

unsigned char *getFileData(char *path, int *size) {
    FILE *file = fopen(path, "rb");

    if (!file) {
        return NULL;
    }

    printf("\"%s\" trouvé\n", path);
    fseek(file, 0L, SEEK_END);
    *size = ftell(file);
    rewind(file);

    unsigned char *buffer = (unsigned char *) malloc(*size);

    fread(buffer, *size, 1, file);
    fclose(file);

    return buffer;
}

char *getDirectoryRepresentationHTML() {  // renvoie la liste des fichiers présents dans le dossier demandé cf : challenge01.root-me.org
    return NULL;
}

char *detect_MIME_type(char *path) {
    char extension[50];
    getFileExtension(path, extension);
    printf("extension : %s\n", extension);

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

void send_status_line(Node *root, int clientId, int statusCode, char *message) {
    char *version = getHeaderValue(root, "HTTP_version");
    char statusLine[150];

    sprintf(statusLine, TEMPLATE_STATUS_LINE, version, statusCode, message);
    writeDirectClient(clientId, statusLine, strlen(statusLine));
    free(version);
}

void sendErrorCode(Node *root, int clientId, int errorCode, char *errorMessage) {
    char message[150];

    sprintf(message, TEMPLATE_ERROR, errorCode, errorMessage, errorCode, errorMessage);

    send_status_line(root, clientId, errorCode, errorMessage);
    send_Content_Type_Header(clientId, "text/html");
    send_Date_Header(clientId);
    send_Server_Header(clientId);
    writeDirectClient(clientId, "\r\n", 2);
    writeDirectClient(clientId, message, strlen(message));
}

void send_Content_Type_Header(int clientId, char *mimeType) {
    char message[50];
    sprintf(message, "Content-type: %s\r\n", mimeType);
    writeDirectClient(clientId, message, strlen(message));
}

void send_Date_Header(int clientId) {
    const char days[7][4] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    const char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char message[40];

    time_t t = time(NULL);
    struct tm tm = *gmtime(&t);
    sprintf(message, "Date: %s, %02d %s %d %02d:%02d:%02d GMT\r\n", days[(tm.tm_wday - 1 + 7) % 7], tm.tm_mday, months[(tm.tm_mon - 1 + 12) % 12], tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    writeDirectClient(clientId, message, strlen(message));
}

void send_Server_Header(int clientId) {
    char message[30] = "Server: serveurHTTP/1.0\r\n";

    writeDirectClient(clientId, message, strlen(message));
}

void send_Connection_Header(int clientId, Node *root) {
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

void send_Content_Range_Header(int clientId, int start, int end, int contentSize) {
    char message[50];

    sprintf(message, "Content-Range: bytes %d-%d/%d\r\n", start, end, contentSize);
    writeDirectClient(clientId, message, strlen(message));
}

void send_Accept_Ranges_Header(int clientId) {
    char message[25] = "Accept-Ranges: bytes\r\n";

    writeDirectClient(clientId, message, strlen(message));
}

void send_headers(int clientId, char *path, Node *root) {
    char mimeType[25];

    if (path[strlen(path) - 1] == '/') {  // si on demande un dossier du site
        strcpy(mimeType, "text/html");
    } else {
        strcpy(mimeType, detect_MIME_type(path));
    }

    send_Accept_Ranges_Header(clientId);
    send_Server_Header(clientId);
    send_Date_Header(clientId);
    send_Connection_Header(clientId, root);
    send_Content_Type_Header(clientId, mimeType);
}

void send_message_body(int clientId, char *path) {
    int size;
    unsigned char *buffer = getFileData(path, &size);
    if (buffer) {
        send_Content_Length_Header(clientId, size);

        if (isGet(root)) {
            writeDirectClient(clientId, "\r\n", 2);
            writeDirectClient(clientId, (char *) buffer, size);
        }
        free(buffer);

    }
}

void send_message_body_streaming(int clientId, char *path) {
    int start = -1;
    int end = -1;
    getRangeRange(root, &start, &end);
    printf("%d : %d\n", start, end);

    int size;
    unsigned char *buffer = getFileData(path, &size);

    if (buffer) {
        if (end == -1) {
            end = start + MAX_SIZE_WITHOUT_CHUNK - 1 < size - 1 ? start + MAX_SIZE_WITHOUT_CHUNK - 1 : size - 1;
        }

        int contentLength = end - start + 1;

        send_Content_Range_Header(clientId, start, end, size);
        send_Content_Length_Header(clientId, contentLength);

        if (isGet(root)) {
            unsigned char *bufferToSend = (unsigned char *) malloc(sizeof(char) * (contentLength + 1));

            for (int i = 0; i < contentLength; i++) {
                bufferToSend[i] = buffer[start + i];
            }

            bufferToSend[contentLength + 1] = '\0';
//            printf("strlen : %lu\n", strlen(bufferToSend));

            writeDirectClient(clientId, "\r\n", 2);
            writeDirectClient(clientId, (char *) bufferToSend, contentLength);
        }

        free(buffer);

    }
}

int isVideoContent(char *path) {
    char *mimeType = detect_MIME_type(path);

    return startWith("video", mimeType);
}