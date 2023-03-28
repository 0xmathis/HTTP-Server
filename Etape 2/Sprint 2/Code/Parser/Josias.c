#include "Mathis.h"
#include "Nathan.h"
#include "Josias.h"
#include "Hugo.h"
#include "utils.h"

int detect_HTTP_message(Node *parent, const char *ptr) {
    if (detect_start_line(parent, ptr) == 0) {
        ptr += getLength(getLastChild(parent));

    } else {
        return 1;
    }

    while (1) {
        if (detect_header_field(parent, ptr) == 0) {
            ptr += getLength(getLastChild(parent));

            if (detect_CRLF(parent, ptr) == 0) {
                ptr += getLength(getLastChild(parent));
            } else {
                return 1;
            }
        } else {
            break;
        }
    }

    if (detect_CRLF(parent, ptr) == 0) {
        ptr += getLength(getLastChild(parent));
    } else {
        return 1;
    }

    if (detect_message_body(parent, ptr) == 0) {
        ptr += getLength(getLastChild(parent));
    }

    setLength(parent, getSumLengthChildren(parent));
    return 0;
}

int detect_ALPHA(const char *ptr) {
    if ((0x41 <= *ptr && *ptr <= 0x5A) || (0x61 <= *ptr && *ptr <= 0x7A)) {
    } else {
        return 2;
    }
    return 0;
}

int detect_alphanum(Node *parent, const char *ptr) {
    Node *alphanumNode = newChild(parent);
    initNode(alphanumNode, "alphanum", ptr, 0);

    if (detect_ALPHA(ptr) == 0) {
        initNode(newChild(alphanumNode), "__alpha", ptr, 1);
    } else if (detect_DIGIT(ptr) == 0) {
        initNode(newChild(alphanumNode), "__digit", ptr, 1);
    } else {
        delNode(alphanumNode, parent);
        return 3;
    }

    setLength(alphanumNode, getSumLengthChildren(alphanumNode));
    return 0;
}

int detect_DIGIT(const char *ptr) {
    if (0x30 <= *ptr && *ptr <= 0x39) {
    } else {
        return 4;
    }

    return 0;
}

