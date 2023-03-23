#include <stdio.h>
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
    char message[500] = "afvdbhkdf6546";

//    fgets(message, 499, file);

    fclose(file);

//    int j = 0;
//    for (unsigned int i = 0x01; i <= 0xFF; i++) {
//        message[j] = (unsigned int) i;
//        j++;
//    }
//
//    message[j] = '\0';

    printf("%s\n", message);

//    return 0;

    Node *parent = newNode();
    initNode(parent, "message", message, 0);

//    for (int i = 0; message[i] != 0x00; i++) {
        int error = detect_cookie_value(parent, message);

        printf("Error : %d : %c : 0x%X\n", error, *(message), *(message));
//    }
    printChildren(parent, 0);
    delTree(parent);

    return 0;
}