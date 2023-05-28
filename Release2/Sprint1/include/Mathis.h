#ifndef PROJET_RESEAU_MATHIS_H
#define PROJET_RESEAU_MATHIS_H

#include "../include/Node.h"

int check_request(Node *, int);

int check_method(Node *, int);

int check_Host_Header(Node *, int);

int check_Range_Header(Node *, int);

void getRangeRange(Node *, int *, int *);

int check_headers(Node *, int);

int isGet(Node *);

int isRange(Node *);

char *getHeaderValue(Node *, char *);

char *getHostTarget(Node *);

Node *getRangeHeader(Node *);

char *getFilePath(Node *);

void getFileExtension(char *, char *);

unsigned char *getFileData(char *, int *);

char *getDirectoryRepresentationHTML();  // renvoie la liste des fichiers présents dans le dossier demandé cf : challenge01.root-me.org

char *detect_MIME_type(char *);

void send_status_line(Node *, int, int, char *);

void sendErrorCode(Node *, int, int, char *);

void send_Content_Type_Header(int, char *);

void send_Date_Header(int);

void send_Server_Header(int);

void send_Connection_Header(int, Node *);

void send_Content_Length_Header(int, int);

void send_headers(int, char *, Node *);

void send_message_body(int, char *);

void send_message_body_streaming(int, char *);

int isVideoContent(char *);

#endif
