#include "api.h"
#include <string.h>
#include <stdlib.h>
#include "Parser/Josias.h"

Node *root = NULL;

void *getRootTree() {
    return root;
}

_Token *searchTree(void *start, char *name) {
    _Token *liste = malloc(sizeof(_Token));
    liste->node = NULL;
    liste->next = NULL;

    if (start) {
        searchTree_Recursif(start, name, liste);
    } else {
        return searchTree(getRootTree(), name);
    }

    if (liste->node) {
        return liste;
    } else {
        return NULL;
    }
}

void searchTree_Recursif(void *start, char *name, _Token *liste) {
    Node *noeud = (Node *) start;

    if (strcmp(noeud->__label, name) == 0) {

        insertion(liste, noeud);

    }

    if (noeud->__child) {
        searchTree_Recursif(noeud->__child, name, liste);
    }

    if (noeud->__brother) {
        searchTree_Recursif(noeud->__brother, name, liste);
    }

}

void insertion(_Token *liste, Node *newNode) {
    _Token *newToken = malloc(sizeof(_Token));
    newToken->node = newNode;
    newToken->next = NULL;

    if (liste->node) {
        newToken->next = liste->next;
        liste->next = newToken;
    } else {

        liste->node = newNode;
    }
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

void purgeElement(_Token **r) {
    if (!*r) {
        return;
    }

    if ((*r)->next) {
        purgeElement(&((*r)->next));
    }

    free(*r);
    *r = NULL;
}

void purgeTree(void *root) {
    delTree((Node *) root);
}

int parseur(char *req, int len) {
    root = newNode();
    int error = detect_HTTP_message(root, req);

    if (getLength(root) > len) {
        setLength(root, len);
    }

    return error;
}