#include <stdio.h>
#include <stdlib.h>
#include "include/Node.h"
#include "include/api.h"

int main(int argc, char **argv) {
    printf("ici\n");
    if (argc == 1) {
        printf("Usage: httpParser <file>\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "rb");

    if (!file) {
        printf("Problème fichier !!\n");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    rewind(file);


    char *message = (char *) malloc(sizeof(char) * (length + 1));
    fread(message, sizeof(char), length, file);

    fclose(file);

    int error = parseur(message, length);

    if (error) {
        printf("NOK\n");
        return 0;
    }

    if (argc == 2) {
        printChildren(root, 0);
    } else if (argc == 3) {
        _Token *token = searchTree(NULL, argv[2]);
        printf("searching for %s\n", argv[2]);

        while (token) {
            int length;
            char *start = getElementValue(token->node, &length);
            printf("%s --> FOUND [%.*s]\n", argv[2], length, start);

            token = token->next;
        }

        purgeElement(&token);
    }

    purgeTree(root);

    return 0;
}