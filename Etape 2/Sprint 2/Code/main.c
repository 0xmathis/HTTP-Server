#include <stdio.h>
#include <stdlib.h>
#include "Node/Node.h"
#include "Parser/Josias.h"
#include "api.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Usage: httpParser <file>\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "rb");

    if (file == NULL) {
        printf("Problème fichier !!\n");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    int taille = ftell(file);
    rewind(file);


    char *message = (char *) malloc(sizeof(char) * (taille + 1));
    fread(message, sizeof(char), taille, file);

    fclose(file);

    char *ptr = message;

    root = newNode();
    initNode(root, "HTTP_message", ptr, 0);

    int error = detect_HTTP_message(root, ptr);

    if (error) {
        printf("NOK\n");
        return 0;
    }

    printChildren(root, 0);

    if (argc == 2) {
    } else if (argc == 3) {
        _Token *token = searchTree(root, argv[2]);
        printf("%ld\n", token);


//        while (token != NULL) {
//            printf("%s\n", getLabel(((Node *)token->node)));
//            token = token->next;
//        }
    }


//    delTree(root);

    return 0;
}