int detect_comment(Node *parent, const char *ptr) {
    Node *commentNode = newChild(parent);
    initNode(commentNode, "comment", ptr, 0);

    if (*ptr == '(') {
        initNode(newChild(commentNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(commentNode));
    } else {
        delNode(commentNode, parent);
        return 5;
    }

    while (1) {
        if (detect_ctext(commentNode, ptr) == 0) {
            ptr += getLength(getLastChild(commentNode));
        } else if (detect_quoted_pair(commentNode, ptr) == 0) {
            ptr += getLength(getLastChild(commentNode));
        } else if (detect_comment(commentNode, ptr) == 0) {
            ptr += getLength(getLastChild(commentNode));
        } else {
            break;
        }
    }

    if (*ptr == ')') {
        initNode(newChild(commentNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(commentNode));
    } else {
        delNode(commentNode, parent);
        return 5;
    }

    setLength(commentNode, getSumLengthChildren(commentNode));
    return 0;
}

int detect_ctext(Node *parent, const char *ptr) {
    Node *ctextNode = newChild(parent);
    initNode(ctextNode, "ctext", ptr, 0);

    if (detect_HTAB(ctextNode, ptr) == 0 || detect_SP(ctextNode, ptr) == 0 || detect_obs_text(parent, ptr) == 0) {
        ptr += getLength(getLastChild(ctextNode));
    } else if ((0x21 <= *ptr && *ptr <= 0x27) || (0x2A <= *ptr && *ptr <= 0x5B) || (0x5D <= *ptr && *ptr <= 0x7E)) {
        initNode(newChild(ctextNode), "__range", ptr, 1);
        ptr += getLength(getLastChild(ctextNode));
    } else {
        delNode(ctextNode, parent);
        return 6;
    }

    setLength(ctextNode, getSumLengthChildren(ctextNode));
    return 0;
}

int detect_quoted_pair(Node *parent, const char *ptr) {
    Node *quotedPairNode = newChild(parent);
    initNode(quotedPairNode, "quoted_pair", ptr, 0);

    if (*ptr == '\\') {
        initNode(newChild(quotedPairNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(quotedPairNode));
    } else {
        delNode(quotedPairNode, parent);
        return 7;
    }

    if (detect_HTAB(quotedPairNode, ptr) == 0 || detect_SP(quotedPairNode, ptr) == 0 || detect_VCHAR(quotedPairNode, ptr) == 0 || detect_obs_text(quotedPairNode, ptr) == 0) {
        ptr += getLength(getLastChild(quotedPairNode));
    } else {
        delNode(quotedPairNode, parent);
        return 7;
    }

    setLength(quotedPairNode, getSumLengthChildren(quotedPairNode));
    return 0;
}

int detect_connection_option(Node *parent, const char *ptr) {
    Node *connectionOptionNode = newChild(parent);
    initNode(connectionOptionNode, "connection_option", ptr, 0);

    if (detect_token(connectionOptionNode, ptr) == 0) {
        ptr += getLength(getLastChild(connectionOptionNode));
    } else {
        delNode(connectionOptionNode, parent);
        return 7;
    }

    setLength(connectionOptionNode, getSumLengthChildren(connectionOptionNode));
    return 0;
}

int detect_token(Node *parent, const char *ptr) {
    Node *tokenNode = newChild(parent);
    initNode(tokenNode, "token", ptr, 0);
    int compteur = 0;

    while (1) {
        if (detect_tchar(tokenNode, ptr) == 0) {
            ptr += getLength(getLastChild(tokenNode));
            compteur++;
        } else {
            break;
        }
    }

    if (compteur < 1) {
        delNode(tokenNode, parent);
        return 8;
    }

    setLength(tokenNode, getSumLengthChildren(tokenNode));
    return 0;
}

int detect_codings(Node *parent, const char *ptr) {
    Node *codingsNode = newChild(parent);
    initNode(codingsNode, "codings", ptr, 0);

    if (detect_content_coding(codingsNode, ptr) == 0) {
        ptr += getLength(getLastChild(codingsNode));
    } else if (startWith("identity", ptr)) {
        initNode(newChild(parent), "insentive_case_string", ptr, 8);
    } else if (*ptr == '*') {
        initNode(newChild(parent), "insentive_case_string", ptr, 1);
    } else {
        delNode(codingsNode, parent);
        return 9;
    }

    setLength(codingsNode, getSumLengthChildren(codingsNode));
    return 0;
}

int detect_content_coding(Node *parent, const char *ptr) {
    Node *contentCodingNode = newChild(parent);
    initNode(contentCodingNode, "content_coding", ptr, 0);

    if (detect_token(contentCodingNode, ptr) == 0) {
        ptr += getLength(getLastChild(contentCodingNode));
    } else {
        delNode(contentCodingNode, parent);
        return 10;
    }

    setLength(contentCodingNode, getSumLengthChildren(contentCodingNode));
    return 0;
}

int detect_cookie_octet(Node *parent, const char *ptr) {
    Node *cookieOctetNode = newChild(parent);
    initNode(cookieOctetNode, "cookie_octet", ptr, 0);

    if (*ptr == '!') {
        initNode(newChild(cookieOctetNode), "__num", ptr, 1);
    } else if (*ptr == 0x21 || (0x23 <= *ptr && *ptr <= 0x2B) || (0x2D <= *ptr && *ptr <= 0x3A) || (0x3C <= *ptr && *ptr <= 0x5B) || (0x5D <= *ptr && *ptr <= 0x7E)) {
        initNode(newChild(cookieOctetNode), "__range", ptr, 1);
    } else {
        delNode(cookieOctetNode, parent);
        return 11;
    }

    setLength(cookieOctetNode, getSumLengthChildren(cookieOctetNode));
    return 0;
}

int detect_cookie_pair(Node *parent, const char *ptr) {
    Node *cookiePairNode = newChild(parent);
    initNode(cookiePairNode, "cookie_pair", ptr, 0);

    if (detect_cookie_name(cookiePairNode, ptr) == 0) {
        ptr += getLength(getLastChild(cookiePairNode));
    } else {
        delNode(cookiePairNode, parent);
        return 12;
    }

    if (*ptr == '=') {
        initNode(newChild(cookiePairNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(cookiePairNode));
    } else {
        delNode(cookiePairNode, parent);
        return 12;
    }

    if (detect_cookie_value(cookiePairNode, ptr) == 0) {
        ptr += getLength(getLastChild(cookiePairNode));
    } else {
        delNode(cookiePairNode, parent);
        return 12;
    }

    setLength(cookiePairNode, getSumLengthChildren(cookiePairNode));
    return 0;
}

int detect_cookie_name(Node *parent, const char *ptr) {
    Node *cookieNameNode = newChild(parent);
    initNode(cookieNameNode, "cookie_name", ptr, 0);

    if (detect_token(cookieNameNode, ptr) == 0) {
        ptr += getLength(getLastChild(cookieNameNode));
    } else {
        delNode(cookieNameNode, parent);
        return 13;
    }

    setLength(cookieNameNode, getSumLengthChildren(cookieNameNode));
    return 0;
}

int detect_cookie_value(Node *parent, const char *ptr) {
    Node *cookieValueNode = newChild(parent);
    initNode(cookieValueNode, "cookie_value", ptr, 0);

    if (detect_DQUOTE(cookieValueNode, ptr) == 0) {
        ptr += getLength(getLastChild(cookieValueNode));

        while (1) {
            if (detect_cookie_octet(cookieValueNode, ptr) == 0) {
                ptr += getLength(getLastChild(cookieValueNode));
            } else {
                break;
            }
        }

        if (detect_DQUOTE(cookieValueNode, ptr) == 0) {
            ptr += getLength(getLastChild(cookieValueNode));
        } else {
            delNode(cookieValueNode, parent);
            return 14;
        }

    } else {
        int compteur = 0;
        while (1) {
            if (detect_cookie_octet(cookieValueNode, ptr) == 0) {
                ptr += getLength(getLastChild(cookieValueNode));
                compteur++;
            } else {
                break;
            }
        }

        if (compteur == 0) {
            delNode(cookieValueNode, parent);
        }
    }

    setLength(cookieValueNode, getSumLengthChildren(cookieValueNode));
    return 0;

}

int detect_CRLF(Node *parent, const char *ptr) {
    if (startWith("\r\n", ptr)) {
        initNode(newChild(parent), "__crlf", ptr, 2);
    } else {
        return 15;
    }

    return 0;
}

int detect_HTAB(Node *parent, const char *ptr) {
    if (*ptr == '\t') {
        initNode(newChild(parent), "__htab", ptr, 1);
    } else {
        return 16;
    }

    return 0;
}

int detect_SP(Node *parent, const char *ptr) {
    if (*ptr == ' ') {
        initNode(newChild(parent), "__sp", ptr, 1);
    } else {
        return 17;
    }

    return 0;
}

int detect_obs_text(Node *parent, const char *ptr) {
    Node *obsTextNode = newChild(parent);
    initNode(obsTextNode, "obs_text", ptr, 0);

    if (0x80 <= (*ptr & 0xFF) && (*ptr & 0xFF) <= 0xFF) {
        initNode(newChild(obsTextNode), "__range", ptr, 1);
    } else {
        delNode(obsTextNode, parent);
        return 18;
    }

    setLength(obsTextNode, getSumLengthChildren(obsTextNode));
    return 0;
}

int detect_dec_octet(Node *parent, const char *ptr) {
    Node *decOctetNode = newChild(parent);
    initNode(decOctetNode, "dec_octet", ptr, 0);

    if (startWith("25", ptr) && 0x30 <= *(ptr + 2) && *(ptr + 2) <= 0x35) {
        initNode(newChild(decOctetNode), "case_insensitive_string", ptr, 2);
        initNode(newChild(decOctetNode), "__digit", ptr + 2, 1);
        ptr += 3;
    } else if (*ptr == '2' && 0x30 <= *(ptr + 1) && *(ptr + 1) <= 0x34 && detect_DIGIT(ptr + 2) == 0) {
        initNode(newChild(decOctetNode), "case_insensitive_string", ptr, 1);
        initNode(newChild(decOctetNode), "__range", ptr + 1, 1);
        initNode(newChild(decOctetNode), "__digit", ptr + 2, 1);
        ptr += 3;
    } else if (*ptr == '1' && detect_DIGIT(ptr + 1) == 0 && detect_DIGIT(ptr + 2) == 0) {
        initNode(newChild(decOctetNode), "case_insensitive_string", ptr, 1);
        initNode(newChild(decOctetNode), "__digit", ptr + 1, 1);
        initNode(newChild(decOctetNode), "__digit", ptr + 2, 1);
        ptr += 3;
    } else if (0x31 <= *ptr && *ptr <= 0x39 && detect_DIGIT(ptr + 1) == 0) {
        initNode(newChild(decOctetNode), "__range", ptr, 1);
        initNode(newChild(decOctetNode), "__digit", ptr + 1, 1);
        ptr += 2;
    } else if (detect_DIGIT(ptr) == 0) {
        initNode(newChild(decOctetNode), "__digit", ptr, 1);
        ptr += 1;
    } else {
        delNode(decOctetNode, parent);
        return 19;
    }

    setLength(decOctetNode, getSumLengthChildren(decOctetNode));
    return 0;
}

int detect_field_vchar(Node *parent, const char *ptr) {
    Node *fieldVcharNode = newChild(parent);
    initNode(fieldVcharNode, "field_vchar", ptr, 0);

    if (detect_VCHAR(fieldVcharNode, ptr) == 0) {
        ptr += getLength(getLastChild(fieldVcharNode));
    } else if (detect_obs_text(fieldVcharNode, ptr) == 0) {
        ptr += getLength(getLastChild(fieldVcharNode));
    } else {
        delNode(fieldVcharNode, parent);
        return 20;
    }

    setLength(fieldVcharNode, getSumLengthChildren(fieldVcharNode));
    return 0;
}

int detect_VCHAR(Node *parent, const char *ptr) {
    if (0x21 <= *ptr && *ptr <= 0x7E) {
        initNode(newChild(parent), "__vchar", ptr, 1);
    } else {
        return 21;
    }

    return 0;
}

int detect_h16(Node *parent, const char *ptr) {
    Node *h16Node = newChild(parent);
    initNode(h16Node, "h16", ptr, 0);
    int comptage = 0;

    while (1) {
        if (detect_HEXDIG(h16Node, ptr) == 0) {
            ptr += getLength(getLastChild(h16Node));
            comptage++;
        } else {
            break;
        }
    }

    if (comptage < 1 || 4 < comptage) {
        delNode(h16Node, parent);
        return 22;
    }

    setLength(h16Node, getSumLengthChildren(h16Node));
    return 0;
}

int detect_HEXDIG(Node *parent, const char *ptr) {
    if ((0x30 <= *ptr && *ptr <= 0x39) || (0x41 <= *ptr && *ptr <= 0x46) || (0x61 <= *ptr && *ptr <= 0x66)) {
        initNode(newChild(parent), "__hexdig", ptr, 1);
    } else {
        return 23;
    }

    return 0;
}

