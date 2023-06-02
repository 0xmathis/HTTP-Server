#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "include/api.h"
#include "include/senders.h"
#include "include/checkers.h"
#include "include/getters.h"

// for librequest
#include "include/request.h"


Node *root = NULL;

int clientId = -1;

void handle_SIGPIPE() {

}

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
//    printf("Demande recue depuis le client %d\n", requete->clientId);
    printf("Client [%d] [%s:%d] -> %s\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port), buffer);

    FILE *file = fopen("server.log", "a");
    fprintf(file, "[%d] [%s:%d]\n%s\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port), requete->buf);
    fclose(file);
//    printf("Contenu de la demande\n%s\n\n", buffer);
//    printf("Contenu de la demande\n%.*s\n\n", requete->len, requete->buf);
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
    char *path = getFilePath();
    char *mimeType = getMIMEtype(path);

//    printf("Path to send : \"%s\"\n", path);

//    if (isStreamable(mimeType)) {
//        sendPartialResponse(path, mimeType);
//    } else {
    sendFullResponse(path, mimeType);
//    }

    free(path);
}

int main() {
    signal(SIGINT, handle_SIGINT);
    signal(SIGPIPE, handle_SIGPIPE);
    //signal(SIGSEGV, handle_SIGPIPE);
    //signal(SIGABRT, handle_SIGPIPE);

    message *requete;

    while (1) {
//        printf("en attente\n");

        // on attend la reception d'une requete HTTP
        // requete pointera vers une ressource allouée par librequest
        if ((requete = getRequest(8080)) == NULL) {
            printf("ICI !!");
            return -1;
        }

//        printf("#########################################\n");
        // Affichage de debug
        showDebugInfos(requete);
        clientId = requete->clientId;

        if (parseur(requete->buf, requete->len) == 0 && check_request(requete->clientId)) {
            //printChildren(root, 0);
            sendResponse();
        }
//        else {
//            printf("Problème requête\n");
//        }

        endWriteDirectClient(requete->clientId);

        if (check_Connection_Header()) {
//            printf("\nClosing connection\n");
            requestShutdownSocket(requete->clientId);
        }

        // on ne se sert plus de requete à partir de maintenant, on peut donc liberer...
        purgeTree(root);
        freeRequest(requete);

//        printf("#########################################\n");
    }

    return (1);
}
