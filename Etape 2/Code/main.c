#include <stdio.h>
#include "Node/Node.h"
#include "Parseur/parseur.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Usage: abnf <file>\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "r");
    char message[401];

    fgets(message, 400, file);

    fclose(file);

    Node *parent = newNode();
    initNode(parent, "message", message, getStringLength(message));

    int error = startParser(parent, message);

    if (error != 0) {
        printf("remaining #%s#\n", message);
        return error;
    }

    printf("remaining ##\n");
    printChildren(parent, 0);
    int i = 0;
    delTree(parent, &i);
    // printf("Nb node deleted : %d\n", i);

    return error;

}