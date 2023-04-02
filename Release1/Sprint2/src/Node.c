#include "../include/Node.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Constructeurs

Node *newChild(Node *parent) {
    Node *child = newNode();

    Node *temp = getChild(parent);
    if (temp == NULL) {
        setChild(parent, child);
    } else {
        while (getBrother(temp) != NULL) {
            temp = getBrother(temp);
        }
        setBrother(temp, child);
    }

    return child;
}

Node *newBrother(Node *bigBrother) {
    Node *brother = newNode();
    Node *temp = getBrother(bigBrother);

    while (temp != NULL) {
        temp = getBrother(temp);
    }

    setBrother(temp, brother);

    return brother;
}

Node *newNode() {
    return (Node *) malloc(sizeof(Node));
}

void initNode(Node *this, const char *label, const char *start, int length) {
    setLabel(this, label);
    setStart(this, start);
    setLength(this, length);
    setChild(this, NULL);
    setBrother(this, NULL);
}


// Destructeurs

void delTree(Node *this) {
    if (getBrother(this) != NULL) {
        delTree(getBrother(this));
        setBrother(this, NULL);
    }

    if (getChild(this) != NULL) {
        delTree(getChild(this));
        setChild(this, NULL);
    }

    free(this);
}

void delNode(Node *this, Node *parent) {
    Node *child = getChild(parent);

    if (child == this) {
        setChild(parent, getBrother(this));
    } else {
        while (child != NULL && getBrother(child) != this) {
            child = getBrother(child);
        }

        if (child == NULL) {
            return;
        }

        setBrother(child, getBrother(this));
    }

    free(this);
}


// Setters

void setLabel(Node *this, const char *label) {
    strcpy(this->__label, label);
}

void setStart(Node *this, const char *start) {
    this->__start = start;
}

void setLength(Node *this, int length) {
    this->__length = length;
}

void setChild(Node *this, Node *child) {
    this->__child = child;
}

void setBrother(Node *this, Node *brother) {
    this->__brother = brother;
}


// Getters

const char *getLabel(Node *this) {
    return this->__label;
}

const char *getStart(Node *this) {
    return this->__start;
}

int getLength(Node *this) {
    if (this == NULL) {
        return 0;
    }

    return this->__length;
}

Node *getChild(Node *this) {
    return this->__child;
}

Node *getBrother(Node *this) {
    return this->__brother;
}

Node *getLastChild(Node *this) {
    Node *child = getChild(this);
    if (child == NULL) {
        return NULL;
    }

    while (getBrother(child) != NULL) {
        child = getBrother(child);
    }

    return child;
}

int getSumLengthChildren(Node *this) {
    int sum = 0;
    Node *child = getChild(this);

    while (child != NULL) {
        sum += getLength(child);
        child = getBrother(child);
    }

    return sum;
}

// Autre

void printChildren(Node *this, int depth) {
    for (int i = 0; i < 4*depth; i++) { printf(" "); }

    printf("[%d:%s] = \"", depth, getLabel(this));

    if (strcmp(getLabel(this), "__crlf") == 0) {
        printf("__");
    } else if (getLength(this) > 9) {
        for (int i = 0; i < 3; i++) {
            if (*(getStart(this) + i) == '\r' || *(getStart(this) + i) == '\n') {
                printf("_");
            } else {
                printf("%c", *(getStart(this) + i));
            }
        }

        printf("..");

        for (int i = getLength(this) - 3; i < getLength(this); i++) {
            if (*(getStart(this) + i) == '\r' || *(getStart(this) + i) == '\n') {
                printf("_");
            } else {
                printf("%c", *(getStart(this) + i));
            }
        }
    } else {
        for (int i = 0; i < getLength(this); i++) {
            if (*(getStart(this) + i) == '\r' || *(getStart(this) + i) == '\n') {
                printf("_");
            } else {
                printf("%c", *(getStart(this) + i));
            }
        }
    }
    printf("\"\n");

    if (getChild(this) != NULL) {
        printChildren(getChild(this), depth + 1);
    }

    if (getBrother(this) != NULL) {
        printChildren(getBrother(this), depth);
    }
}