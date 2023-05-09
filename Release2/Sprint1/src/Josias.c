#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/api.h"
#include "../include/constantes.h"
#include "../include/Mathis.h"
#include "../include/request.h"
#include "../include/utils.h"

//int check_Accept_Header(Node *root, int clientId) {
//    //OPTIONNAL
//    _Token *mediaRanges = searchTree(root, "media_range");
//
//    if (!mediaRanges) {  // si pas de Accept Header (donc pas de media-range), on accepte tout les types
//        return 1;
//    }
//
//    char *mimeType = detect_MIME_type(root);
//    int flag = 0;
//
//    while (mediaRanges && !flag) {
//        Node *node = mediaRanges->node;
//
//        int length;
//        char *start = getElementValue(node, &length);
//        char *value = (char *) malloc(sizeof(char) * (length + 1));
//        sprintf(value, "%.*s", length, start);
//
//        printf("media-range : %s\n", value);
//
//        if (startWith(mimeType, value)) {
//            char *parameter = getHeaderValue(node, "parameter");
//
//            printf("parameter : %s\n", parameter);
//            free(parameter);
//        }
//
//
//
//        mediaRanges = mediaRanges->next;
//
////        if (getBrother(node) && strcmp("accept_params", getLabel(getBrother(node))) == 0) {
////
////        }
////        if (strcmp(value, mimeType) == 0) {
////            flag = 1;
////        }
//
//
////        break;
//    }
//
//    //RECOMMENDED
//    _Token *accept_params = searchTree(root, "accept_params");
//
//
////    while (accept_params) {
////        *start = getElementValue(accept_params, &length);
////        *value = (char *) malloc(sizeof(char) * (length + 1));
////        sprintf(value, "%.*s", length, start);
////
////        if (strcmp(value, mimeType) == 0) {
////            flag = 1;
////        }
//
////    }
//    if (!flag) {//Si le type de la ressource ne fait pas parti de ceux acceptés par le client
//        sendErrorCode(root, clientId, 406, "Not accepted media type");
//        return 0;
//    }
//
////    //Accept-header
////    _Token *accept_header = searchTree(root, "accept_header");
////    if (accept_header != NULL) {//Si on a un accept header
////
////    }
//
//
////    int length;
////    char *start = getElementValue(accept_header, &length);
////    char *value = (char *) malloc(sizeof(char) * (length + 1));
////    sprintf(value, "%.*s", length, start);
//
//
//    free(mimeType);
//    return 1;
//}

int check_accept(Node *root, int clientId){

    //OPTIONNAL
    _Token *media_range = searchTree(root, "media_range");

    //RECOMMENDED
    _Token *accept_params = searchTree(root, "accept_params");
    int length;
    char *start = getElementValue(accept_params, &length);
    char *value = (char *) malloc(sizeof(char) * (length+1));

    bool flag = false;
    while(accept_params->next != NULL){

        *start = getElementValue(accept_params, &length);
        *value = (char *) malloc(sizeof(char) * (length+1));
        sprintf(value, "%.*s", length, start);

        if(strcmp(value, detect_MIME_type(root)) == 0){
            flag = true;
        }

    }
    if(!flag)
    {//Si le type de la ressource ne fait pas parti de ceux acceptés par le client
        sendErrorCode(root, clientId, 406, "Not accepted media type");
        return 1;
    }

    //Accept-header
    _Token *accept_header = searchTree(root, "accept_header");
    if(accept_header != NULL)
    {//Si on a un accept header

    }


    int length;
    char *start = getElementValue(accept_header, &length);
    char *value = (char *) malloc(sizeof(char) * (length+1));
    sprintf(value, "%.*s", length, start);











}