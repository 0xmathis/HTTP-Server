#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../Release1/Sprint2/include/api.h"

// for librequest
#include "include/request.h"

#define ERROR "HTTP/1.0 400 SUCKA\r\n\r\n"
#define REPONSE "HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n"

Node *root = NULL;

void showDebugInfos(message *requete) {
    printf("#########################################\nDemande recue depuis le client %d\n", requete->clientId);
    printf("Client [%d] [%s:%d]\n", requete->clientId, inet_ntoa(requete->clientAddress->sin_addr), htons(requete->clientAddress->sin_port));
    printf("Contenu de la demande\n%.*s\n\n", requete->len, requete->buf);
}

void sendTestFile(int clientId, char *path) {
    char realPath[100] = ".";
    strcat(realPath, path);

    FILE *file = fopen(realPath, "rb");

    if (!file) {
        printf("Problème fichier");
        return;
    }

    printf("%s trouvé\n", realPath);
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file);

    unsigned char *buffer = (unsigned char *) malloc(size);

    fread(buffer, size, 1, file);
    fclose(file);

    writeDirectClient(clientId, (char *) buffer, size);

    free(buffer);
    printf("%s envoyé\n", realPath);
}

void sendTest(int clientId) {
    FILE *file = fopen("testSite/test.html", "rb");

    if (!file) {
        printf("Problème fichier");
        return;
    }

    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file);

    unsigned char *buffer = (unsigned char *) malloc(size);

    fread(buffer, size, 1, file);
    fclose(file);

    writeDirectClient(clientId, (char *) buffer, size);

    free(buffer);
}

void sendImageTest(int clientId) {
    FILE *file = fopen("testSite/ecureuil.jpg", "rb");

    if (!file) {
        printf("Problème image");
        return;
    }

    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file);

    unsigned char *buffer = (unsigned char *) malloc(size);

    fread(buffer, size, 1, file);
    fclose(file);

    writeDirectClient(clientId, (char *) buffer, size);

    free(buffer);
}

void sendBackRequest(int clientId) {
    _Token *result = searchTree(NULL, "HTTP_message");
    _Token *token = result;

    while (token) {
        Node *node = (Node *) token->node;
        writeDirectClient(clientId, (char *) getStart(node), getLength(node));
        token = token->next;
    }

    purgeElement(&result);
}

int main() {
    message *requete;

    while (1) {
        // on attend la reception d'une requete HTTP requete pointera vers une ressource allouée par librequest.
        if ((requete = getRequest(8080)) == NULL) return -1;

        // Affichage de debug
        showDebugInfos(requete);

        if (!parseur(requete->buf, requete->len)) {
            writeDirectClient(requete->clientId, REPONSE, strlen(REPONSE));

            _Token *result = searchTree(NULL, "request_target");

            Node *node = (Node *) result->node;
            int length;
            char *start = getElementValue(node, &length);
            char path[40];
            sprintf(path, "%.*s", length, start);

            sendTestFile(requete->clientId, path);

            purgeTree(root);
        } else {
            writeDirectClient(requete->clientId, ERROR, strlen(ERROR));
        }

        endWriteDirectClient(requete->clientId);
        requestShutdownSocket(requete->clientId);
        // on ne se sert plus de requete à partir de maintenant, on peut donc liberer...
        freeRequest(requete);
    }

    return (1);
}
