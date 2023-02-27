#ifndef NODE_H
#define NODE_H


typedef struct Node Node;

struct Node {
    char __label[50];
    char *__start;
    int __length;
    Node *__child;
    Node *__brother;
};

// Constructeurs

Node *newChild(Node *);

Node *newBrother(Node *);

// Destructeurs

void delChild(Node *);

void delBrother(Node *);

// Setters

void setLabel(Node *, char *);

void setStart(Node *, char *);

void setLength(Node *, int);

void setChild(Node *, Node *);

void setBrother(Node *, Node *);

// Getters

char *getLabel(Node *);

char *getStart(Node *);

int getLength(Node *);

Node *getChild(Node *);

Node *getBrother(Node *);

// Autre

void printChildren(Node *, int);

#endif