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
        return 61;
    }

    setLength(startLineNode, getSumLengthChildren(startLineNode));

    return 0;
}

int detect_request_line(Node *parent, const char *ptr) {
    Node *requestLineNode = newChild(parent);
    initNode(requestLineNode, "request_line", ptr, 0);

    if (!detect_method(requestLineNode, ptr)) {
        ptr += getLength(getLastChild(requestLineNode));
    } else {
        return 62;
    }

    if (!detect_SP(requestLineNode, ptr)) {
        ptr += getLength(getLastChild(requestLineNode));
    } else {
        return 62;
    }

    if (!detect_request_target(requestLineNode, ptr)) {
        ptr += getLength(getLastChild(requestLineNode));
    } else {
        return 62;
    }

    if (!detect_SP(requestLineNode, ptr)) {
        ptr += getLength(getLastChild(requestLineNode));
    } else {
        return 62;
    }

    if (!detect_HTTP_version(requestLineNode, ptr)) {
        ptr += getLength(getLastChild(requestLineNode));
    } else {
        return 62;
    }

    if (!detect_CRLF(requestLineNode, ptr)) {
        ptr += getLength(getLastChild(requestLineNode));
    } else {
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

    if (!detect_origin_form(requestTargetNode, ptr)) {
        ptr += getLength(getLastChild(requestTargetNode));
    } else {
        return 64;
    }

    setLength(requestTargetNode, getSumLengthChildren(requestTargetNode));

    return 0;
}

int detect_origin_form(Node *parent, const char *ptr) {
    Node *originFormNode = newChild(parent);
    initNode(originFormNode, "origin_form", ptr, 0);

    if (!detect_absolute_path(originFormNode, ptr)) {
        ptr += getLength(getLastChild(originFormNode));
    } else {
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

            if (detect_segment(absolutePathNode, ptr) == 0) {
                ptr += getLength(getLastChild(absolutePathNode));
                compteur++;
            }
        } else {
            break;
        }
    }

    if (compteur == 0) {
        delNode(absolutePathNode, parent);
        return 66;
    }

    setLength(absolutePathNode, getSumLengthChildren(absolutePathNode) + 1);

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

    setLength(HTTPVersionNode, getSumLengthChildren(HTTPVersionNode) + 2);

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

    while (1) {

        if (!detect_field_name(headerFieldNode, ptr)) {

            ptr += getLength(getLastChild(headerFieldNode));

            if (*ptr == ':') {
                Node *TDNode = newChild(headerFieldNode);
                initNode(TDNode, ":", ptr, 1);
                ptr += 1;
            } else {
                return 69;
            }

            if (!detect_OWS(headerFieldNode, ptr)) {
                ptr += getLength(getLastChild(headerFieldNode));
            } else {
                return 69;
            }

            if (!detect_field_value(headerFieldNode, ptr)) {
                ptr += getLength(getLastChild(headerFieldNode));
            } else {
                return 69;
            }

            if (!detect_OWS(headerFieldNode, ptr)) {
                ptr += getLength(getLastChild(headerFieldNode));
            } else {
                return 69;
            }

        } else if (!detect_Connection_header(headerFieldNode, ptr)) {
            ptr += getLength(getLastChild(headerFieldNode));

        } else if (!detect_Content_Length_header(headerFieldNode, ptr)) {
            ptr += getLength(getLastChild(headerFieldNode));

        } else if (!detect_Content_Type_header(headerFieldNode, ptr)) {
            ptr += getLength(getLastChild(headerFieldNode));

        } else if (!detect_Cookie_header(headerFieldNode, ptr)) {
            ptr += getLength(getLastChild(headerFieldNode));

//        } else if (!detect_Transfer_Encoding_header(headerFieldNode, ptr)) {
//            ptr += getLength(getLastChild(headerFieldNode));

        } else if (!detect_Expect_header(headerFieldNode, ptr)) {
            ptr += getLength(getLastChild(headerFieldNode));

        } else if (!detect_Host_header(headerFieldNode, ptr)) {
            ptr += getLength(getLastChild(headerFieldNode));

        } else if (!detect_User_Agent_header(headerFieldNode, ptr)) {
            ptr += getLength(getLastChild(headerFieldNode));

        } else if (!detect_Accept_Language_header(headerFieldNode, ptr)) {
            ptr += getLength(getLastChild(headerFieldNode));

        } else if (!detect_Accept_header(headerFieldNode, ptr)) {
            ptr += getLength(getLastChild(headerFieldNode));

        } else if (!detect_Accept_Encoding_header(headerFieldNode, ptr)) {
            ptr += getLength(getLastChild(headerFieldNode));

        } else if (!detect_CRLF(headerFieldNode, ptr)) {
            break;
        } else {
            return 69;
        }
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

    while (1) {
        if (!detect_field_content(fieldValueNode, ptr)) {
            ptr += getLength(getLastChild(fieldValueNode));

        } else if (!detect_obs_fold(fieldValueNode, ptr)) {
            ptr += getLength(getLastChild(fieldValueNode));

        } else if (!detect_OWS(fieldValueNode, ptr)) {
            // supprimer le dernier fils qui vient d'être créer
            break;

        } else {
            return 71;
        }
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

    if (detect_RWS(fieldContentNode, ptr) == 0) {
        ptr += getLength(getLastChild(fieldContentNode));

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
    Node *ConnectionheaderNode = newChild(parent);
    initNode(ConnectionheaderNode, "Connection_header", ptr, 0);

    if (*ptr == 'C' && *ptr + 1 == 'o' && *ptr + 2 == 'n' & *ptr + 3 == 'n' && *ptr + 4 == 'e' && *ptr + 5 == 'c' && *ptr + 6 == 't' & *ptr + 7 == 'i'
        && *ptr + 8 == 'o' && *ptr + 9 == 'n' && *ptr + 10 == ':') {

        Node *ConnNode = newChild(ConnectionheaderNode);
        initNode(ConnNode, "Connection", ptr, 10);
        ptr += 10;
        Node *TDNode = newChild(ConnectionheaderNode);
        initNode(TDNode, ":", ptr, 1);
        ptr += 1;
    } else {
        return 74;
    }

    if (!detect_OWS(ConnectionheaderNode, ptr)) {
        ptr += getLength(getLastChild(ConnectionheaderNode));
    } else {
        return 74;
    }

    if (!detect_Connection(ConnectionheaderNode, ptr)) {
        ptr += getLength(getLastChild(ConnectionheaderNode));
    } else {
        return 74;
    }

    if (!detect_OWS(ConnectionheaderNode, ptr)) {
        ptr += getLength(getLastChild(ConnectionheaderNode));
    } else {
        return 74;
    }

    setLength(ConnectionheaderNode, getSumLengthChildren(ConnectionheaderNode));

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
            if (detect_OWS(connectionNode, ptr) == 0) {
                ptr += getLength(getLastChild(connectionNode));
            }

            if (*ptr == ',') {
                initNode(newChild(connectionNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(connectionNode));
            } else {
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
    Node *ContentLengthHeaderNode = newChild(parent);
    initNode(ContentLengthHeaderNode, "Content_Length_header", ptr, 0);

    if (*ptr == 'C' && *ptr + 1 == 'o' && *ptr + 2 == 'n' & *ptr + 3 == 't' && *ptr + 4 == 'e' && *ptr + 5 == 'n' && *ptr + 6 == 't' & *ptr + 7 == '-'
        && *ptr + 8 == 'L' && *ptr + 9 == 'e' && *ptr + 10 == 'n' && *ptr + 11 == 'g' && *ptr + 12 == 't' && *ptr + 13 == 'h' && *ptr + 14 == ':') {

        Node *CLNode = newChild(ContentLengthHeaderNode);
        initNode(CLNode, "Content_Length", ptr, 14);
        ptr += 14;
        Node *DDNode = newChild(ContentLengthHeaderNode);
        initNode(DDNode, ":", ptr, 1);
        ptr += 1;
    } else {
        return 76;
    }

    if (!detect_OWS(ContentLengthHeaderNode, ptr)) {
        ptr += getLength(getLastChild(ContentLengthHeaderNode));
    } else {
        return 76;
    }

    if (!detect_Content_Length(ContentLengthHeaderNode, ptr)) {
        ptr += getLength(getLastChild(ContentLengthHeaderNode));
    } else {
        return 76;
    }

    if (!detect_OWS(ContentLengthHeaderNode, ptr)) {
        ptr += getLength(getLastChild(ContentLengthHeaderNode));
    } else {
        return 76;
    }

    setLength(ContentLengthHeaderNode, getSumLengthChildren(ContentLengthHeaderNode));

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
                if (detect_OWS(contentTypeHeaderNode, ptr) == 0) {
                    ptr += getLength(getLastChild(contentTypeHeaderNode));
                }
            } else {
                return 77;
            }
        } else {
            return 77;
        }

    } else {
        return 77;
    }

    setLength(contentTypeHeaderNode, getSumLengthChildren(contentTypeHeaderNode));

    return 0;
}

int detect_Content_Type(Node *parent, const char *ptr) {
    Node *ContentTypeNode = newChild(parent);
    initNode(ContentTypeNode, "Content_Type", ptr, 0);

    if (!detect_media_type(ContentTypeNode, ptr)) {
        ptr += getLength(getLastChild(ContentTypeNode));
    } else {
        return 78;
    }

    setLength(ContentTypeNode, getSumLengthChildren(ContentTypeNode));

    return 0;
}

int detect_media_type(Node *parent, const char *ptr) {
    Node *MediaTypeNode = newChild(parent);
    initNode(MediaTypeNode, "media_type", ptr, 0);

    if (!detect_type(MediaTypeNode, ptr)) {
        ptr += getLength(getLastChild(MediaTypeNode));
    } else {
        return 79;
    }

    if (*ptr == '/') {
        Node *SlashNode = newChild(MediaTypeNode);
        initNode(SlashNode, "/", ptr, 1);
        ptr += 1;
    } else {
        return 79;
    }

    if (!detect_subtype(MediaTypeNode, ptr)) {
        ptr += getLength(getLastChild(MediaTypeNode));
    } else {
        return 79;
    }

    while (1) { //pareil ici comment sortir de cette boucle
        if (!detect_OWS(MediaTypeNode, ptr)) {
            ptr += getLength(getLastChild(MediaTypeNode));
        } else {
            return 79;
        }

        if (*ptr == ';') {
            Node *PVNode = newChild(MediaTypeNode);
            initNode(PVNode, ";", ptr, 1);
            ptr += 1;
        } else {
            return 79;
        }

        if (!detect_OWS(MediaTypeNode, ptr)) {
            ptr += getLength(getLastChild(MediaTypeNode));
        } else {
            return 79;
        }

        if (!detect_parameter(MediaTypeNode, ptr)) {
            ptr += getLength(getLastChild(MediaTypeNode));
        } else {
            return 79;
        }

    }

    setLength(MediaTypeNode, getSumLengthChildren(MediaTypeNode));

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
    Node *CookieHeaderNode = newChild(parent);
    initNode(CookieHeaderNode, "Cookie_header", ptr, 0);

    if (*ptr == 'C' && *ptr + 1 == 'o' && *ptr + 2 == 'o' & *ptr + 3 == 'k' && *ptr + 4 == 'i' && *ptr + 5 == 'e' & *ptr + 6 == ':') {
        Node *CooNode = newChild(CookieHeaderNode);
        initNode(CooNode, "Cookie", ptr, 6);
        ptr += 6;
        Node *DDNode = newChild(CookieHeaderNode);
        initNode(DDNode, ":", ptr, 1);
        ptr += 1;
    } else {
        return 82;
    }

    if (!detect_cookie_string(CookieHeaderNode, ptr)) {
        ptr += getLength(getLastChild(CookieHeaderNode));
    } else {
        return 82;
    }

    setLength(CookieHeaderNode, getSumLengthChildren(CookieHeaderNode));

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
