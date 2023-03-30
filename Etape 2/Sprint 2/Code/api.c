#include "api.h"
#include <string.h>
#include <stdlib.h>

Node *root = NULL;

void *getRootTree() {
    return root;
}

char *getElementTag(void *node, int *len) {
    Node *noeud = (Node *) node;

    if (len != NULL) {
        *len = getLength(noeud);
    }

    return (char *) getLabel(noeud);
}

char *getElementValue(void *node, int *len) {
    Node *noeud = (Node *) node;

    if (len != NULL) {
        *len = getLength(noeud);
    }

    return (char *) getStart(noeud);
}

void purgeTree(void *root) {
    Node *racine = (Node *) root;

    if (racine->__child != NULL) {
        purgeTree(racine->__child);
    }
    if (racine->__brother != NULL) {

        purgeTree(racine->__brother);
    }

    free(racine);
}

void purgeElement(_Token **r) {
    if ((*r)->next != NULL) {
        purgeElement(&((*r)->next));
    }

    free(*r);
}

_Token *searchTree(void *start, char *name) {
    _Token * liste = malloc(sizeof(_Token));
    liste->node = NULL;
    liste->next = NULL;

    if (start == NULL) {
        return searchTree(getRootTree(), name);
    } else {
        searchTree_Recursif(start, name, liste);
        
    }

    if(liste->next == NULL){
        return NULL;
    }
    else{
        return liste;
    }
}

void insertion(_Token *liste, Node *newNode)
{
    _Token *newToken = malloc(sizeof(_Token));
    newToken-> node = newNode;
    newToken-> next = NULL;

    if(liste->node == NULL){
        
        liste->node = newNode;
    }
    else{
        newToken->next = liste->next;
        liste->next = newToken;
    }
}

void searchTree_Recursif(void *start, char *name, _Token * liste){

    Node * noeud = (Node *) start;

    if(!strcmp(noeud->__label,name)){
        
        insertion(liste, noeud);
       
    }

    if(noeud->__child!=NULL){
        searchTree_Recursif(noeud->__child, name, liste);
    }

    if(noeud->__brother!=NULL){
        searchTree_Recursif(noeud->__brother, name, liste);
    }

}