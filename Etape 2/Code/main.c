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
    initNode(parent, "message", &message[0], getStringLength(message));

    int error = startParser(parent, NULL, message);

    if (error != 0) {
        printf("NOT OK\n");
        return error;
    }

    printf("OK\n");
    printChildren(parent, 0);

    return error;

}