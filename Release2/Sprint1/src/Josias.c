#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/api.h"
#include "../include/constantes.h"
#include "../include/Mathis.h"
#include "../include/request.h"
#include "../include/utils.h"

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