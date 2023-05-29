#ifndef PROJET_RESEAU_HUGO_H
#define PROJET_RESEAU_HUGO_H

int check_version();

int check_Connection_Header();

int check_Transfer_Encoding(int);

void remove_dot_segments(char *);

int check_request(int);

void send_status_line(int, int, char *);

void send_headers(int, char *);

void send_Server_Header(int);

void send_Date_Header(int);

unsigned char *getFileData(char *, int *);

#endif