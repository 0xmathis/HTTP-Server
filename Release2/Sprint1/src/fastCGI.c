#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/senders.h"
#include "../include/request.h"
#include "../include/utils.h"
#include "../include/fastCGI.h"
#include "../include/getters.h"
#include "../include/api.h"
#include "../include/others.h"


// =========================================================================================================== // 

size_t readSocket(int fd, char *buf, size_t len) {
    size_t readlen = 0;
    ssize_t nb = 0;

    if (len == 0) return 0;

    do {
        // try to read
        do {
            nb = read(fd, buf + readlen, len - readlen);
        } while (nb == -1 && errno == EINTR);
        if (nb > 0) readlen += nb;
    } while ((nb > 0) && (len != readlen));

    if (nb < 0) readlen = -1;
    return readlen;
}
//============================================================================================================ // 

void readData(int fd, FCGI_Header *h, size_t *len) {
    size_t nb;
    *len = 0;

    nb = sizeof(FCGI_Header) - FASTCGILENGTH;
    if ((readSocket(fd, (char *) h, nb) == nb)) {
        h->requestId = htons(h->requestId);
        h->contentLength = htons(h->contentLength);
        *len += nb;
        nb = h->contentLength + h->paddingLength;
        if ((readSocket(fd, (char *) h->contentData, nb) == nb)) {
            *len += nb;
        } else {
            *len = 0;
        }
    }
}

// =========================================================================================================== // 
void writeSocket(int fd, FCGI_Header *h, unsigned int len) {
    int w;

    h->contentLength = htons(h->contentLength);
    h->paddingLength = htons(h->paddingLength);


    while (len) {
        // try to write
        do {
            w = write(fd, h, len);
        } while (w == -1 && errno == EINTR);
        len -= w;
    }
}

// =========================================================================================================== // 
void writeLen(int len, char **p) {
    if (len > 0x7F) {
        *((*p)++) = (len >> 24) & 0x7F;
        *((*p)++) = (len >> 16) & 0xFF;
        *((*p)++) = (len >> 8) & 0xFF;
        *((*p)++) = (len) & 0xFF;
    } else *((*p)++) = (len) & 0x7F;
}

// =========================================================================================================== // 
int addNameValuePair(FCGI_Header *h, char *name, char *value) {
    char *p;
    unsigned int nameLen = 0, valueLen = 0;

    if (name) nameLen = strlen(name);
    if (value) valueLen = strlen(value);

    if (valueLen > FASTCGIMAXNVPAIR) return -1;
    if ((h->contentLength + ((nameLen > 0x7F) ? 4 : 1) + ((valueLen > 0x7F) ? 4 : 1)) > FASTCGILENGTH) return -1;

    p = (h->contentData) + h->contentLength;
    writeLen(nameLen, &p);
    writeLen(valueLen, &p);
    strncpy(p, name, nameLen);
    p += nameLen;
    if (value) strncpy(p, value, valueLen);
    h->contentLength += nameLen + ((nameLen > 0x7F) ? 4 : 1);
    h->contentLength += valueLen + ((valueLen > 0x7F) ? 4 : 1);

    return 0;
}
// =========================================================================================================== // 		

void sendGetValue(int fd) {
    FCGI_Header h;

    h.version = FCGI_VERSION_1;
    h.type = FCGI_GET_VALUES;
    h.requestId = htons(FCGI_NULL_REQUEST_ID);
    h.contentLength = 0;
    h.paddingLength = 0;
    addNameValuePair(&h, FCGI_MAX_CONNS, NULL);
    addNameValuePair(&h, FCGI_MAX_REQS, NULL);
    addNameValuePair(&h, FCGI_MPXS_CONNS, NULL);
    writeSocket(fd, &h, FCGI_HEADER_SIZE + (h.contentLength) + (h.paddingLength));
}

// =========================================================================================================== // 
void sendBeginRequest(int fd, unsigned short requestId, unsigned short role, unsigned char flags) {
    FCGI_Header h;
    FCGI_BeginRequestBody *begin;

    h.version = FCGI_VERSION_1;
    h.type = FCGI_BEGIN_REQUEST;
    h.requestId = htons(requestId);
    h.contentLength = sizeof(FCGI_BeginRequestBody);
    h.paddingLength = 0;
    begin = (FCGI_BeginRequestBody *) &(h.contentData);
    begin->role = htons(role);
    begin->flags = flags;
    writeSocket(fd, &h, FCGI_HEADER_SIZE + (h.contentLength) + (h.paddingLength));
}

// =========================================================================================================== //
void sendAbortRequest(int fd, unsigned short requestId) {
    FCGI_Header h;

    h.version = FCGI_VERSION_1;
    h.type = htons(FCGI_ABORT_REQUEST);
    h.requestId = requestId;
    h.contentLength = 0;
    h.paddingLength = 0;
    writeSocket(fd, &h, FCGI_HEADER_SIZE + (h.contentLength) + (h.paddingLength));
}

#define sendStdin(fd, id, stdin, len) sendWebData(fd,FCGI_STDIN,id,stdin,len)
#define sendData(fd, id, data, len) sendWebData(fd,FCGI_DATA,id,data,len)

//============================================================================================================ // 

void sendWebData(int fd, unsigned char type, unsigned short requestId, char *data, unsigned int len) {
    FCGI_Header h;
    if (len > FASTCGILENGTH) return;

    h.version = FCGI_VERSION_1;
    h.type = type;
    h.requestId = htons(requestId);
    h.contentLength = len;
    h.paddingLength = 0;
    memcpy(h.contentData, data, len);
    writeSocket(fd, &h, FCGI_HEADER_SIZE + (h.contentLength) + (h.paddingLength));
}

