#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/api.h"
#include "../include/constantes.h"
#include "../include/Mathis.h"
#include <../include/Nathan.h>
#include "../include/request.h"
#include "../include/utils.h"

int check_accept_encoding(Node *root, int clientId) {

    _Token *codings = searchTree(root, "codings");
    int length;
    char *start = getElementValue(codings, &length);
    char *value = (char *) malloc(sizeof(char) * length+1);
    sprintf(value, "%.*s", length, start);
    const char *encodings[] = {'*',"gzip", "deflate", "br", "identity"};

    if (codings == NULL) {
        return 0;

    } else {

        if (strstr(value, "q=0")==NULL) {
            for (int i = 0; i < 5; i++) {
                if ((strstr(value, encodings[i])) != NULL) {
                    return 0;
                }
            }
        }

        while (codings->next != NULL) {

            start = getElementValue(codings, &length);
            value = (char *) malloc(sizeof(char) * length+1);
            sprintf(value, "%.*s", length, start);
            
            if (strstr(value, "q=0")==NULL) {
                for (int i = 0; i < 5; i++) {
                    if ((strstr(value, encodings[i])) != NULL) {
                        return 0;
                    }
                }
            }
        }
    }

    sendErrorCode(root, clientId, 406, "ALED");
    return 1;
}

char *getEncoding(Node *root, int clientId) {

    _Token *codings = searchTree(root, "codings");
    int length;

    char *start = getElementValue(codings, &length);
    char *value = (char *) malloc(sizeof(char) * length+1);
    sprintf(value, "%.*s", length, start);

    int flag = 0;
    int qmax = 1;
    const char *encodings[] = {'*',"gzip", "deflate", "br", "identity"};
    char *encoding = malloc(50 * sizeof(char));
    char *index;

    if (codings == NULL) {
        return 0;

    } else {

        if (strstr(value, "q=0")==NULL) {

            for (int i = 0; i < 5; i++) {

                if ((strstr(value, encodings[i])) != NULL) {
                    index = strstr(value, "q=");
                    if (index != NULL) {

                        if (value[atoi(index)+2] > qmax) {
                            qmax = value[atoi(index)+2];
                            strcpy(encoding, encodings[i]);
                            flag = 1;

                        } else {
                            if (flag != 1) {
                                strcpy(encoding, encodings[i]);
                                flag = 1;
                            }
                        }

                    } else {
                        if (flag != 1) {
                            strcpy(encoding, encodings[i]);
                            flag = 1;
                        }
                    }
                }
            }
        }

        while (codings->next != NULL) {

            start = getElementValue(codings, &length);
            value = (char *) malloc(sizeof(char) * length+1);
            sprintf(value, "%.*s", length, start);
            
            if (strstr(value, "q=0")==NULL) {

                for (int i = 0; i < 5; i++) {

                    if ((strstr(value, encodings[i])) != NULL) {
                        index = strstr(value, "q=");
                        if (index != NULL) {

                            if (value[atoi(index)+2] > qmax) {
                                qmax = value[atoi(index)+2];
                                strcpy(encoding, encodings[i]);
                                flag = 1;

                            } else {
                                if (flag != 1) {
                                    strcpy(encoding, encodings[i]);
                                    flag = 1;
                                }
                            }

                        } else {
                            if (flag != 1) {
                                strcpy(encoding, encodings[i]);
                                flag = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    if (flag != 1){
        sendErrorCode(root, clientId, 406, "Pas d'encoding accepté");
        return "No accepted encoding";
    } else {
        return encoding;
    }
    
}

void send_Encoding(int clientId, Node *root) {
    char *encoding = getEncoding(root, clientId);

    if (encoding) {
        char message[50];
        sprintf(message, "Content-type: %s\r\n", encoding);
        writeDirectClient(clientId, message, strlen(message));
        free(encoding);
    }
    
}

int check_accept_language(Node *root, int clientId) {

    _Token *accept_language = searchTree(root, "accept-language");
    int length;

    char *start = getElementValue(accept_language, &length);
    char *value = (char *) malloc(sizeof(char) * length+1);
    sprintf(value, "%.*s", length, start);

    while (accept_language->next != NULL) {
        
        if (strcmp(value, "fr") == 0 || strcmp(value,'fr-FR') == 0) {
            return 0;
        }
    }

    sendErrorCode(root, clientId, 406, "Le français n'est pas dans la liste des langages acceptés");
    return 1;

}
