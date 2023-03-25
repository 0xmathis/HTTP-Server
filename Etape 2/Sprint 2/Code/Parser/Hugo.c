#include "Nathan.h"
#include "Josias.h"
#include "Mathis.h"
#include "Hugo.h"
#include "utils.h"
#include <stdio.h>

int detect_start_line(Node *parent, const char *ptr) {
    Node *startLineNode = newChild(parent);
    initNode(startLineNode, "start_line", ptr, 0);

    if (detect_request_line(startLineNode, ptr) == 0) {
        ptr += getLength(getLastChild(startLineNode));
    } else {
        delNode(startLineNode, parent);
        return 61;
    }

    setLength(startLineNode, getSumLengthChildren(startLineNode));

    return 0;
}

int detect_request_line(Node *parent, const char *ptr) {
    Node *requestLineNode = newChild(parent);
    initNode(requestLineNode, "request_line", ptr, 0);

    if (detect_method(requestLineNode, ptr) == 0) {
        ptr += getLength(getLastChild(requestLineNode));

        if (detect_SP(requestLineNode, ptr) == 0) {
            ptr += getLength(getLastChild(requestLineNode));

            if (detect_request_target(requestLineNode, ptr) == 0) {
                ptr += getLength(getLastChild(requestLineNode));

                if (detect_SP(requestLineNode, ptr) == 0) {
                    ptr += getLength(getLastChild(requestLineNode));

                    if (detect_HTTP_version(requestLineNode, ptr) == 0) {
                        ptr += getLength(getLastChild(requestLineNode));

                        if (detect_CRLF(requestLineNode, ptr) == 0) {
                            ptr += getLength(getLastChild(requestLineNode));
                        } else {
                            delNode(requestLineNode, parent);
                            return 62;
                        }
                    } else {
                        delNode(requestLineNode, parent);
                        return 62;
                    }
                } else {
                    delNode(requestLineNode, parent);
                    return 62;
                }
            } else {
                delNode(requestLineNode, parent);
                return 62;
            }
        } else {
            delNode(requestLineNode, parent);
            return 62;
        }
    } else {
        delNode(requestLineNode, parent);
        return 62;
    }

    setLength(requestLineNode, getSumLengthChildren(requestLineNode));

    return 0;
}

int detect_method(Node *parent, const char *ptr) {
    Node *methodNode = newChild(parent);
    initNode(methodNode, "method", ptr, 0);

    if (detect_token(methodNode, ptr) == 0) {
        ptr += getLength(getLastChild(methodNode));
    } else {
        delNode(methodNode, parent);
        return 63;
    }

    setLength(methodNode, getSumLengthChildren(methodNode));

    return 0;
}

int detect_request_target(Node *parent, const char *ptr) {
    Node *requestTargetNode = newChild(parent);
    initNode(requestTargetNode, "request_target", ptr, 0);

    if (detect_origin_form(requestTargetNode, ptr) == 0) {
        ptr += getLength(getLastChild(requestTargetNode));
    } else {
        delNode(requestTargetNode, parent);
        return 64;
    }

    setLength(requestTargetNode, getSumLengthChildren(requestTargetNode));

    return 0;
}

