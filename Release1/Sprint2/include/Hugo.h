#ifndef PROJET_RESEAU_HUGO_H
#define PROJET_RESEAU_HUGO_H

#include "Node.h"

int detect_start_line(Node *, const char *);

int detect_request_line(Node *, const char *);

int detect_method(Node *, const char *);

int detect_request_target(Node *, const char *);

int detect_origin_form(Node *, const char *);

int detect_absolute_path(Node *, const char *);

int detect_HTTP_version(Node *, const char *);

int detect_HTTP_name(Node *, const char *);

int detect_header_field(Node *, const char *);

int detect_field_name(Node *, const char *);

int detect_field_value(Node *, const char *);

int detect_field_content(Node *, const char *);

int detect_obs_fold(Node *, const char *);

int detect_Connection_header(Node *, const char *);

int detect_Connection(Node *, const char *);

int detect_Content_Length_header(Node *, const char *);

int detect_Content_Length(Node *, const char *);

int detect_Content_Type_header(Node *, const char *);

int detect_Content_Type(Node *, const char *);

int detect_media_type(Node *, const char *);

int detect_type(Node *, const char *);

int detect_subtype(Node *, const char *);

int detect_Cookie_header(Node *, const char *);

int detect_cookie_string(Node *, const char *);

#endif