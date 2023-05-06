#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/api.h"
#include "../include/constantes.h"
#include "../include/Mathis.h"
#include "../include/request.h"
#include "../include/utils.h"

int check_request(Node *root, int clientId) {
    if (!check_method(root, clientId) || !check_path(root, clientId)) {
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

int check_path(Node *root, int clientId) {
    char *path = getFilePath(root);
    FILE *file = fopen(path, "rb");
    printf("path : %s\n", path);

    if (!file) {
        printf("Problème fichier\n");
        sendErrorCode(root, clientId, 404, "Not Found");
        return 0;
    }

    fclose(file);
    free(path);

    return 1;
}

int isGet(Node *root) {
    char *method = getHeaderValue(root, "method");

    if (strcmp(method, "GET") == 0) {
        free(method);
        return 1;
    }

    free(method);
    return 0;
}

char *getHeaderValue(Node *root, char *headerValue) {
    _Token *result = searchTree(root, headerValue);

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

char *getFilePath(Node *root) {
    char *fullPath = (char *) malloc(sizeof(char) * 200);
    char *path = getHeaderValue(root, "request_target");

    if (strlen(path) == 1 && *path == '/') {  // si on demande la racine du site
        sprintf(fullPath, "%s%sindex.html", PATH, path);
    } else {
        sprintf(fullPath, "%s%s", PATH, path);
    }

    free(path);

    return fullPath;
}

void getFileExtension(char *ptr, char *output) {
    for (int i = 0; ptr[i] != '\0'; i++) {
        if (ptr[i] == '.') {
            strcpy(output, &ptr[i]);
            break;
        }
    }
}

unsigned char *getDataFromFile(char *path, int *size) {
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

char *detect_MIME_type(Node *root) {
    char *path = getFilePath(root);
    char extension[50];
    getFileExtension(path, extension);
    printf("extension : %s\n", extension);
    free(path);

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
        return "image/vnd.microsoft.icon";
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

    return "text/plain";
}

void sendErrorCode(Node *root, int clientId, int errorCode, char *errorMessage) {
    char *version = getHeaderValue(root, "HTTP_version");
    char statusLine[150], message[150];

    sprintf(statusLine, TEMPLATE_STATUS_LINE, version, errorCode, errorMessage);
    sprintf(message, TEMPLATE_ERROR, errorCode, errorMessage, errorCode, errorMessage);

    writeDirectClient(clientId, statusLine, strlen(statusLine));
    send_Date_Header(clientId);
    send_Server_Header(clientId);
    writeDirectClient(clientId, "\r\n", 2);
    writeDirectClient(clientId, message, strlen(message));

    free(version);
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
    sprintf(message, "Date: %s, %02d %s %d %02d:%02d:%02d GMT\r\n", days[tm.tm_wday - 1], tm.tm_mday, months[tm.tm_mon - 1], tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

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

void send_headers(int clientId, char *path, Node *root) {
    char mimeType[25];

    if (path[strlen(path) - 1] == '/') {  // si on demande un dossier du site
        strcpy(mimeType, "text/html");
    } else {
        strcpy(mimeType, detect_MIME_type(root));
    }

    send_Server_Header(clientId);
    send_Date_Header(clientId);
    send_Connection_Header(clientId, root);
    send_Content_Type_Header(clientId, mimeType);
    writeDirectClient(clientId, "\r\n", 2);
}

void send_message_body(int clientId, char *path) {
    if (path[strlen(path) - 1] == '/') {  // si on demande un dossier du site

    } else {
        int size;
        unsigned char *buffer = getDataFromFile(path, &size);
        if (buffer) {
            writeDirectClient(clientId, (char *) buffer, size);
            free(buffer);
        }
    }
}

int isConnectionToClose(Node *root) {
    char *connection = getHeaderValue(root, "connection_option");

    printf("connection : %s\n", connection);

    return 0;
}