// =========================================================================================================== // 
int createSocket(int port) {
    int fd;
    struct sockaddr_in serv_addr;
//    int enable = 1;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed\n");
        return (-1);
    }

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(port);

    if (connect(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed\n");
        return (-1);
    }

    return fd;
}

// =========================================================================================================== //
int get_PHP_Data(int clientId) {
    int fd;
    size_t len;

// TEST ///
    FCGI_Header h;
    fd = createSocket(9000);
    sendBeginRequest(fd, 10, FCGI_RESPONDER, FCGI_KEEP_CONN);
    h.version = FCGI_VERSION_1;
    h.type = FCGI_PARAMS;
    h.requestId = htons(10);
    h.contentLength = 0;
    h.paddingLength = 0;
    //addNameValuePair(&h,"SCRIPT_NAME","/info.php");
    addNameValuePair(&h, "REQUEST_METHOD", "GET");
    addNameValuePair(&h, "SCRIPT_FILENAME", "/var/www/html/info.php");
//    addNameValuePair(&h, "SCRIPT_FILENAME", "/home/mathis/Documents/Projet-Reseau/Release2/Sprint1/sites/site1.fr/info.php");
    writeSocket(fd, &h, FCGI_HEADER_SIZE + (h.contentLength) + (h.paddingLength));
    h.contentLength = 0;
    h.paddingLength = 0;
    writeSocket(fd, &h, FCGI_HEADER_SIZE + (h.contentLength) + (h.paddingLength));


    readData(fd, &h, &len);
    send_Transfer_Encoding_Header(clientId, "chunked");
    writeDirectClient(clientId, h.contentData, h.contentLength);

    do {
        char chunkSize[16];
        readData(fd, &h, &len);
        sprintf(chunkSize, "%X\r\n", h.contentLength);

        writeDirectClient(clientId, chunkSize, strlen(chunkSize));
        writeDirectClient(clientId, h.contentData, h.contentLength);
        writeDirectClient(clientId, "\r\n", 2);
    } while ((len != 0) && (h.type != FCGI_END_REQUEST));

    writeDirectClient(clientId, "0\r\n\r\n", 5);
    close(fd);

    return 1;
}

void send_PHP_request(int *fd, FCGI_Header *header, char *path) {
    *fd = createSocket(9000);
    sendBeginRequest(*fd, 10, FCGI_RESPONDER, FCGI_KEEP_CONN);
    header->version = FCGI_VERSION_1;
    header->type = FCGI_PARAMS;
    header->requestId = htons(10);
    header->contentLength = 0;
    header->paddingLength = 0;
//    addNameValuePair(header,"SCRIPT_NAME","/info.php");

    if (isGet()) {
        addNameValuePair(header, "REQUEST_METHOD", "GET");
        addNameValuePair(header, "SCRIPT_FILENAME", path);
        printf("Query param = %s\n",getHeaderValue(root,"query"));
        addNameValuePair(header, "QUERY_STRING", getHeaderValue(root,"query"));
    } else if (isPost()) {
        addNameValuePair(header, "REQUEST_METHOD", "POST");
        addNameValuePair(header, "SCRIPT_FILENAME", path);
        printf("Query param = %s\n",getHeaderValue(root,"message_body"));
        addNameValuePair(header, "QUERY_STRING", getHeaderValue(root,"message_body"));
    }
    writeSocket(*fd, header, FCGI_HEADER_SIZE + (header->contentLength) + (header->paddingLength));
    header->contentLength = 0;
    header->paddingLength = 0;
    writeSocket(*fd, header, FCGI_HEADER_SIZE + (header->contentLength) + (header->paddingLength));
}

void receive_PHP_answer(int clientId, int fd, FCGI_Header header) {
    size_t len;
    int i = 0;

    readData(fd, &header, &len);
    send_Transfer_Encoding_Header(clientId, "chunked");

    while(!startWith("\r\n\r\n",&header.contentData[i])){
        i += 1;
    }

    i += 4;

    writeDirectClient(clientId, header.contentData, i);
    printf("First answer :\n\"\"\"\n%.*s\n\"\"\"\n", i, header.contentData);
    printf("i = %d, Length = %d\n",i, header.contentLength );

    if (isHead()) {
        close(fd);
        return;
    }

    if(header.contentLength != i){
        char chunkSize[16];

        sprintf(chunkSize, "%X\r\n", header.contentLength-i);

        writeDirectClient(clientId, chunkSize, strlen(chunkSize));
        writeDirectClient(clientId, &header.contentData[i], header.contentLength-i);
        writeDirectClient(clientId, "\r\n", 2);
        printf("Second answer :\n\"\"\"\n%.*s\n\"\"\"\n", header.contentLength-i, &header.contentData[i]);
    }

    do {
        char chunkSize[16];
        readData(fd, &header, &len);
        sprintf(chunkSize, "%X\r\n", header.contentLength);

        writeDirectClient(clientId, chunkSize, strlen(chunkSize));
        writeDirectClient(clientId, header.contentData, header.contentLength);
        writeDirectClient(clientId, "\r\n", 2);

    } while ((len != 0) && (header.type != FCGI_END_REQUEST));

    writeDirectClient(clientId, "0\r\n\r\n", 5);
    close(fd);
}