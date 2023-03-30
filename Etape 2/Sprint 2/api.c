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
    start = (Node *) start;

    if (start == NULL) {
        return searchTree(getRootTree(), name);
    } else {
        _Token *liste;

        if (!strcmp(getLabel(start), name)) {
            liste = malloc(sizeof(_Token));
            liste->node = start;
            liste->next = NULL;

            if (getChild(start) == NULL && getBrother(start) == NULL) {
                return NULL;
            }

            if (getChild(start) != NULL) {
                liste->next = searchTree(getChild(start), name);
            }

            if (getBrother(start) != NULL) {
                _Token *p = liste;

                while (p != NULL) {
                    p = p->next;
                }

                p->next = searchTree(getBrother(start), name);
            }

        } else {
            liste = NULL;

            if (getChild(start) != NULL) {
                searchTree(getChild(start), name);
            } else {
                return NULL;
            }

            if (getBrother(start) != NULL) {
                searchTree(getBrother(start), name);
            } else {
                return NULL;
            }
        }

        return liste;


    }
}

// void searchTree_Recursif(void *start, char *name, _Token ** p){

//     if(!strcmp(start->__label,name)){
//         _Token * newElement = malloc(sizeof(_Token));

//         newElement->node = start;
//         newElement->next = NULL;
//         (*p)->next = newElement;
//         (*p) = (*p)->next;
//     }

//     if(start->__child!=NULL){
//         searchTree_Recursif(start->__child, name, p);
//     }

//     if(start->__brother!=NULL){
//         searchTree_Recursif(start->__brother, name, p);
//     }

// }