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

int check_Accept_Header(Node *root, int clientId){

_Token *mediaRanges = searchTree(root, "media_range");

    if (!mediaRanges) {  // si pas de media range (donc pas de Accept Header), on accepte tout les types
        return 1;
    }

    char *mimeType = detect_MIME_type(root);    
    int flag1= flag2 = 0;

    while (mediaRanges.next != NULL) {

        Node *node = mediaRanges->node;

        //On ecrit dans la variable value la chaine de media-range
        int length;
        char *start = getElementValue(node, &length);
        char *value = (char *) malloc(sizeof(char) * (length + 1));
        sprintf(value, "%.*s", length, start);

        if(strcmp(value,"*/*") == 0){
            flag1 = 1;
            flag2 = 1;
        }

        if (startWith(mimeType, value)) { //Si type et sous-type coïncident remplacable par strstr(value, mimeType)
            char *parameter = getHeaderValue(node, "parameter");
            flag1 =1;
        
        }

        //Stockage du type et du sous-type 
        Node *type = getChild(node);
        int lenght_type;
        char *start_type = getElementValue(type, &lenght_type);
        char *value_type = (char *) malloc(sizeof(char) * (lenght_type + 1));
        sprintf(value_type, "%.*s", lenght_type, start);

        Node *subtype = getChild(node);
        int lenght_subtype;
        char *start_subtype = getElementValue(subtype, &lenght_subtype);
        char *value_subtype = (char *) malloc(sizeof(char) * (lenght_subtype + 1));
        sprintf(value_subtype, "%.*s", lenght_subtype, start);

        if (startWith(mimeType, type)) { //Si les types ne coïncident pas
            char *shortenMimeType = strcpy(shortenMimeType, mimeType + lenght_type); //chaine mimeType sans le type 
            if (startWith(shortenMimeType, value_subtype) || value_subtype == '*')
            { //Si le sous-type est accepté
                flag1 = 1;
            }
        }

        if(flag1)
        { //Type et sous-type sont acceptés 
            //On verifie que accept_params ne contient pas q = 0
            _Token accept_params = searchTree(root, "accept_params")

            while(accept_params->next != NULL)
            {
                *start = getElementValue(media_range, &length);
                *value = (char *) malloc(sizeof(char) * (length+1));
                sprintf(value, "%.*s", length, start);

                if(strstr(value,"q=0") == NULL){
                    flag2 = 1;
                }
            }
        }
        media_range = media_range->next;
    }
    // Fin de parcours des media-range

    if(!flag2){ // Si les mime type n'est pas accepté
        sendErrorCode(root, clientId, 406, "Not accepted media type");
    }

    return 0;

}