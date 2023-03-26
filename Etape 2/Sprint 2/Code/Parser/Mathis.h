#ifndef PROJET_RESEAU_MATHIS_H
#define PROJET_RESEAU_MATHIS_H

#include "../Node/Node.h"

int detect_Transfer_Encoding_header(Node *, const char *);

int detect_Transfer_Encoding(Node *, const char *);

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

int detect_DQUOTE(Node *, const char *);

int detect_Referer_header(Node *, const char *);

int detect_Referer(Node *, const char *);

int detect_absolute_URI(Node *, const char *);

int detect_scheme(Node *, const char *);

int detect_hier_part(Node *, const char *);

int detect_autority(Node *, const char *);

int detect_path_abempty(Node *, const char *);

int detect_path_absolute(Node *, const char *);

int detect_path_rootless(Node *, const char *);

//int detect_path_empty(Node *, const char *);

int detect_partial_URI(Node *, const char *);

int detect_userinfo(Node *, const char *);

int detect_segment_nz(Node *, const char *);

int detect_path_noscheme(Node *, const char *);

int detect_relative_part(Node *, const char *);

int detect_segment_nz_nc(Node *, const char *);

int detect_Accept_Charset_Header(Node *, const char *);

int detect_Accept_Charset(Node *, const char *);

int detect_charset(Node *, const char *);

#endif
