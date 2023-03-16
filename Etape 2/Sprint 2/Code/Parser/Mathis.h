#ifndef PROJET_RESEAU_MATHIS_H
#define PROJET_RESEAU_MATHIS_H

#include "../Node/Node.h"

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

// A supprimer

int detect_OWS(Node *, const char *);
int detect_RWS(Node *, const char *);
int detect_dec_octet(Node *, const char *);
int detect_h16(Node *, const char *);
int detect_ls32(Node *, const char *);
int detect_HEXDIG(Node *, const char *);
int detect_unreserved(Node *, const char *);
int detect_sub_delims(Node *, const char *);
int detect_pct_encoded(Node *, const char *);
int detect_Accept_Encoding(Node *, const char *);
int detect_DIGIT(Node *, const char *);
int detect_product(Node *, const char *);
int detect_product(Node *, const char *);
int detect_language_range(Node *, const char *);
int detect_weight(Node *, const char *);
int detect_type(Node *, const char *);
int detect_subtype(Node *, const char *);
int detect_parameter(Node *, const char *);
int detect_token(Node *, const char *);
int detect_comment(Node *, const char *);
int detect_quoted_string(Node *, const char *);
int detect_codings(Node *, const char *);


#endif
