#ifndef PROJET_RESEAU_CHECKERS_H
#define PROJET_RESEAU_CHECKERS_H

int check_Accept_Encoding_Header(int);

int check_Accept_Header(int, char *);

int check_Connection_Header();

int check_headers(int, char *);

int check_Host_Header(int);

int check_method(int);

int check_path(int, char *);

int check_Range_Header(int, char *);

int check_request(int);

int check_Transfer_Encoding(int);

int check_version();

#endif