int detect_origin_form(Node *parent, const char *ptr) {
    Node *originFormNode = newChild(parent);
    initNode(originFormNode, "origin_form", ptr, 0);

    if (detect_absolute_path(originFormNode, ptr) == 0) {
        ptr += getLength(getLastChild(originFormNode));

        if (*ptr == '?') {
            initNode(newChild(originFormNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(originFormNode));

            if (detect_query(originFormNode, ptr) == 0) {
                ptr += getLength(getLastChild(originFormNode));
            } else {
                delNode(originFormNode, parent);
                return 65;
            }
        }
    } else {
        delNode(originFormNode, parent);
        return 65;
    }

    setLength(originFormNode, getSumLengthChildren(originFormNode));

    return 0;
}

int detect_absolute_path(Node *parent, const char *ptr) {
    Node *absolutePathNode = newChild(parent);
    initNode(absolutePathNode, "absolute_path", ptr, 0);
    int compteur = 0;

    while (1) {
        if (*ptr == '/') {
            initNode(newChild(absolutePathNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(absolutePathNode));
            compteur++;

            if (*ptr != '?' && detect_segment(absolutePathNode, ptr) == 0) {
                ptr += getLength(getLastChild(absolutePathNode));
            }
        } else {
            break;
        }
    }

    if (compteur == 0) {
        delNode(absolutePathNode, parent);
        return 66;
    }

    setLength(absolutePathNode, getSumLengthChildren(absolutePathNode));

    return 0;
}

int detect_HTTP_version(Node *parent, const char *ptr) {
    Node *HTTPVersionNode = newChild(parent);
    initNode(HTTPVersionNode, "HTTP_version", ptr, 0);

    if (detect_HTTP_name(HTTPVersionNode, ptr) == 0) {
        ptr += getLength(getLastChild(HTTPVersionNode));

        if (*ptr == '/') {
            initNode(newChild(HTTPVersionNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(HTTPVersionNode));

            if (detect_DIGIT(HTTPVersionNode, ptr) == 0) {
                initNode(newChild(HTTPVersionNode), "__digit", ptr, 1);
                ptr += getLength(getLastChild(HTTPVersionNode));

                if (*ptr == '.') {
                    initNode(newChild(HTTPVersionNode), "case_insensitive_string", ptr, 1);
                    ptr += getLength(getLastChild(HTTPVersionNode));

                    if (detect_DIGIT(HTTPVersionNode, ptr) == 0) {
                        initNode(newChild(HTTPVersionNode), "__digit", ptr, 1);
                        ptr += getLength(getLastChild(HTTPVersionNode));
                    } else {
                        delNode(HTTPVersionNode, parent);
                        return 67;
                    }
                } else {
                    delNode(HTTPVersionNode, parent);
                    return 67;
                }
            } else {
                delNode(HTTPVersionNode, parent);
                return 67;
            }
        } else {
            delNode(HTTPVersionNode, parent);
            return 67;
        }
    } else {
        delNode(HTTPVersionNode, parent);
        return 67;
    }

    setLength(HTTPVersionNode, getSumLengthChildren(HTTPVersionNode));

    return 0;
}

int detect_HTTP_name(Node *parent, const char *ptr) {
    Node *HTTPNameNode = newChild(parent);
    initNode(HTTPNameNode, "HTTP_name", ptr, 0);

    if (startWith("HTTP", ptr)) {
        initNode(newChild(HTTPNameNode), "__num", ptr, 4);
    } else {
        delNode(HTTPNameNode, parent);
        return 68;
    }

    setLength(HTTPNameNode, getSumLengthChildren(HTTPNameNode));

    return 0;
}

int detect_header_field(Node *parent, const char *ptr) {
    Node *headerFieldNode = newChild(parent);
    initNode(headerFieldNode, "header_field", ptr, 0);

    if (detect_Connection_header(headerFieldNode, ptr) == 0) {
        ptr += getLength(getLastChild(headerFieldNode));
    } else if (detect_Content_Length_header(headerFieldNode, ptr) == 0) {
        ptr += getLength(getLastChild(headerFieldNode));
    } else if (detect_Content_Type_header(headerFieldNode, ptr) == 0) {
        ptr += getLength(getLastChild(headerFieldNode));
    } else if (detect_Cookie_header(headerFieldNode, ptr) == 0) {
        ptr += getLength(getLastChild(headerFieldNode));
    } else if (detect_Expect_header(headerFieldNode, ptr) == 0) {
        ptr += getLength(getLastChild(headerFieldNode));
    } else if (detect_Host_header(headerFieldNode, ptr) == 0) {
        ptr += getLength(getLastChild(headerFieldNode));
    } else if (detect_User_Agent_header(headerFieldNode, ptr) == 0) {
        ptr += getLength(getLastChild(headerFieldNode));
    } else if (detect_Accept_Language_header(headerFieldNode, ptr) == 0) {
        ptr += getLength(getLastChild(headerFieldNode));
    } else if (detect_Accept_header(headerFieldNode, ptr) == 0) {
        ptr += getLength(getLastChild(headerFieldNode));
    } else if (detect_Accept_Encoding_header(headerFieldNode, ptr) == 0) {
        ptr += getLength(getLastChild(headerFieldNode));
    } else if (detect_field_name(headerFieldNode, ptr) == 0) {
        ptr += getLength(getLastChild(headerFieldNode));

        if (*ptr == ':') {
            initNode(newChild(headerFieldNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(headerFieldNode));

            if (detect_OWS(headerFieldNode, ptr) == 0) {
                ptr += getLength(getLastChild(headerFieldNode));
            }

            if (detect_field_value(headerFieldNode, ptr) == 0) {
                ptr += getLength(getLastChild(headerFieldNode));

                if (detect_OWS(headerFieldNode, ptr) == 0) {
                    ptr += getLength(getLastChild(headerFieldNode));
                }
            } else {
                delNode(headerFieldNode, parent);
                return 69;
            }
        } else {
            delNode(headerFieldNode, parent);
            return 69;
        }
    } else {
        delNode(headerFieldNode, parent);
        return 69;
    }

    setLength(headerFieldNode, getSumLengthChildren(headerFieldNode));


    return 0;
}

int detect_field_name(Node *parent, const char *ptr) {
    Node *fieldNameNode = newChild(parent);
    initNode(fieldNameNode, "field_name", ptr, 0);

    if (detect_token(fieldNameNode, ptr) == 0) {
        ptr += getLength(getLastChild(fieldNameNode));
    } else {
        delNode(fieldNameNode, parent);
        return 70;
    }

    setLength(fieldNameNode, getSumLengthChildren(fieldNameNode));

    return 0;
}

int detect_field_value(Node *parent, const char *ptr) {
    Node *fieldValueNode = newChild(parent);
    initNode(fieldValueNode, "field_value", ptr, 0);
    int compteur = 0;

    while (1) {
        if (detect_field_content(fieldValueNode, ptr) == 0) {
            ptr += getLength(getLastChild(fieldValueNode));
            compteur++;
        } else if (detect_obs_fold(fieldValueNode, ptr) == 0) {
            ptr += getLength(getLastChild(fieldValueNode));
            compteur++;
        } else {
            break;
        }
    }

    if (compteur == 0) {
        delNode(fieldValueNode, parent);
    }

    setLength(fieldValueNode, getSumLengthChildren(fieldValueNode));

    return 0;
}

int detect_field_content(Node *parent, const char *ptr) {
    Node *fieldContentNode = newChild(parent);
    initNode(fieldContentNode, "field_content", ptr, 0);

    if (detect_field_vchar(fieldContentNode, ptr) == 0) {
        ptr += getLength(getLastChild(fieldContentNode));
    } else {
        delNode(fieldContentNode, parent);
        return 72;
    }

    int compteur = 0;

    while (1) {
        if (detect_SP(fieldContentNode, ptr) == 0) {
            ptr += getLength(getLastChild(fieldContentNode));
            compteur++;
        } else if (detect_HTAB(fieldContentNode, ptr) == 0) {
            ptr += getLength(getLastChild(fieldContentNode));
            compteur++;
        } else {
            break;
        }
    }

    if (compteur >= 1) {
        if (detect_field_vchar(fieldContentNode, ptr) == 0) {
            ptr += getLength(getLastChild(fieldContentNode));
        } else {
            delNode(fieldContentNode, parent);
            return 72;
        }
    }


    setLength(fieldContentNode, getSumLengthChildren(fieldContentNode));

    return 0;
}

int detect_obs_fold(Node *parent, const char *ptr) {
    Node *obsFoldNode = newChild(parent);
    initNode(obsFoldNode, "obs_fold", ptr, 0);

    if (detect_CRLF(obsFoldNode, ptr) == 0) {
        ptr += getLength(getLastChild(obsFoldNode));

        int compteur = 0;
        while (1) {
            if (detect_SP(obsFoldNode, ptr) == 0 || detect_HTAB(obsFoldNode, ptr) == 0) {
                compteur++;
                ptr += getLength(getLastChild(obsFoldNode));
            } else {
                break;
            }
        }

        if (compteur == 0) {
            delNode(obsFoldNode, parent);
            return 73;
        }
    } else {
        delNode(obsFoldNode, parent);
        return 73;
    }

    setLength(obsFoldNode, getSumLengthChildren(obsFoldNode));

    return 0;
}

int detect_Connection_header(Node *parent, const char *ptr) {
    Node *connectionheaderNode = newChild(parent);
    initNode(connectionheaderNode, "Connection_header", ptr, 0);

    if (startWith("Connection", ptr)) {
        initNode(newChild(connectionheaderNode), "case_insensitive_string", ptr, 10);
        ptr += getLength(getLastChild(connectionheaderNode));

        if (*ptr == ':') {
            initNode(newChild(connectionheaderNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(connectionheaderNode));

            if (detect_OWS(connectionheaderNode, ptr) == 0) {
                ptr += getLength(getLastChild(connectionheaderNode));
            }

            if (detect_Connection(connectionheaderNode, ptr) == 0) {
                ptr += getLength(getLastChild(connectionheaderNode));

                if (detect_OWS(connectionheaderNode, ptr) == 0) {
                    ptr += getLength(getLastChild(connectionheaderNode));
                }
            } else {
                delNode(connectionheaderNode, parent);
                return 74;
            }
        } else {
            delNode(connectionheaderNode, parent);
            return 74;
        }
    } else {
        delNode(connectionheaderNode, parent);
        return 74;
    }

    setLength(connectionheaderNode, getSumLengthChildren(connectionheaderNode));

    return 0;
}

int detect_Connection(Node *parent, const char *ptr) {
    Node *connectionNode = newChild(parent);
    initNode(connectionNode, "Connection", ptr, 0);

    while (1) {
        if (*ptr == ',') {
            initNode(newChild(connectionNode), "case_insensitive_string", ptr, 1);
            ptr += 1;

            if (detect_OWS(connectionNode, ptr) == 0) {
                ptr += getLength(getLastChild(connectionNode));
            }
        } else {
            break;
        }
    }

    if (detect_connection_option(connectionNode, ptr) == 0) {
        ptr += getLength(getLastChild(connectionNode));

        while (1) { //problème ici comment savoir si c'est l'OWS du début du * ou de la fin du connection header
            if (detect_OWS(connectionNode, ptr) == 0 && *(ptr + getLength(getLastChild(connectionNode))) == ',') {
                ptr += getLength(getLastChild(connectionNode));
                initNode(newChild(connectionNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(connectionNode));
            } else if (*ptr == ',') {
                initNode(newChild(connectionNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(connectionNode));
            } else {
                if (startWith("OWS", getLabel(getLastChild(connectionNode)))) {
                    delNode(getLastChild(connectionNode), connectionNode);
                }
                break;
            }

            if (detect_OWS(connectionNode, ptr) == 0) {
                ptr += getLength(getLastChild(connectionNode));
            }

            if (detect_connection_option(connectionNode, ptr) == 0) {
                ptr += getLength(getLastChild(connectionNode));
            }
        }
    } else {
        delNode(connectionNode, parent);
        return 76;
    }

    setLength(connectionNode, getSumLengthChildren(connectionNode));

    return 0;
}

int detect_Content_Length_header(Node *parent, const char *ptr) {
    Node *contentLengthHeaderNode = newChild(parent);
    initNode(contentLengthHeaderNode, "Content_Length_header", ptr, 0);

    if (startWith("Content-Length", ptr)) {
        initNode(newChild(contentLengthHeaderNode), "case_insensitive_string", ptr, 14);
        ptr += getLength(getLastChild(contentLengthHeaderNode));

        if (*ptr == ':') {
            initNode(newChild(contentLengthHeaderNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(contentLengthHeaderNode));

            if (detect_OWS(contentLengthHeaderNode, ptr) == 0) {
                ptr += getLength(getLastChild(contentLengthHeaderNode));
            }

            if (detect_Content_Length(contentLengthHeaderNode, ptr) == 0) {
                ptr += getLength(getLastChild(contentLengthHeaderNode));

                if (detect_OWS(contentLengthHeaderNode, ptr) == 0) {
                    ptr += getLength(getLastChild(contentLengthHeaderNode));
                }
            } else {
                delNode(contentLengthHeaderNode, parent);
                return 76;
            }
        } else {
            delNode(contentLengthHeaderNode, parent);
            return 76;
        }
    } else {
        delNode(contentLengthHeaderNode, parent);
        return 76;
    }

    setLength(contentLengthHeaderNode, getSumLengthChildren(contentLengthHeaderNode));

    return 0;
}

int detect_Content_Length(Node *parent, const char *ptr) {
    Node *ContentLengthNode = newChild(parent);
    initNode(ContentLengthNode, "Content_Length", ptr, 0);
    int i = 0;

    while (1) {
        if (detect_DIGIT(ContentLengthNode, ptr) == 0) {
            initNode(newChild(ContentLengthNode), "__digit", ptr, 1);
            ptr += getLength(getLastChild(ContentLengthNode));
            i++;
        } else {
            break;
        }
    }

    if (i < 1) {
        delNode(ContentLengthNode, parent);
        return 77;
    }

    setLength(ContentLengthNode, getSumLengthChildren(ContentLengthNode));

    return 0;
}

int detect_Content_Type_header(Node *parent, const char *ptr) {
    Node *contentTypeHeaderNode = newChild(parent);
    initNode(contentTypeHeaderNode, "Content_Type_header", ptr, 0);

    if (startWith("Content-Type", ptr)) {
        initNode(newChild(contentTypeHeaderNode), "case_insensitive_string", ptr, 12);
        ptr += getLength(getLastChild(contentTypeHeaderNode));

        if (*ptr == ':') {
            initNode(newChild(contentTypeHeaderNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(contentTypeHeaderNode));

            if (detect_OWS(contentTypeHeaderNode, ptr) == 0) {
                ptr += getLength(getLastChild(contentTypeHeaderNode));
            }

            if (detect_Content_Type(contentTypeHeaderNode, ptr) == 0) {
                ptr += getLength(getLastChild(contentTypeHeaderNode));

                if (detect_OWS(contentTypeHeaderNode, ptr) == 0) {
                    ptr += getLength(getLastChild(contentTypeHeaderNode));
                }
            } else {
                delNode(contentTypeHeaderNode, parent);
                return 77;
            }
        } else {
            delNode(contentTypeHeaderNode, parent);
            return 77;
        }

    } else {
        delNode(contentTypeHeaderNode, parent);
        return 77;
    }

    setLength(contentTypeHeaderNode, getSumLengthChildren(contentTypeHeaderNode));

    return 0;
}

int detect_Content_Type(Node *parent, const char *ptr) {
    Node *contentTypeNode = newChild(parent);
    initNode(contentTypeNode, "Content_Type", ptr, 0);

    if (detect_media_type(contentTypeNode, ptr) == 0) {
        ptr += getLength(getLastChild(contentTypeNode));
    } else {
        delNode(contentTypeNode, parent);
        return 78;
    }

    setLength(contentTypeNode, getSumLengthChildren(contentTypeNode));

    return 0;
}

int detect_media_type(Node *parent, const char *ptr) {
    Node *mediaTypeNode = newChild(parent);
    initNode(mediaTypeNode, "media_type", ptr, 0);

    if (detect_type(mediaTypeNode, ptr) == 0) {
        ptr += getLength(getLastChild(mediaTypeNode));

        if (*ptr == '/') {
            initNode(newChild(mediaTypeNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(mediaTypeNode));

            if (detect_subtype(mediaTypeNode, ptr) == 0) {
                ptr += getLength(getLastChild(mediaTypeNode));

                while (1) {
                    if (detect_OWS(mediaTypeNode, ptr) == 0 && *(ptr + getLength(getLastChild(mediaTypeNode))) == ',') {
                        ptr += getLength(getLastChild(mediaTypeNode));
                        initNode(newChild(mediaTypeNode), "case_insensitive_string", ptr, 1);
                        ptr += getLength(getLastChild(mediaTypeNode));

                    } else if (*ptr == ';') {
                        initNode(newChild(mediaTypeNode), "case_insensitive_string", ptr, 1);
                        ptr += getLength(getLastChild(mediaTypeNode));

                        if (detect_OWS(mediaTypeNode, ptr) == 0) {
                            ptr += getLength(getLastChild(mediaTypeNode));
                        }

                        if (detect_parameter(mediaTypeNode, ptr) == 0) {
                            ptr += getLength(getLastChild(mediaTypeNode));
                        } else {
                            delNode(mediaTypeNode, parent);
                            return 79;
                        }
                    } else {
                        if (startWith("OWS", getLabel(getLastChild(mediaTypeNode)))) {
                            delNode(getLastChild(mediaTypeNode), mediaTypeNode);
                        }
                        break;
                    }
                }
            } else {
                delNode(mediaTypeNode, parent);
                return 79;
            }
        } else {
            delNode(mediaTypeNode, parent);
            return 79;
        }
    } else {
        delNode(mediaTypeNode, parent);
        return 79;
    }

    setLength(mediaTypeNode, getSumLengthChildren(mediaTypeNode));

    return 0;
}

int detect_type(Node *parent, const char *ptr) {
    Node *typeNode = newChild(parent);
    initNode(typeNode, "type", ptr, 0);

    if (detect_token(typeNode, ptr) == 0) {
        ptr += getLength(getLastChild(typeNode));
    } else {
        delNode(typeNode, parent);
        return 80;
    }

    setLength(typeNode, getSumLengthChildren(typeNode));

    return 0;
}

int detect_subtype(Node *parent, const char *ptr) {
    Node *subTypeNode = newChild(parent);
    initNode(subTypeNode, "subtype", ptr, 0);

    if (detect_token(subTypeNode, ptr) == 0) {
        ptr += getLength(getLastChild(subTypeNode));
    } else {
        delNode(subTypeNode, parent);
        return 81;
    }

    setLength(subTypeNode, getSumLengthChildren(subTypeNode));

    return 0;
}

int detect_Cookie_header(Node *parent, const char *ptr) {
    Node *cookieHeaderNode = newChild(parent);
    initNode(cookieHeaderNode, "Cookie_header", ptr, 0);

    if (startWith("Cookie:", ptr)) {
        initNode(newChild(cookieHeaderNode), "case_insensitive_string", ptr, 7);
        ptr += getLength(getLastChild(cookieHeaderNode));

        if (detect_OWS(cookieHeaderNode, ptr) == 0) {
            ptr += getLength(getLastChild(cookieHeaderNode));
        }

        if (detect_cookie_string(cookieHeaderNode, ptr) == 0) {
            ptr += getLength(getLastChild(cookieHeaderNode));

            if (detect_OWS(cookieHeaderNode, ptr) == 0) {
                ptr += getLength(getLastChild(cookieHeaderNode));
            }
        } else {
            delNode(cookieHeaderNode, parent);
            return 82;
        }

    } else {
        delNode(cookieHeaderNode, parent);
        return 82;
    }

    setLength(cookieHeaderNode, getSumLengthChildren(cookieHeaderNode));

    return 0;
}

int detect_cookie_string(Node *parent, const char *ptr) {
    Node *cookieStringNode = newChild(parent);
    initNode(cookieStringNode, "cookie_string", ptr, 0);

    if (detect_cookie_pair(cookieStringNode, ptr) == 0) {
        ptr += getLength(getLastChild(cookieStringNode));

        while (1) {
            if (*ptr == ';') {
                initNode(newChild(cookieStringNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(cookieStringNode));

                if (detect_SP(cookieStringNode, ptr) == 0) {
                    ptr += getLength(getLastChild(cookieStringNode));

                    if (detect_cookie_pair(cookieStringNode, ptr) == 0) {
                        ptr += getLength(getLastChild(cookieStringNode));
                    } else {
                        delNode(cookieStringNode, parent);
                        return 83;
                    }
                } else {
                    delNode(cookieStringNode, parent);
                    return 83;
                }
            } else {
                break;
            }
        }
    } else {
        delNode(cookieStringNode, parent);
        return 83;
    }

    setLength(cookieStringNode, getSumLengthChildren(cookieStringNode));

    return 0;
}
