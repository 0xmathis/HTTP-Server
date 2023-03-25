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
    fread(message, sizeof(char), taille, file);

    //
//    char message[500] = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0 (\"comment\") ";

    fclose(file);

//    printf("%s\n", message);

    char *ptr = message;

    Node *HTTPMessageNode = newNode();
    initNode(HTTPMessageNode, "HTTP_message", ptr, 0);

    int error = detect_HTTP_message(HTTPMessageNode, ptr);
//    printf("Error : %d\n", error);

    if (error) {
//        printf("NOK\n");
//        return 0;
    }
//    int error = detect_Accept_Encoding_header(HTTPMessageNode, ptr);
//    printf("Error : %d\n", error);

    printChildren(HTTPMessageNode, 0);
    delTree(HTTPMessageNode);

    return 0;
}