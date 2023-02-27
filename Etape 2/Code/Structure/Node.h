#ifndef NODE_H
#define NODE_H

typedef struct Node {
    char *label;
    char *start;
    int length;
    Node *child;
    Node *brother;
} Node;

// Constructeurs

Node *newChild(Node *);

Node *newBrother(Node *);

// Destructeurs

void delChild(Node *);

void delBrother(Node *);

// Setters

void setLabel(Node *, char *)

void setStart(Node *, char *);

void setLength(Node *, int);

// Getters

char *getLabel(Node *);

char *getStart(Node *);

int getLength(Node *);

// Autre

void printChilds(Node *);

#endif