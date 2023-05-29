#ifndef PROJET_RESEAU_NATHAN_H
#define PROJET_RESEAU_NATHAN_H

#include "../include/api.h"

int check_Accept_Encoding_Header(int);

int check_accept_language(int);

char *getEncoding();

void send_Content_Encoding_Header(int);

char *percentEncodings(char *);

char *sanitizePath(char *);

int check_path(int, char *);

void send_Transfer_Encoding_Header(int, char *);

void getFileExtension(char *, char *);

char *getFilePath();

int isStreamable(char *);

#endif