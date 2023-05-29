#ifndef PROJET_RESEAU_JOSIAS_H
#define PROJET_RESEAU_JOSIAS_H

int check_Accept_Header(int, char *);

int check_Host_Header(int);

int check_headers(int, char *);

int check_method(int);

void send_Content_Type_Header(int, char *);

void send_Connection_Header(int);

void send_Content_Length_Header(int, int);

char *getHostTarget();

char *getMIMEtype(char *);

int isGet();

#endif