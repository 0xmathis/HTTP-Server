#include <ctype.h>
#include <string.h>
#include "../Node/Node.h"
#include "parseur.h"
#include <stdio.h>


int detectStart(char *i) {
    int rv = 0;
    if ((*i == 's' && *(i + 1) == 't' && *(i + 2) == 'a' && *(i + 3) == 'r' && *(i + 4) == 't') ||
        (*i == 'S' && *(i + 1) == 'T' && *(i + 2) == 'A' && *(i + 3) == 'R' && *(i + 4) == 'T')) {
        rv = 1;
    }
    return (rv);
}

int detectFin(char *i) {
    int rv = 0;
    if (((strchr(",.:!?", *i) != NULL) && (*(i + 1) == 'f' && *(i + 2) == 'i' && *(i + 3) == 'n')) || ((strchr(",.:!?", *i) != NULL) && (*i == 'F' && *(i + 1) == 'I' && *(i + 2) == 'N'))) {
        rv = 1;
    }
    return (rv);
}

int startParser(Node *parent_node, Node *current_node, char *i) {
    if (detectStart(i)) {
        parser(parent_node, current_node, i + 5);
    } else {
        // erreur pas de start
        return (-1);
    }

    return 0;
}

int parser(Node *parent_node, Node *current_node, char *i) {
    while (*i != '\0') {
        if (detectFin(i)) {

            Node *n = newChild(parent_node);
            initNode(n, "fin", i, 3);
            i += 3;
            current_node = n;

        } else if (*i == '\n') {

            if (strcmp(getLabel(current_node), "fin") == 0) {
                Node *n = newChild(parent_node);
                initNode(n, "__lf", i, 1);
                return 0;
            } else {
                return (-1);
            }

        } else if (isdigit(*i)) {

            // Si le caractère est un chiffre
            // Si on ne se trouve pas déjà dans un nombre

            if (current_node == NULL) {
                Node *n = newChild(parent_node);
                initNode(n, "nombre", i, 1);
                current_node = n;
            } else {

                // Si on se trouve dans un Node
                //Si on était dans un mot
                if (!(strcmp(getLabel(current_node), "mot"))) {
                    return (-1);
                } else {
                    setLength(current_node, getLength(current_node) + 1);
                }
            }
        } else if (strchr(",.:!?", *i) != NULL) {
            // Si le caractère est une ponctuation

            if (!(strcmp(getLabel(current_node), "nombre"))) {
                // Si on était dans un nombre

                return (-1);
            } else if (!(strcmp(getLabel(current_node), "mot"))) {
                if (strchr("\t -_", *getStart(getLastChild(current_node))) != NULL) {
                    Node *n = newChild(parent_node);
                    initNode(n, "ponct", i, 1);
                    current_node = NULL;
                } else {
                    return (-1);
                }
            } else {
                Node *n = newChild(parent_node);
                initNode(n, "ponct", i, 1);
                current_node = NULL;
            }

        } else if (strchr("\t -_", *i) != NULL) {
            // Si le caractère est un séparateur

            if (!(strcmp(getLabel(current_node), "mot"))) {
                // Si on était dans un mot

                if (strchr("\t -_", *getStart(getLastChild(current_node))) != NULL) {
                    // Si on a déjà eu un séparateur pour terminer le mot
                    return (-1);
                } else {
                    // Si le séparateur termine un mot

                    setLength(current_node, getLength(current_node) + 1);
                }

            } else {

                Node *n = newChild(parent_node);
                initNode(n, "separateur", i, 1);

            }
            current_node = NULL;
        } else if (isalpha(*i)) {

            // Si le caractère est une lettre
            // Si on ne se trouve pas deja dans un mot

            if (current_node == NULL) {

                Node *n = newChild(parent_node);
                initNode(n, "mot", i, 1);
                current_node = n;

            } else {

                // Si on se trouve dans un Node
                //Si on était dans un mot

                if (!(strcmp(getLabel(current_node), "mot"))) {
                    if (strchr("\t -_", *getStart(getLastChild(current_node))) != NULL) {
                        return (-1);
                    } else {
                        setLength(current_node, getLength(current_node) + 1);
                    }

                } else {
                    return (-1);
                }
            }
        }
        i += 1;
    }

    return 0;
}

int getStringLength(char *string) {
    int length = 0;

    while (*(string + length) != '\0') {
        length++;
    }

    return length;
}