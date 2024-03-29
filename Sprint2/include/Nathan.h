#ifndef PROJET_RESEAU_NATHAN_H
#define PROJET_RESEAU_NATHAN_H

#include "Node.h"

int detect_language_range(Node *, const char *);

int detect_ls32(Node *, const char *);

int detect_OWS(Node *, const char *);

int detect_BWS(Node *, const char *);

int detect_parameter(Node *, const char *);

int detect_quoted_string(Node *, const char *);

int detect_pchar(Node *, const char *);

int detect_unreserved(Node *, const char *);

int detect_pct_encoded(Node *, const char *);

int detect_sub_delims(Node *, const char *);

int detect_product(Node *, const char *);

int detect_product_version(Node *, const char *);

int detect_qdtext(Node *, const char *);

int detect_query(Node *, const char *);

int detect_qvalue(Node *, const char *);

int detect_RWS(Node *, const char *);

int detect_segment(Node *, const char *);

int detect_tchar(Node *, const char *);

int detect_transfer_coding(Node *, const char *);

int detect_transfer_extension(Node *, const char *);

int detect_transfer_parameter(Node *, const char *);

int detect_weight(Node *, const char *);

#endif