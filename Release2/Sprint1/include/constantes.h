#ifndef CONSTANTES_H
#define CONSTANTES_H


#define TEMPLATE_STATUS_LINE "%s %.3d %s\r\n"
#define TEMPLATE_ERROR       "<html>\n<head><title>%.3d %s</title></head>\n<body>\n<center><h1>%.3d %s</h1></center>\n</body>\n</html>\n"


#define PATH_DEFAULT "site1.fr:8080"


#define MAX_SIZE_WITHOUT_CHUNK  1000  // 1Ko
#define MAX_SIZE_STREAMING      100000  // 100Ko

//char ACCEPTED_ENCODINGS[5][10] = {"*", "deflate", "br", "identity"};
//int NB_ACCEPTED_ENCODINGS = 4;

#endif
