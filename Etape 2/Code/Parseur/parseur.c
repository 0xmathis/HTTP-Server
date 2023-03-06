#include <ctype.h>
#include <string.h>
#include "../Node/Node.h"
#include "parseur.h"


int detectStart(char *i) {
    int rv = 0;
    if (toLower(i[0]) == 's' && toLower(i[1]) == 't' && toLower(i[2]) == 'a' && toLower(i[3]) == 'r' && toLower(toLower(i[4])) == 't') {
        rv = 1;
    }
    return (rv);
}

int detectFin(char *i) {
    int rv = 0;
    if (toLower(i[0]) == 'f' && toLower(i[1]) == 'i' && toLower(i[2]) == 'n' && toLower(i[3]) == '\n') {
        rv = 1;
    }
    return (rv);
}

int startParser(Node *parent_node, char *i) {
    if (detectStart(i)) {
        Node *current_node = newChild(parent_node);
        initNode(current_node, "debut", i, 5);
        initNode(newChild(current_node), "__istring", i, 5);
        return parser(parent_node, current_node, i + 5);
    } else {  // si on a pas de start
        return (-1);
    }
}

int parser(Node *parent_node, Node *current_node, char *i) {
    int comptage = 0;
    while (*i != '\0') {
        if (detectFin(i)) {
            Node *n = newChild(parent_node);
            initNode(n, "fin", i, 3);
            initNode(newChild(n), "__istring", i, 3);
            i += 3;

            n = newChild(parent_node);
            initNode(n, "__lf", i, 1);
        } else if (isdigit(*i)) {  // Si le caractère est un chiffre
            if (!(strcmp(getLabel(current_node), "nombre"))) {  // Si on était déjà dans un nombre
                setLength(current_node, getLength(current_node) + 1);
                Node *m = newChild(current_node);
                initNode(m, "__digit", i, 1);
            }
            else if (!(strcmp(getLabel(current_node), "mot"))) {  // Si on était dans un mot
                return (-1);
            } else {
                Node *n = newChild(parent_node);
                initNode(n, "nombre", i, 1);
                current_node = n;
                Node *m = newChild(n);
                initNode(m, "__digit", i, 1);
            }
        } else if (strchr(",.:!?", *i) != NULL) {  // Si le caractère est une ponctuation
            if (!(strcmp(getLabel(current_node), "mot"))) {  // si on était dans un mot
                if (strchr("\t -_", *getStart(getLastChild(current_node))) != NULL) {  // si le mot est déjà fini
                    Node *n = newChild(parent_node);
                    initNode(n, "ponct", i, 1);
                    current_node = n;
                    Node *m = newChild(n);
                    initNode(m, "__icar", i, 1);
                    comptage += 1;
                } else {
                    return (-1);
                }
            } else if (strchr(",.:!?", *getStart(current_node)) != NULL) {  // si on avait déjà une ponctuation
                if (detectFin(i + 1)) {  // si après on a la fin
                    Node *n = newChild(parent_node);
                    initNode(n, "ponct", i, 1);
                    initNode(newChild(n), "__icar", i, 1);
                    current_node = n;
                    i += 1;

                    n = newChild(parent_node);
                    initNode(n, "fin", i, 3);
                    initNode(newChild(n), "__istring", i, 3);
                    i += 3;

                    n = newChild(parent_node);
                    initNode(n, "__lf", i, 1);
                } else {
                    return (-1);
                }
            } else if (strchr("\t -_", *getStart(getLastChild(current_node))) != NULL) {  // si on avait un separateur
                Node *n = newChild(parent_node);
                initNode(n, "ponct", i, 1);
                current_node = n;
                Node *m = newChild(n);
                initNode(m, "__icar", i, 1);
            } else {
                return (-1);
            }
        } else if (strchr("\t -_", *i) != NULL) {  // Si le caractère est un séparateur
            if (!(strcmp(getLabel(current_node), "mot"))) {  // Si on était dans un mot
                if (strchr("\t -_", *getStart(getLastChild(current_node))) != NULL) {  // Si on a déjà eu un séparateur pour terminer le mot
                    return (-1);
                } else {
                    setLength(current_node, getLength(current_node) + 1);
                    Node *n = newChild(current_node);
                    initNode(n, "separateur", i, 1);
                    Node *m = newChild(n);

                    if (*i == ' ') {
                        initNode(m, "__sp", i, 1);
                    } else if (*i == '\t') {
                        initNode(m, "__htab", i, 1);
                    } else {
                        initNode(m, "__icar", i, 1);
                    }
                }
            } else if (!(strcmp(getLabel(current_node), "nombre"))) {  // si on était dans un nombre
                comptage += 1;

                Node *n = newChild(parent_node);
                initNode(n, "separateur", i, 1);
                current_node = n;
                Node *m = newChild(n);

                if (*i == ' ') {
                    initNode(m, "__sp", i, 1);
                } else if (*i == '\t') {
                    initNode(m, "__htab", i, 1);
                } else {
                    initNode(m, "__icar", i, 1);
                }
            } else {
                return (-1);
            }
        } else if (isalpha(*i)) {  // Si le caractère est une lettre
            if (!(strcmp(getLabel(current_node), "mot"))) {  // Si on était dans un mot
                if (strchr("\t -_", *getStart(getLastChild(current_node))) != NULL) {  // si le mot était déjà terminé
                    return (-1);
                } else {
                    setLength(current_node, getLength(current_node) + 1);
                    Node *m = newChild(current_node);
                    initNode(m, "__alpha", i, 1);
                }
            } else if (!(strcmp(getLabel(current_node), "nombre"))) {  // Si on était dans un nombre
                return (-1);
            } else {
                Node *n = newChild(parent_node);
                initNode(n, "mot", i, 1);
                current_node = n;
                Node *m = newChild(n);
                initNode(m, "__alpha", i, 1);
            }
        } else {
            return -1;
        }

        i += 1;
    }

    if (comptage < 2) {  // si on a pas au moins 2 couples mot-ponct/nombre-separateur
        return (-1);
    } else {
        return 0;
    }
}

int getStringLength(char *string) {
    int length = 0;

    while (*(string + length) != '\0') {
        length++;
    }

    return length;
}

char toLower(char letter) {
    if ('A' <= letter && letter <= 'Z') {
        return letter + 32;
    }

    return letter;
}