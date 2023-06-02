#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/api.h"
#include "../include/senders.h"
#include "../include/others.h"
#include "../include/request.h"
#include "../include/constantes.h"
#include <time.h>
#include "../include/getters.h"
#include "../include/fastCGI.h"


void send_Accept_Ranges_Header(int clientId) {
    char message[25] = "Accept-Ranges: bytes\r\n";

    writeDirectClient(clientId, message, strlen(message));
}

void send_Connection_Header(int clientId) {
    char *connectionOption = getHeaderValue(root, "connection_option");

    if (connectionOption) {
        char message[30];
        snprintf(message, 29, "Connection: %s\r\n", connectionOption);

        if (strcmp(connectionOption, "keep-alive") == 0 || strcmp(connectionOption, "close") == 0) {
            writeDirectClient(clientId, message, strlen(message));
        }

        free(connectionOption);
    }
}

//void send_Content_Encoding_Header(int clientId) {
//    char *encoding = getEncoding();
//
//    if (encoding) {
//        char message[50];
//        sprintf(message, "Content-encoding: %s\r\n", encoding);
//        writeDirectClient(clientId, message, strlen(message));
//        free(encoding);
//    }
//
//}

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

void send_Content_Type_Header(int clientId, char *mimeType) {
    char message[100];
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

void send_error_code(int clientId, int errorCode, char *errorMessage) {
    char message[200];

    snprintf(message, 199, TEMPLATE_ERROR, errorCode, errorMessage, errorCode, errorMessage);

    send_status_line(clientId, errorCode, errorMessage);
    send_Content_Type_Header(clientId, "text/html");
    send_Date_Header(clientId);
    send_Server_Header(clientId);
    writeDirectClient(clientId, "\r\n", 2);

    if (isGet()) {
        writeDirectClient(clientId, message, strlen(message));
    }

    fflush(stdout);
}

void send_headers(int clientId, char *mimeType) {
//    char mimeTypeToSend[25];

//    if (path[strlen(path) - 1] == '/') {  // si on demande un dossier du site
//        strcpy(mimeTypeToSend, "text/html");
//    } else {
//        strcpy(mimeTypeToSend, mimeType);
//    }

    send_Accept_Ranges_Header(clientId);
    send_Server_Header(clientId);
    send_Date_Header(clientId);
    send_Connection_Header(clientId);

    if (!isPHP()) {
        send_Content_Type_Header(clientId, mimeType);
    }

//    send_Content_Encoding_Header(clientId);
}

void send_message_body(int clientId, char *path) {
    if (isPHP()) {
        return send_message_body_php(clientId, path);
    }

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

void send_message_body_php(int clientId, char *path) {
    int fd;
    FCGI_Header header;
    char *pwd = getPWD();

    if (!pwd) {
        // printf("Problème PWD\n");
        return;
    }

    strcat(pwd, "/");
    strcat(pwd, path);
    // printf("PWD : %s\n", pwd);

    send_PHP_request(&fd, &header, pwd);
    send_PHP_answer(clientId, fd);

    free(pwd);
}

void send_message_body_chunked(int clientId, char *path) {
    // printf("chunking\n");
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
    // printf("streaming\n");
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

void send_Server_Header(int clientId) {
    char message[30] = "Server: serveurHTTP/1.0\r\n";

    writeDirectClient(clientId, message, strlen(message));
}

void send_status_line(int clientId, int statusCode, char *message) {
    char *version = getHeaderValue(root, "HTTP_version");
    char statusLine[150];

    if (strcmp(version, "HTTP/1.0") && strcmp(version, "HTTP/1.1")) { // Si on nous demande au-dessus de 1.1
        sprintf(statusLine, TEMPLATE_STATUS_LINE, "HTTP/1.1", statusCode, message);
    } else {
        sprintf(statusLine, TEMPLATE_STATUS_LINE, version, statusCode, message);
    }

    printf("\t-> %s", statusLine);

    fflush(stdin);
    fflush(stdout);

    writeDirectClient(clientId, statusLine, strlen(statusLine));
    free(version);
}

void send_Transfer_Encoding_Header(int clientId, char *encoding) {
    char message[50];
    sprintf(message, "Transfer-Encoding: %s\r\n", encoding);
    writeDirectClient(clientId, message, strlen(message));
}
