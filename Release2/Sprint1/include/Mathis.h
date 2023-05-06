#ifndef PROJET_RESEAU_MATHIS_H
#define PROJET_RESEAU_MATHIS_H

#include "../include/Node.h"

int check_request(Node *, int);

int check_method(Node *, int);

int check_path(Node *, int);

int isGet(Node *);

char *getHeaderValue(Node *, char *);

char *getFilePath(Node *);

char *getFileExtension(char *);

unsigned char *getDataFromFile(char *, int *);

char *getDirectoryRepresentationHTML();  // renvoie la liste des fichiers présents dans le dossier demandé cf : challenge01.root-me.org

char *detect_MIME_type(Node *);

void sendErrorCode(Node *, int, int, char *);

void send_Content_Type_Header(int, char *);

void send_Date_Header(int);

void send_Server_Header(int);

void send_headers(int, char *);

void send_message_body(int, char *);


#endif
