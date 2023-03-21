#ifndef PROJET_RESEAU_MATHIS_H
#define PROJET_RESEAU_MATHIS_H

#include "../Node/Node.h"

int detect_HTTP_message(Node *, const char *);

int detect_ALPHA(Node *, const char *);

int detect_alphanum(Node *, const char *);

int detect_DIGIT(Node *, const char *);

int detect_comment(Node *, const char *);

int detect_ctext(Node *, const char *);

int detect_quoted_pair(Node *, const char *);

int detect_connection_option(Node *, const char *);

int detect_token(Node *, const char *);

int detect_codings(Node *, const char *);

int detect_content_coding(Node *, const char *);

int detect_cookie_octet(Node *, const char *);

int detect_cookie_pair(Node *, const char *);

int detect_cookie_name(Node *, const char *);

int detect_cookie_value(Node *, const char *);

int detect_CRLF(Node *, const char *);

int detect_HTAB(Node *, const char *);

int detect_SP(Node *, const char *);

int detect_obs_text(Node *, const char *);

int detect_dec_octet(Node *, const char *);

int detect_field_vchar(Node *, const char *);

int detect_VCHAR(Node *, const char *);

int detect_h16(Node *, const char *);

int detect_HEXDIG(Node *, const char*);

#endif