#include <ctype.h>
#include <string.h>
#include "../Node/Node.h"
#include "parseur.h"


// TODO : finir la fonction qui récupère la chaine "start" au debut
// IDEE : faire une grosse fonction qui appelle d'abord detectStart puis parser


int detectStart(char *i) {
    int rv = 0;
    if (*i == 's' && *(i + 1) == 't' && *(i + 2) == 'a' && *(i + 3) == 'r' && *(i + 4) == 't') {
        rv = 1;
    }
    return (rv);
}

int detectFin(char *i) {
    int rv = 0;
    if (*i == 'f' && *(i + 1) == 'i' && *(i + 2) == 'n') {
        rv = 1;
    }
    return (rv);
}

int parser(Node *parent_node, Node *current_node, char *i) {
    if (detectFin(i)) {
        Node *n = newChild(parent_node);
        initNode(n, "fin", i, 3);
        i += 2;
        current_node = n;
    } else if (*i == '\n') {
        if (strcmp(getLabel(current_node), "fin") == 0) {
            Node *n = newChild(parent_node);
            initNode(n, "__lf", i, 1);
            return 0;
        } else {
            return (-1);
        }
    } else if (isdigit(*i)) { // Si le caractère est un chiffre
        // Si on ne se trouve pas déjà dans un nombre
        if (current_node == NULL) {
            Node *n = newChild(parent_node);
            initNode(n, "nombre", i, 1);
            current_node = n;
        } else {  // Si on se trouve dans un Node
            //Si on était dans un mot
            if (!(strcmp(getLabel(current_node), "mot"))) {
                return (-1);
            } else {
                setLength(current_node, getLength(current_node) + 1);
            }
        }
    } else if (strchr(",.:!?", *i) != NULL) {  // Si le caractère est une ponctuation
        Node *n = newChild(parent_node);
        initNode(n, "ponct", i, 1);
        current_node = NULL;
    } else if (strchr("\t -_", *i) != NULL) {  // Si le caractère est un séparateur
        // Si le séparateur termine un mot
        if (!(strcmp(getLabel(current_node), "mot"))) {
            int k = getLength(current_node);
            setLength(current_node, k + 1);
        } else {
            Node *n = newChild(parent_node);
            initNode(n, "separateur", i, 1);
        }
        current_node = NULL;
    } else if (isalpha(*i)) {  // Si le caractère est une lettre
        // Si on ne se trouve pas deja dans un mot
        if (current_node == NULL) {
            Node *n = newChild(parent_node);
            initNode(n, "mot", i, 1);
            current_node = n;
        } else {  // Si on se trouve dans un Node
            //Si on était dans un mot
            if (!(strcmp(getLabel(current_node), "mot"))) {
                int k = getLength(current_node);
                setLength(current_node, k + 1);
            } else {
                return (-1);
            }
        }
    }

    return parser(parent_node, current_node, i + 1);
}

int getStringLength(char *string) {
    int length = 0;

    while (*(string + length) != '\0') {
        length++;
    }

    return length;
}