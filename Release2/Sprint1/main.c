#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "include/api.h"
#include "include/senders.h"
#include "include/checkers.h"
#include "include/getters.h"
#include <signal.h>
#include "include/parseQuery.h"

// for librequest
#include "include/request.h"


Node *root = NULL;

int cliendId = -1;

void handle_SIGINT() {
    endWriteDirectClient(cliendId);
    printf("\nCtrl+C intercepted\n");
    exit(0);
}

void showDebugInfos(message *requete) {
    printf("Demande recue depuis le client %d\n", requete->clientId);
    printf("Client [%d] [%s:%d]\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port));
    printf("Contenu de la demande\n%.*s\n\n", requete->len, requete->buf);
}

void sendFullResponse(int clientId, char *path, char *mimeType) {
    send_status_line(clientId, 200, "OK");
    send_headers(clientId, mimeType);
    send_message_body(clientId, path);
}

void sendPartialResponse(int clientId, char *path, char *mimeType) {
    send_status_line(clientId, 206, "Partial Content");
    send_headers(clientId, mimeType);
    send_message_body_streaming(clientId, path);
}

void sendResponse(int clientId) {
    printf("\n");

    char *path = getFilePath();
    char *mimeType = getMIMEtype(path);

    printf("Path to send : \"%s\"\n", path);

//    if (isStreamable(mimeType)) {
//        sendPartialResponse(clientId, path, mimeType);
//    } else {
    sendFullResponse(clientId, path, mimeType);
//    }

    free(path);
}

int main() {
    signal(SIGINT, handle_SIGINT);
    message *requete;

//    const char input[] = "nom1=valeur1&nom2=valeur2&nom3=valeur3";
//
//    Pair *pairs = parseQuery(input);
//
//    for (int i = 0; i < pairs->count; i++) {
//        printf("Nom: %s, Valeur: %s\n", pairs[i].name, pairs[i].value);
//    }
//
//    free(pairs);
//
//    return 0;

    while (1) {
        printf("en attente\n");

        // on attend la reception d'une requete HTTP
        // requete pointera vers une ressource allouée par librequest
        if ((requete = getRequest(8080)) == NULL) {
            printf("ICI !!");
            return -1;
        }

        printf("#########################################\n");
        // Affichage de debug
        showDebugInfos(requete);
        cliendId = requete->clientId;

        if (parseur(requete->buf, requete->len) == 0) {
//            printChildren(root, 0);
            if (check_request(requete->clientId)) {
                sendResponse(requete->clientId);

            }
        } else {
            printf("Problème requête\n");
        }

        endWriteDirectClient(requete->clientId);

        if (check_Connection_Header()) {
            printf("\nClosing connection\n");
            requestShutdownSocket(requete->clientId);
        }

        // on ne se sert plus de requete à partir de maintenant, on peut donc liberer...
        purgeTree(root);
        freeRequest(requete);

        printf("#########################################\n");
    }

    return (1);
}
