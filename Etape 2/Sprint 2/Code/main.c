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

//    char message[500] = "3A;\t\t\t \ttabbCh# = \t\t  \t J$0Wc   \t  \t; 14\t=_k*IDzP  \t\t \t      \t  ;\t\tfE-\t=\t \t  \t\t\"D\"\t\t  \t \t \t \t \t;\t\t\t 6^ioL$c=  \"ŸX\\N\"\t;\tVB= \t \t\"ã\\~©\\²\";  \t\td_lyh\t \t=\t\"\\¥öX\\O\\Ì\" \t\t ;ZPM=   \t   \t\"\\À\\¥ñv\\`t\\^\"";

    fclose(file);

//    printf("%s\n", message);

    char *ptr = message;

    Node *HTTPMessageNode = newNode();
    initNode(HTTPMessageNode, "HTTP_message", ptr, 0);

    int error = detect_HTTP_message(HTTPMessageNode, ptr);
//    printf("Error : %d\n", error);

    if (error) {
        printf("NOK\n");
        return 0;
    }

    printChildren(HTTPMessageNode, 0);
    delTree(HTTPMessageNode);

    return 0;
}