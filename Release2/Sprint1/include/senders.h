#ifndef PROJET_RESEAU_SENDERS_H
#define PROJET_RESEAU_SENDERS_H

void send_Accept_Ranges_Header(int);

void send_Connection_Header(int);

void send_Content_Encoding_Header(int);

void send_Content_Length_Header(int, int);

void send_Content_Range_Header(int, int, int, int);

void send_Content_Type_Header(int, char *);

void send_Date_Header(int);

void send_error_code(int, int, char *);

void send_headers(int, char *);

void send_message_body(int, char *);

void send_message_body_chunked(int, char *);

void send_message_body_php(int, char *);

void send_message_body_streaming(int, char *);

void send_Server_Header(int);

void send_status_line(int, int, char *);

void send_Transfer_Encoding_Header(int, char *);

#endif