#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "include/api.h"
#include "include/Mathis.h"
#include "include/constantes.h"
#include <time.h>


// for librequest
#include "include/request.h"

#define ERROR "HTTP/1.0 400 SUCKA\r\n\r\n"
#define REPONSE "HTTP/1.0 200 OK\r\n"

// TODO : Content-Length-Header
// TODO : Renvoyer le bon site en fonction des valeurs de Host

Node *root = NULL;

void showDebugInfos(message *requete) {
    printf("Demande recue depuis le client %d\n", requete->clientId);
    printf("Client [%d] [%s:%d]\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port));
    printf("Contenu de la demande\n%.*s\n\n", requete->len, requete->buf);
}

void sendTestFile(int clientId) {
    char *path = getFilePath(root);

    send_headers(clientId, path, root);
    send_message_body(clientId, path);

    printf("\"%s\" envoyé\n", path);
    free(path);
}

//void sendTest(int clientId) {
//    FILE *file = fopen("testSite/test.html", "rb");
//
//    if (!file) {
//        printf("Problème fichier");
//        return;
//    }
//
//    fseek(file, 0L, SEEK_END);
//    long size = ftell(file);
//    rewind(file);
//
//    unsigned char *buffer = (unsigned char *) malloc(size);
//
//    fread(buffer, size, 1, file);
//    fclose(file);
//
//    writeDirectClient(clientId, (char *) buffer, size);
//
//    free(buffer);
//}
//
//void sendImageTest(int clientId) {
//    FILE *file = fopen("testSite/ecureuil.jpg", "rb");
//
//    if (!file) {
//        printf("Problème image");
//        return;
//    }
//
//    fseek(file, 0L, SEEK_END);
//    long size = ftell(file);
//    rewind(file);
//
//    unsigned char *buffer = (unsigned char *) malloc(size);
//
//    fread(buffer, size, 1, file);
//    fclose(file);
//
//    writeDirectClient(clientId, (char *) buffer, size);
//
//    free(buffer);
//}
//
//void sendBackRequest(int clientId) {
//    _Token *result = searchTree(NULL, "HTTP_message");
//    _Token *token = result;
//
//    while (token) {
//        Node *node = (Node *) token->node;
//        writeDirectClient(clientId, (char *) getStart(node), getLength(node));
//        token = token->next;
//    }
//
//    purgeElement(&result);
//}

int main() {
    message *requete;

    while (1) {
        printf("en attente\n");

        // on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest.
        if ((requete = getRequest(8080)) == NULL) return -1;

        printf("#########################################\n");
        // Affichage de debug
        showDebugInfos(requete);

        if (!parseur(requete->buf, requete->len)) {
//            printChildren(root, 0);
            if (check_request(root, requete->clientId)) {
                writeDirectClient(requete->clientId, REPONSE, strlen(REPONSE));

                sendTestFile(requete->clientId);

                purgeTree(root);
            }
        } else {
            printf("Problème requête\n");
        }

        endWriteDirectClient(requete->clientId);
        requestShutdownSocket(requete->clientId);
        // on ne se sert plus de requete à partir de maintenant, on peut donc liberer...
        freeRequest(requete);

        printf("#########################################\n");
    }

    return (1);
}
