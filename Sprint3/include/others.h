#ifndef PROJET_RESEAU_OTHERS_H
#define PROJET_RESEAU_OTHERS_H

int isGet();

int isHead();

int isPost();

int isPHP(char *);

int isStreamable(char *);

char *percentEncodings(char *);

char *getPWD();

void remove_dot_segments(char *);

char *sanitizePath(char *);

#endif
