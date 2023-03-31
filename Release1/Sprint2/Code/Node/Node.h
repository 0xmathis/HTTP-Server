#ifndef NODE_H
#define NODE_H


typedef struct Node Node;

struct Node {
    char __label[50];
    const char *__start;
    int __length;
    Node *__child;
    Node *__brother;
};

// Constructeurs

Node *newNode();

Node *newChild(Node *);

Node *newBrother(Node *);

void initNode(Node *, const char *, const char *, int);

// Destructeurs

void delTree(Node *);

void delNode(Node *, Node *);

// Setters

void setLabel(Node *, const char *);

void setStart(Node *, const char *);

void setLength(Node *, int);

void setChild(Node *, Node *);

void setBrother(Node *, Node *);

// Getters

const char *getLabel(Node *);

const char *getStart(Node *);

int getLength(Node *);

Node *getChild(Node *);

Node *getBrother(Node *);

Node *getLastChild(Node *);

int getSumLengthChildren(Node *);

// Autre

void printChildren(Node *, int);

#endif