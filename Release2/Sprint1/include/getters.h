#ifndef PROJET_RESEAU_GETTERS_H
#define PROJET_RESEAU_GETTERS_H

#include "Node.h"

unsigned char *getFileData(char *, int *);

void getFileExtension(char *, char *);

char *getFilePath(int *);

int getFilePathLength();

char *getHeaderValue(Node *, char *);

char *getHostTarget();

char *getMIMEtype(char *);

Node *getRangeHeader();

void getRangeRange(int *, int *);

#endif