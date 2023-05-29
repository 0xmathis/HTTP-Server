#ifndef PROJET_RESEAU_MATHIS_H
#define PROJET_RESEAU_MATHIS_H

#include "../include/Node.h"


// Getters

char *getHeaderValue(Node *, char *);

Node *getRangeHeader();

void getRangeRange(int *, int *);


// Checkers

int check_Range_Header(int, char *);


// Senders

void send_message_body(int, char *);

void send_message_body_chunked(int, char *);

void send_message_body_streaming(int, char *);

void send_Accept_Ranges_Header(int);

void send_Content_Range_Header(int, int, int, int);

void sendErrorCode(int, int, char *);


// Testers


#endif
