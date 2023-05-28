#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "include/api.h"
#include "include/Mathis.h"
#include "include/Hugo.h"


// for librequest
#include "include/request.h"

#define ERROR "HTTP/1.0 400 SUCKA\r\n\r\n"
#define REPONSE "HTTP/1.0 200 OK\r\n"

// TODO: Personnaliser la première ligne de la réponse
// TODO: Percent Encoding (RFC3986)
// TODO: Dot Segment Removal (RFC3986)
// TODO: Pas de content length -> transfer-encoding = chunked
// TODO: Être capable de parser/créer du chunked encoding
// TODO: Si on applique un autre transfer-encoding que chunked dans la requête, le client doit appliquer chunked à la fin
// TODO: Si on applique un autre transfer-encoding que chunked dans la réponse, le serveur doit soit appliquer chunked à la fin, soit fermer la connection après l'envoie
// TODO: Transfer-coding inconnu -> erreur 501
// TODO: Si HTTP/1.0 -> Pas de Transfer-Encoding dans la réponse du serveur
// TODO: faire une fonction séparée qui envoie les vidéos en streaming (donc pas toute la vidéo d'un coup) on doit envoyer un Range-header (voir la vidéo sur le streaming dans l'historique youtube)

Node *root = NULL;

void showDebugInfos(message *requete) {
    printf("Demande recue depuis le client %d\n", requete->clientId);
    printf("Client [%d] [%s:%d]\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port));
    printf("Contenu de la demande\n%.*s\n\n", requete->len, requete->buf);
}

void sendFullResponse(int clientId, char *path) {
    send_status_line(root, clientId, 200, "OK");
    send_headers(clientId, path, root);
    send_message_body(clientId, path);
}

void sendPartialResponse(int clientId, char *path) {
    send_status_line(root, clientId, 206, "Partial Content");
    send_headers(clientId, path, root);
    send_message_body_streaming(clientId, path);
}

void sendResponse(int clientId) {
    char *path = getFilePath(root);

    if (isVideoContent(path)) {
        sendPartialResponse(clientId, path);
    } else {
        sendFullResponse(clientId, path);
    }

    printf("\"%s\" envoyé\n", path);
    free(path);
}


int main() {
    message *requete;

//    check_path(root,0);
//
//    return 1;

    while (1) {
        printf("en attente\n");

        // on attend la reception d'une requete HTTP
        // requete pointera vers une ressource allouée par librequest
        if ((requete = getRequest(8080)) == NULL) return -1;

        printf("#########################################\n");
        // Affichage de debug
        showDebugInfos(requete);

        if (parseur(requete->buf, requete->len) == 0) {
//            printChildren(root, 0);
            if (check_request(root, requete->clientId)) {
                printf("ici\n");
                sendResponse(requete->clientId);

                purgeTree(root);
            }
        } else {
            printf("Problème requête\n");
        }

        endWriteDirectClient(requete->clientId);

//        if(!check_connection(root, requete->clientId)){
//            requestShutdownSocket(requete->clientId);
//        }

        // on ne se sert plus de requete à partir de maintenant, on peut donc liberer...
        freeRequest(requete);

        printf("#########################################\n");
    }

    return (1);
}
