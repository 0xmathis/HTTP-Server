#include <stdio.h>
#include "Node/Node.h"
#include "Parser/Mathis.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Usage: httpParser <file>\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "r");
    char message[500];

    fgets(message, 499, file);

    fclose(file);

    printf("%s\n", message);

    Node *parent = newNode();
    initNode(parent, "message", message, 0);

    int error = detect_product(parent, message);

    printf("Error : %d\n", error);

    printChildren(parent, 0);
    delTree(parent);
}