#include <stdio.h>
#include <stdlib.h>
#include "Node/Node.h"
#include "Parser/Mathis.h"
#include "Parser/Nathan.h"
#include "Parser/Josias.h"
#include "Parser/Hugo.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Usage: httpParser <file>\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("Problème fichier !!\n");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    int taille = ftell(file);
    rewind(file);


    char *message = (char *) malloc(sizeof(char) * (taille + 1));

//    fgets(message, 499, file);
//    fscanf(file, "%[^'\r\n\r\n']", message);

    fread(message, sizeof(char), taille, file);

    fclose(file);

    printf("%s\n", message);

    char *ptr = message;

    Node *HTTPMessageNode = newNode();
    initNode(HTTPMessageNode, "HTTP_message", ptr, 0);

    int error = detect_HTTP_message(HTTPMessageNode, ptr);
    printf("Error : %d\n", error);

    printChildren(HTTPMessageNode, 0);
    delTree(HTTPMessageNode);

    return 0;
}