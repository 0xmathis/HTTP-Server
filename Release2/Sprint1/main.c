#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/api.h"
#include "include/checkers.h"
#include "include/getters.h"
#include "include/others.h"
#include "include/senders.h"

// for librequest
#include "include/request.h"


Node *root = NULL;
int clientId = -1;

void handle_SIGPIPE() {}

void handle_SIGINT() {
    endWriteDirectClient(clientId);
    printf("\nCtrl+C intercepted\n");
    exit(0);
}

void showDebugInfos(message *requete) {
    char bufferCopyShort[200] = {0};
    char buffer[200] = {0};
    strncpy(bufferCopyShort, requete->buf, 199);
    sscanf(bufferCopyShort, "%[^\r\n]", buffer);
    printf("Client [%d] [%s:%d] -> %s\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port), buffer);

    FILE *file = fopen("server.log", "a");
    fprintf(file, "[%d] [%s:%d]\n%s\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port), requete->buf);
    fclose(file);
}

void sendFullResponse(char *path, char *mimeType) {
    send_status_line(clientId, 200, "OK");
    send_headers(clientId, path, mimeType);
    send_message_body(clientId, path);
}

void sendPartialResponse(char *path, char *mimeType) {
    send_status_line(clientId, 206, "Partial Content");
    send_headers(clientId, path, mimeType);
    send_message_body_streaming(clientId, path);
}

void sendResponse() {
    char *path = getFilePath(NULL);
    char *mimeType = getMIMEtype(path);

    if (isStreamable(mimeType)) {
        sendPartialResponse(path, mimeType);
    } else {
        sendFullResponse(path, mimeType);
    }

    free(path);
}

int main() {
    signal(SIGINT, handle_SIGINT);
    signal(SIGPIPE, handle_SIGPIPE);

    message *requete;

    while (1) {
        // on attend la reception d'une requete HTTP
        // requete pointera vers une ressource allouée par librequest
        if ((requete = getRequest(8080)) == NULL) {
            printf("ICI !!");
            return -1;
        }

        // Affichage de debug
        showDebugInfos(requete);
        clientId = requete->clientId;

        if (parseur(requete->buf, requete->len) == 0) {
//            printChildren(root, 0);

            if (check_request(requete->clientId)) {
                sendResponse();
            }
        }

        endWriteDirectClient(requete->clientId);

        if (check_Connection_Header()) {
            requestShutdownSocket(requete->clientId);
        }

        // on ne se sert plus de requete à partir de maintenant, on peut donc liberer...
        purgeTree(root);
        freeRequest(requete);
    }
}
