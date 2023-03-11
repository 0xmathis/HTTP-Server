#ifndef PROJET_RESEAU_MATHIS_H
#define PROJET_RESEAU_MATHIS_H

#include "../../Node/Node.h"

int detect_Transfer_Encoding_header(Node *, const char *);

int detect_Transfert_Encoding(Node *, const char *);

int detect_Expect_header(Node *, const char *);

int detect_Expect(Node *, const char *);

int detect_Host_header(Node *, const char *);

int detect_Host(Node *, const char *);

int detect_uri_host(Node *, const char *);

int detect_host(Node *, const char *);

int detect_IP_literal(Node *, const char *);

int detect_IPv4address(Node *, const char *);

int detect_IPv6address(Node *, const char *);

int detect_IPvFuture(Node *, const char *);

int detect_reg_name(Node *, const char *);

int detect_port(Node *, const char *);

int detect_User_Agent_header(Node *, const char *);

int detect_User_Agent(Node *, const char *);

int detect_Accept_Language_header(Node *, const char *);

int detect_Accept_Language(Node *, const char *);

int detect_Accept_header(Node *, const char *);

int detect_Accept(Node *, const char *);

int detect_media_range(Node *, const char *);

int detect_accept_params(Node *, const char *);

int detect_accept_ext(Node *, const char *);

int detect_Accept_Encoding_header(Node *, const char *);

int detect_Accept_Encoding(Node *, const char *);

int detect_message_body(Node *, const char *);

int detect_OCTET(Node *, const char *);

#endif
