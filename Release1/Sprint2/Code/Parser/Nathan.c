#include "Mathis.h"
#include "Nathan.h"
#include "Josias.h"
#include "utils.h"

int detect_language_range(Node *parent, const char *ptr) {
    Node *languageNode = newChild(parent);
    initNode(languageNode, "language_range", ptr, 0);

    if (detect_ALPHA(ptr) == 0) {
        initNode(newChild(languageNode), "__alpha", ptr, 1);
        ptr += getLength(getLastChild(languageNode));
        int comptage = 1;

        while (1) {
            if (detect_ALPHA(ptr) == 0) {
                initNode(newChild(languageNode), "__alpha", ptr, 1);
                ptr += getLength(getLastChild(languageNode));
            } else {
                break;
            }

            comptage++;
        }

        if (comptage <= 8) {
            while (*ptr == '-') {
                initNode(newChild(languageNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(languageNode));
                comptage = 0;

                while (detect_alphanum(languageNode, ptr) == 0) {
                    ptr += getLength(getLastChild(languageNode));
                    comptage += 1;
                }

                if (comptage == 0 || comptage > 8) {
                    delNode(languageNode, parent);
                    return 31;
                }
            }
        } else {
            delNode(languageNode, parent);
            return 31;
        }
    } else if (*ptr == '*') {
        initNode(newChild(languageNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(languageNode));
    } else {
        delNode(languageNode, parent);
        return 31;
    }

    setLength(languageNode, getSumLengthChildren(languageNode));
    return 0;
}

int detect_ls32(Node *parent, const char *ptr) {
    Node *ls32Node = newChild(parent);
    initNode(ls32Node, "ls32", ptr, 0);

    if (detect_IPv4address(ls32Node, ptr) == 0) {
        ptr += getLength(getLastChild(ls32Node));
    } else if (detect_h16(ls32Node, ptr) == 0) {
        ptr += getLength(getLastChild(ls32Node));

        if (*ptr == ':') {
            initNode(newChild(ls32Node), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(ls32Node));

            if (detect_h16(ls32Node, ptr) == 0) {
                ptr += getLength(getLastChild(ls32Node));

            } else {
                delNode(ls32Node, parent);
                return 32;
            }
        } else {
            delNode(ls32Node, parent);
            return 32;
        }
    } else {
        delNode(ls32Node, parent);
        return 32;
    }

    setLength(ls32Node, getSumLengthChildren(ls32Node));
    return 0;
}

int detect_OWS(Node *parent, const char *ptr) {
    Node *owsNode = newChild(parent);
    initNode(owsNode, "OWS", ptr, 0);
    int compteur = 0;

    while (1) {
        if (*ptr == ' ') {
            initNode(newChild(owsNode), "__sp", ptr, 1);
            ptr += getLength(getLastChild(owsNode));
            compteur++;
        } else if (*ptr == '\t') {
            initNode(newChild(owsNode), "__htab", ptr, 1);
            ptr += getLength(getLastChild(owsNode));
            compteur++;
        } else {
            break;
        }
    }

    if (compteur == 0) {
        delNode(owsNode, parent);
        return 60;
    }

    setLength(owsNode, getSumLengthChildren(owsNode));

    return 0;
}

int detect_BWS(Node *parent, const char *ptr) {
    Node *bwsNode = newChild(parent);
    initNode(bwsNode, "BWS", ptr, 0);

    if (detect_OWS(bwsNode, ptr) != 0) {
        delNode(bwsNode, parent);
        return 59;
    }

    setLength(bwsNode, getSumLengthChildren(bwsNode));
    return 0;
}

int detect_parameter(Node *parent, const char *ptr) {
    Node *parameterNode = newChild(parent);
    initNode(parameterNode, "parameter", ptr, 0);

    if (detect_token(parameterNode, ptr) == 0) {
        ptr += getLength(getLastChild(parameterNode));
    } else {
        delNode(parameterNode, parent);
        return 33;
    }

    if (*ptr == '=') {
        initNode(newChild(parameterNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(parameterNode));
    } else {
        delNode(parameterNode, parent);
        return 33;
    }

    if (detect_token(parameterNode, ptr) == 0) {
        ptr += getLength(getLastChild(parameterNode));
    } else if (detect_quoted_string(parameterNode, ptr) == 0) {
        ptr += getLength(getLastChild(parameterNode));
    } else {
        delNode(parameterNode, parent);
        return 33;
    }

    setLength(parameterNode, getSumLengthChildren(parameterNode));
    return 0;
}

int detect_quoted_string(Node *parent, const char *ptr) {
    Node *quotedStringNode = newChild(parent);
    initNode(quotedStringNode, "quoted_string", ptr, 0);

    if (detect_DQUOTE(quotedStringNode, ptr) == 0) {
        ptr += getLength(getLastChild(quotedStringNode));
    } else {
        delNode(quotedStringNode, parent);
        return 34;
    }

    while (1) {
        if (detect_qdtext(quotedStringNode, ptr) == 0 || detect_quoted_pair(quotedStringNode, ptr) == 0) {
            ptr += getLength(getLastChild(quotedStringNode));
        } else {
            break;
        }
    }

    if (detect_DQUOTE(quotedStringNode, ptr) == 0) {
        ptr += getLength(getLastChild(quotedStringNode));
    } else {
        delNode(quotedStringNode, parent);
        return 34;
    }

    setLength(quotedStringNode, getSumLengthChildren(quotedStringNode));
    return 0;
}

int detect_pchar(Node *parent, const char *ptr) {
    Node *pcharNode = newChild(parent);
    initNode(pcharNode, "pchar", ptr, 0);

    if (detect_unreserved(pcharNode, ptr) == 0) {
        ptr += getLength(getLastChild(pcharNode));
    } else if (detect_pct_encoded(pcharNode, ptr) == 0) {
        ptr += getLength(getLastChild(pcharNode));
    } else if (detect_sub_delims(pcharNode, ptr) == 0) {
        ptr += getLength(getLastChild(pcharNode));
    } else if (*ptr == ':') {
        initNode(newChild(pcharNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(pcharNode));
    } else if (*ptr == '@') {
        initNode(newChild(pcharNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(pcharNode));
    } else {
        delNode(pcharNode, parent);
        return 35;
    }

    setLength(pcharNode, getSumLengthChildren(pcharNode));
    return 0;
}

int detect_unreserved(Node *parent, const char *ptr) {
    Node *unreservedNode = newChild(parent);
    initNode(unreservedNode, "unreserved", ptr, 0);

    if (detect_ALPHA(ptr) == 0) {
        initNode(newChild(unreservedNode), "__alpha", ptr, 1);
        setLength(unreservedNode, 1);
    } else if (detect_DIGIT(ptr) == 0) {
        initNode(newChild(unreservedNode), "__digit", ptr, 1);
        setLength(unreservedNode, 1);
    } else if (*ptr == '-') {
        initNode(newChild(unreservedNode), "case_insensitive_string", ptr, 1);
        setLength(unreservedNode, 1);
    } else if (*ptr == '.') {
        initNode(newChild(unreservedNode), "case_insensitive_string", ptr, 1);
        setLength(unreservedNode, 1);
    } else if (*ptr == '_') {
        initNode(newChild(unreservedNode), "case_insensitive_string", ptr, 1);
        setLength(unreservedNode, 1);
    } else if (*ptr == '~') {
        initNode(newChild(unreservedNode), "case_insensitive_string", ptr, 1);
        setLength(unreservedNode, 1);
    } else {
        delNode(unreservedNode, parent);
        return 36;
    }

    setLength(unreservedNode, getSumLengthChildren(unreservedNode));
    return 0;
}

int detect_pct_encoded(Node *parent, const char *ptr) {
    Node *pct_encodedNode = newChild(parent);
    initNode(pct_encodedNode, "pct_encoded", ptr, 0);

    if (*ptr == '%') {
        initNode(newChild(pct_encodedNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(pct_encodedNode));
    } else {
        delNode(pct_encodedNode, parent);
        return 37;
    }

    if (detect_HEXDIG(pct_encodedNode, ptr) == 0) {
        ptr += getLength(getLastChild(pct_encodedNode));
    } else {
        delNode(pct_encodedNode, parent);
        return 37;
    }

    if (detect_HEXDIG(pct_encodedNode, ptr) == 0) {
        ptr += getLength(getLastChild(pct_encodedNode));
    } else {
        delNode(pct_encodedNode, parent);
        return 37;
    }

    setLength(pct_encodedNode, getSumLengthChildren(pct_encodedNode));
    return 0;
}

int detect_sub_delims(Node *parent, const char *ptr) {
    Node *sub_delimsNode = newChild(parent);
    initNode(sub_delimsNode, "sub_delims", ptr, 0);

    if (*ptr == '!') {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '$') {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '&') {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '\'') {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '(') {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == ')') {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '*') {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '+') {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == ',') {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == ';') {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '=') {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else {
        delNode(sub_delimsNode, parent);
        return 38;
    }

    setLength(sub_delimsNode, getSumLengthChildren(sub_delimsNode));
    return 0;
}

int detect_product(Node *parent, const char *ptr) {
    Node *productNode = newChild(parent);
    initNode(productNode, "product", ptr, 0);

    if (detect_token(productNode, ptr) == 0) {
        ptr += getLength(getLastChild(productNode));
    } else {
        delNode(productNode, parent);
        return 40;
    }

    if (*ptr == '/') {
        initNode(newChild(productNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(productNode));

        if (detect_product_version(productNode, ptr) == 0) {
            ptr += getLength(getLastChild(productNode));
        } else {
            delNode(productNode, parent);
            return 40;
        }
    }

    setLength(productNode, getSumLengthChildren(productNode));
    return 0;
}

int detect_product_version(Node *parent, const char *ptr) {
    Node *productVersionNode = newChild(parent);
    initNode(productVersionNode, "product_version", ptr, 0);

    if (detect_token(productVersionNode, ptr) == 0) {
        ptr += getLength(getLastChild(productVersionNode));
    } else {
        delNode(productVersionNode, parent);
        return 41;
    }

    setLength(productVersionNode, getSumLengthChildren(productVersionNode));
    return 0;
}

int detect_qdtext(Node *parent, const char *ptr) {
    Node *qdtextNode = newChild(parent);
    initNode(qdtextNode, "qdtext", ptr, 0);

    if (detect_HTAB(qdtextNode, ptr) == 0) {
        ptr += getLength(getLastChild(qdtextNode));
    } else if (detect_SP(qdtextNode, ptr) == 0) {
        ptr += getLength(getLastChild(qdtextNode));
    } else if (*ptr == '!') {
        initNode(newChild(qdtextNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(qdtextNode));
    } else if (detect_ALPHA(ptr) == 0) {
        initNode(newChild(qdtextNode), "__range", ptr, 1);
        ptr += getLength(getLastChild(qdtextNode));
    } else if (detect_DIGIT(ptr) == 0) {
        initNode(newChild(qdtextNode), "__range", ptr, 1);
        ptr += getLength(getLastChild(qdtextNode));
    } else if ((0x23 <= *ptr && *ptr <= 0x5B) || (0x5D <= *ptr && *ptr <= 0x7E)) {
        initNode(newChild(qdtextNode), "__range", ptr, 1);
        ptr += getLength(getLastChild(qdtextNode));
    } else if (detect_obs_text(qdtextNode, ptr) == 0) {
        ptr += getLength(getLastChild(qdtextNode));
    } else {
        delNode(qdtextNode, parent);
        return 42;
    }

    setLength(qdtextNode, getSumLengthChildren(qdtextNode));
    return 0;
}

int detect_query(Node *parent, const char *ptr) {
    Node *queryNode = newChild(parent);
    initNode(queryNode, "query", ptr, 0);
    int compteur = 0;

    while (1) {
        if (detect_pchar(queryNode, ptr) == 0) {
            ptr += getLength(getLastChild(queryNode));
            compteur++;
        } else if (*ptr == '/' || *ptr == '?') {
            initNode(newChild(queryNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(queryNode));
            compteur++;
        } else {
            break;
        }
    }

    if (compteur == 0) {
        delNode(queryNode, parent);
    }

    setLength(queryNode, getSumLengthChildren(queryNode));
    return 0;
}

int detect_qvalue(Node *parent, const char *ptr) {
    Node *qvalueNode = newChild(parent);
    initNode(qvalueNode, "qvalue", ptr, 0);

    if (*ptr == '0') {
        initNode(newChild(qvalueNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(qvalueNode));

        if (*ptr == '.') {
            initNode(newChild(qvalueNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(qvalueNode));

            int comptage = 0;
            while (1) {
                if (detect_DIGIT(ptr) == 0) {
                    initNode(newChild(qvalueNode), "__digit", ptr, 1);
                    ptr += getLength(getLastChild(qvalueNode));
                } else {
                    break;
                }

                comptage++;
            }

            if (comptage > 3) {
                delNode(qvalueNode, parent);
                return 43;
            }
        }
    } else if (*ptr == '1') {
        initNode(newChild(qvalueNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(qvalueNode));

        if (*ptr == '.') {
            initNode(newChild(qvalueNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(qvalueNode));
            int comptage = 0;

            while (1) {
                if (*ptr == '0') {
                    initNode(newChild(qvalueNode), "case_insensitive_string", ptr, 1);
                    ptr += getLength(getLastChild(qvalueNode));
                } else {
                    break;
                }

                comptage++;
            }

            if (comptage > 3) {
                delNode(qvalueNode, parent);
                return 43;
            }
        }
    } else {
        delNode(qvalueNode, parent);
        return 43;
    }

    setLength(qvalueNode, getSumLengthChildren(qvalueNode));
    return 0;
}

int detect_RWS(Node *parent, const char *ptr) {
    Node *rwsNode = newChild(parent);
    initNode(rwsNode, "RWS", ptr, 0);

    int comptage = 0;
    while (1) {
        if (detect_SP(rwsNode, ptr) == 0 || detect_HTAB(rwsNode, ptr) == 0) {
            comptage += 1;
            ptr += getLength(getLastChild(rwsNode));
        } else {
            break;
        }
    }

    if (comptage == 0) {
        delNode(rwsNode, parent);
        return 44;
    }

    setLength(rwsNode, getSumLengthChildren(rwsNode));
    return 0;
}

int detect_segment(Node *parent, const char *ptr) {
    Node *segmentNode = newChild(parent);
    initNode(segmentNode, "segment", ptr, 0);
    int compteur = 0;

    while (1) {
        if (detect_pchar(segmentNode, ptr) == 0) {
            ptr += getLength(getLastChild(segmentNode));
            compteur++;
        } else {
            break;
        }
    }

    if (compteur == 0) {
        delNode(segmentNode, parent);
        return 56;
    }

    setLength(segmentNode, getSumLengthChildren(segmentNode));
    return 0;
}

int detect_tchar(Node *parent, const char *ptr) {
    Node *tcharNode = newChild(parent);
    initNode(tcharNode, "tchar", ptr, 0);

    if (isIn("!#$%&'*+-.^_`|~", *ptr)) {
        initNode(newChild(tcharNode), "case_insensitive_string", ptr, 1);
        setLength(tcharNode, 1);
    } else if (detect_DIGIT(ptr) == 0) {
        initNode(newChild(tcharNode), "__digit", ptr, 1);
        setLength(tcharNode, 1);
    } else if (detect_ALPHA(ptr) == 0) {
        initNode(newChild(tcharNode), "__alpha", ptr, 1);
        setLength(tcharNode, 1);
    } else {
        delNode(tcharNode, parent);
        return 45;
    }

    setLength(tcharNode, getSumLengthChildren(tcharNode));
    return 0;
}

int detect_transfer_coding(Node *parent, const char *ptr) {
    Node *transferCodingNode = newChild(parent);
    initNode(transferCodingNode, "transfer_coding", ptr, 0);

    if (startWith("chunked", ptr)) {
        initNode(newChild(transferCodingNode), "case_insensitive_string", ptr, 7);
        ptr += getLength(getLastChild(transferCodingNode));
    } else if (startWith("compress", ptr)) {
        initNode(newChild(transferCodingNode), "case_insensitive_string", ptr, 8);
        ptr += getLength(getLastChild(transferCodingNode));
    } else if (startWith("deflate", ptr)) {
        initNode(newChild(transferCodingNode), "case_insensitive_string", ptr, 7);
        ptr += getLength(getLastChild(transferCodingNode));
    } else if (startWith("gzip", ptr)) {
        initNode(newChild(transferCodingNode), "case_insensitive_string", ptr, 4);
        ptr += getLength(getLastChild(transferCodingNode));
    } else if (detect_transfer_extension(transferCodingNode, ptr) == 0) {
        ptr += getLength(getLastChild(transferCodingNode));
    } else {
        delNode(transferCodingNode, parent);
        return 46;
    }

    setLength(transferCodingNode, getSumLengthChildren(transferCodingNode));
    return 0;
}

int detect_transfer_extension(Node *parent, const char *ptr) {
    Node *transferExtensionNode = newChild(parent);
    initNode(transferExtensionNode, "transfer_extension", ptr, 0);

    if (detect_token(transferExtensionNode, ptr) == 0) {
        ptr += getLength(getLastChild(transferExtensionNode));
    } else {
        delNode(transferExtensionNode, parent);
        return 47;
    }

    while (1) {
        if (detect_OWS(transferExtensionNode, ptr) == 0 && *(ptr + getLength(getLastChild(transferExtensionNode))) == ';') {
            ptr += getLength(getLastChild(transferExtensionNode));
            initNode(newChild(transferExtensionNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(transferExtensionNode));

        } else if (*ptr == ';') {
            initNode(newChild(transferExtensionNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(transferExtensionNode));
        } else {
            if (startWith("OWS", getLabel(getLastChild(transferExtensionNode)))) {
                delNode(getLastChild(transferExtensionNode), transferExtensionNode);
            }
            break;
        }

        if (detect_OWS(transferExtensionNode, ptr) == 0) {
            ptr += getLength(getLastChild(transferExtensionNode));
        }

        if (detect_transfer_parameter(transferExtensionNode, ptr) == 0) {
            ptr += getLength(getLastChild(transferExtensionNode));
        } else {
            delNode(transferExtensionNode, parent);
            return 47;
        }
    }

    setLength(transferExtensionNode, getSumLengthChildren(transferExtensionNode));
    return 0;
}

int detect_transfer_parameter(Node *parent, const char *ptr) {
    Node *transferParameterNode = newChild(parent);
    initNode(transferParameterNode, "transfer_parameter", ptr, 0);

    if (detect_token(transferParameterNode, ptr) == 0) {
        ptr += getLength(getLastChild(transferParameterNode));
    } else {
        delNode(transferParameterNode, parent);
        return 48;
    }

    if (detect_BWS(transferParameterNode, ptr) == 0) {
        ptr += getLength(getLastChild(transferParameterNode));
    }

    if (*ptr == '=') {
        initNode(newChild(transferParameterNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(transferParameterNode));
    } else {
        delNode(transferParameterNode, parent);
        return 48;
    }

    if (detect_BWS(transferParameterNode, ptr) == 0) {
        ptr += getLength(getLastChild(transferParameterNode));
    }

    if (detect_token(transferParameterNode, ptr) == 0) {
        ptr += getLength(getLastChild(transferParameterNode));
    } else if (detect_quoted_string(transferParameterNode, ptr) == 0) {
        ptr += getLength(getLastChild(transferParameterNode));
    } else {
        delNode(transferParameterNode, parent);
        return 48;
    }

    setLength(transferParameterNode, getSumLengthChildren(transferParameterNode));
    return 0;
}

int detect_weight(Node *parent, const char *ptr) {
    Node *weightNode = newChild(parent);
    initNode(weightNode, "weight", ptr, 0);

    if (detect_OWS(weightNode, ptr) == 0) {
        ptr += getLength(getLastChild(weightNode));
    }

    if (*ptr == ';') {
        initNode(newChild(weightNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(weightNode));
    } else {
        delNode(weightNode, parent);
        return 49;
    }

    if (detect_OWS(weightNode, ptr) == 0) {
        ptr += getLength(getLastChild(weightNode));
    }

    if (startWith("q=", ptr)) {
        initNode(newChild(weightNode), "case_insensitive_string", ptr, 2);
        ptr += getLength(getLastChild(weightNode));
    } else {
        delNode(weightNode, parent);
        return 49;
    }

    if (detect_qvalue(weightNode, ptr) == 0) {
        ptr += getLength(getLastChild(weightNode));
    } else {
        delNode(weightNode, parent);
        return 49;
    }

    setLength(weightNode, getSumLengthChildren(weightNode));
    return 0;
}