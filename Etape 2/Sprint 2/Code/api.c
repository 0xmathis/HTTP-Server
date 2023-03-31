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

void searchTreeRecursif(void *start, char *name, _Token *liste) {
    start = (Node *) start;

    if (strcmp(getLabel(start), name) == 0) {
        insertion(liste, start);
    }

    if (getChild(start)) {
        searchTreeRecursif(getChild(start), name, liste);
    }

    if (getBrother(start)) {
        searchTreeRecursif(getBrother(start), name, liste);
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
    node = (Node *) node;

    if (len) {
        *len = getLength(node);
    }

    return (char *) getLabel(node);
}

char *getElementValue(void *node, int *len) {
    node = (Node *) node;

    if (len) {
        *len = getLength(node);
    }

    return (char *) getStart(node);
}

void purgeElement(_Token **root) {
    if (!*root) {
        return;
    }

    if ((*root)->next) {
        purgeElement(&((*root)->next));
    }

    free(*root);
    *root = NULL;
}

void purgeTree(void *root) {
    delTree((Node *) root);
}

int parseur(char *message, int length) {
    root = newNode();
    int error = detect_HTTP_message(root, message);

    if (getLength(root) > length) {
        setLength(root, length);
    }

    return error;
}