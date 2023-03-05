#ifndef PARSEUR_H
#define PARSEUR_H

#include "../Node/Node.h"

int detectStart(char *);

int detectFin(char *);

int startParser(Node *, Node *, char *);

int parser(Node *, Node *, char *);

int getStringLength(char *);

#endif