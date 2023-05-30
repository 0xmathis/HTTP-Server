#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zlib.h>
#include "../include/api.h"
#include "../include/Josias.h"
#include "../include/Mathis.h"
#include "../include/Nathan.h"
#include "../include/Hugo.h"
#include "../include/constantes.h"
#include "../include/request.h"

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

// Getters

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


// Checkers

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
            sendErrorCode(clientId, 501, "Unknown transfer coding");
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


// Senders

void send_status_line(int clientId, int statusCode, char *message) {
    char *version = getHeaderValue(root, "HTTP_version");
    char statusLine[150];

    sprintf(statusLine, TEMPLATE_STATUS_LINE, version, statusCode, message);
    writeDirectClient(clientId, statusLine, strlen(statusLine));
    free(version);
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
    send_Content_Type_Header(clientId, mimeType);
//    send_Content_Encoding_Header(clientId);
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


// Others

void remove_dot_segments(char *path) {
    printf("Removing dot segments\n");
//    printf("path : %s\n", path);

    for (int i = 0; i < (int) strlen(path) - 1; i++) {
        // Si on rencontre /./ on l'enlève
        if (path[i] == '/' && path[i + 1] == '.' && (i + 2 >= (int) strlen(path) || path[i + 2] == '/')) {
            for (int j = i; j < (int) strlen(path) - 2; j++) {
                path[j] = path[j + 2];
            }

            i--;
            //printf("%s\n", path);
        }

        // Si on rencontre /../ on l'enleve si il est pas au début on enlève le répertoire précédent
        if (path[i] == '/' && path[i + 1] == '.' && path[i + 2] == '.' && (i + 3 >= (int) strlen(path) || path[i + 3] == '/')) {
            int temp = i;

            if (i > 0) {
                i--;
            }

            while (path[i] != '/' && i != 0) {
                i--;
            }

            for (int j = i; j < (int) strlen(path) - 1; j++) {
                path[j] = path[j + 3 + (temp - i)];
            }

            i--;
        }
    }
}
