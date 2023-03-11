#include "Node.h"
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
    
    return;
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
    for (int i = 0; i < depth; i++) { printf("\t"); }
    printf("%s: ", this->__label);
    for (int i = 0; i < getLength(this); i++) { printf("%c", *(getStart(this) + i)); }
    printf("\n");

    if (getChild(this) != NULL) {
        printChildren(getChild(this), depth + 1);
    }

    if (getBrother(this) != NULL) {
        printChildren(getBrother(this), depth);
    }
}