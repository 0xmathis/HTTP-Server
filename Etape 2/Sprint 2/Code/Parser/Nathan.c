#include "Mathis.h"
#include "Nathan.h"
#include "Josias.h"
#include "Hugo.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

int detect_language_range(Node *parent, const char *ptr) {
    Node *languageNode = newChild(parent);
    initNode(languageNode, "language-range", ptr, 0);

    if (detect_ALPHA(languageNode, ptr) == 0) {
        ptr += getLength(getLastChild(languageNode));
        int comptage0 = 1;

        while (detect_ALPHA(languageNode, ptr) == 0) {
            comptage0 += 1;
            ptr += getLength(getLastChild(languageNode));
        }


        if (comptage0 <= 8) {
            while (*ptr == '-') {
                initNode(newChild(languageNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(languageNode));
                comptage0 = 0;
                while (detect_alphanum(languageNode, ptr) == 0) {
                    comptage0 += 1;
                    ptr += getLength(getLastChild(languageNode));
                }


                if (comptage0 == 0 || comptage0 > 8) {
                    return 31;
                }
            }
        } else {
            return 31;
        }
    } else if (*ptr == '*') {

        initNode(newChild(languageNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(languageNode));
    } else {
        return 31;
    }

    setLength(languageNode, getSumLengthChildren(languageNode));

    return 0;
}

int detect_ls32(Node *parent, const char *ptr) {
    Node *ls32Node = newChild(parent);
    initNode(ls32Node, "ls32", ptr, 0);

    if (detect_h16(ls32Node, ptr) == 0) {
        ptr += getLength(getLastChild(ls32Node));

        if (*ptr == ':') {
            initNode(newChild(ls32Node), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(ls32Node));

            if (detect_h16(ls32Node, ptr) == 0) {
                ptr += getLength(getLastChild(ls32Node));

            } else {

                return 32;
            }
        } else {
            return 32;
        }
    } else if (detect_IPv4address(ls32Node, ptr) == 0) {

        ptr += getLength(getLastChild(ls32Node));
    } else {

        return 32;
    }

    setLength(ls32Node, getSumLengthChildren(ls32Node));

    return 0;
}

int detect_OWS(Node *parent, const char *ptr) {
    Node *OWSNode = newChild(parent);
    initNode(OWSNode, "OWS", ptr, 0);

    while (1) {
        if (*ptr == ' ') {
            initNode(newChild(OWSNode), "__sp", ptr, 1);
            ptr += getLength(getLastChild(OWSNode));
        } else if (*ptr == '\t') {
            initNode(newChild(OWSNode), "__htab", ptr, 1);
            ptr += getLength(getLastChild(OWSNode));
        } else {
            break;
        }
    }

    setLength(OWSNode, getSumLengthChildren(OWSNode));

    return 0;
}

int detect_parameter(Node *parent, const char *ptr) {
    Node *parameterNode = newChild(parent);
    initNode(parameterNode, "parameter", ptr, 0);

    if (detect_token(parameterNode, ptr) == 0) {
        ptr += getLength(getLastChild(parameterNode));

        if (*ptr == '=') {
            initNode(newChild(parameterNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(parameterNode));

            if (detect_token(parameterNode, ptr) == 0) {
                ptr += getLength(getLastChild(parameterNode));
            } else if (detect_quoted_string(parameterNode, ptr) == 0) {

                ptr += getLength(getLastChild(parameterNode));
            } else {

                return 33;
            }
        } else {
            return 33;
        }
    } else {
        return 33;
    }

    setLength(parameterNode, getSumLengthChildren(parameterNode));

    return 0;
}

int detect_quoted_string(Node *parent, const char *ptr) {
    Node *quoted_stringNode = newChild(parent);
    initNode(quoted_stringNode, "quoted-string", ptr, 0);

    if (*ptr == '"') {
        initNode(newChild(quoted_stringNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(quoted_stringNode));

        while (detect_qdtext(quoted_stringNode, ptr) == 0 || detect_quoted_string(quoted_stringNode, ptr) == 0) {
            ptr += getLength(getLastChild(quoted_stringNode));
        }

        if (*ptr == '"') {
            initNode(newChild(quoted_stringNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(quoted_stringNode));
        } else {
            return 34;
        }
    } else {
        return 34;
    }

    setLength(quoted_stringNode, getSumLengthChildren(quoted_stringNode));

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
        return 35;
    }

    setLength(pcharNode, getSumLengthChildren(pcharNode));

    return 0;
}

int detect_unreserved(Node *parent, const char *ptr) {
    Node *unreservedNode = newChild(parent);
    initNode(unreservedNode, "unreserved", ptr, 0);

    if (detect_ALPHA(unreservedNode, ptr) == 0) {
        initNode(newChild(unreservedNode), "__alpha", ptr, 1);
        setLength(unreservedNode, 1);
    } else if (detect_DIGIT(unreservedNode, ptr) == 0) {
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

    return 0;
}

int detect_pct_encoded(Node *parent, const char *ptr) {
    Node *pct_encodedNode = newChild(parent);
    initNode(pct_encodedNode, "pct-encoded", ptr, 0);

    if (*ptr == '%') {
        initNode(newChild(pct_encodedNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(pct_encodedNode));

        if (detect_HEXDIG(pct_encodedNode, ptr) == 0) {
            ptr += getLength(getLastChild(pct_encodedNode));

            if (detect_HEXDIG(pct_encodedNode, ptr) == 0) {
                ptr += getLength(getLastChild(pct_encodedNode));
            } else {
                return 37;
            }
        } else {
            return 37;
        }
    } else {
        return 37;
    }

    setLength(pct_encodedNode, getSumLengthChildren(pct_encodedNode));

    return 0;
}

int detect_sub_delims(Node *parent, const char *ptr) {
    Node *sub_delimsNode = newChild(parent);
    initNode(sub_delimsNode, "sub-delims", ptr, 0);

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

        if (*ptr == '/') {
            initNode(newChild(productNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(productNode));

            if (detect_product_version(productNode, ptr) == 0) {
                ptr += getLength(getLastChild(productNode));
            } else {
                return 40;
            }
        }
    } else {
        return 40;
    }

    setLength(productNode, getSumLengthChildren(productNode));

    return 0;
}

int detect_product_version(Node *parent, const char *ptr) {
    Node *product_versionNode = newChild(parent);
    initNode(product_versionNode, "product-version", ptr, 0);

    if (detect_token(product_versionNode, ptr) == 0) {
        ptr += getLength(getLastChild(product_versionNode));
    } else {
        return 41;
    }

    setLength(product_versionNode, getSumLengthChildren(product_versionNode));

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
    } else if (detect_ALPHA(qdtextNode, ptr) == 0 || detect_DIGIT(qdtextNode, ptr) == 0) {
        ptr += getLength(getLastChild(qdtextNode));
    } else if (0x23 <= *ptr && *ptr <= 0x5B || 0x5D <= *ptr && *ptr <= 0x7E) {
        initNode(newChild(qdtextNode), "case_insensitive_string", ptr, 1);
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

    while (detect_pchar(queryNode, ptr) == 0 || *ptr == '/' || *ptr == '?') {

        if (detect_pchar(queryNode, ptr) == 0) {
            ptr += getLength(getLastChild(queryNode));
        } else {
            initNode(newChild(queryNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(queryNode));
        }
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

            int comptage1 = 0;
            while (detect_DIGIT(qvalueNode, ptr) == 0) {
                comptage1 += 1;
                ptr += getLength(getLastChild(qvalueNode));
            }

            if (comptage1 > 3) {
                return 43;
            }
        }
    } else if (*ptr == '1') {
        initNode(newChild(qvalueNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(qvalueNode));

        if (*ptr == '.') {
            initNode(newChild(qvalueNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(qvalueNode));

            int comptage2 = 0;
            while (*ptr == '0') {
                comptage2 += 1;
                initNode(newChild(qvalueNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(qvalueNode));
            }

            if (comptage2 > 3) {
                return 43;
            }
        }
    } else {
        return 43;
    }

    setLength(qvalueNode, getSumLengthChildren(qvalueNode));

    return 0;
}

int detect_RWS(Node *parent, const char *ptr) {
    Node *RWSNode = newChild(parent);
    initNode(RWSNode, "RWS", ptr, 0);

    int comptage3 = 0;
    while (detect_SP(RWSNode, ptr) == 0 || detect_HTAB(RWSNode, ptr) == 0) {
        comptage3 += 1;
        ptr += getLength(getLastChild(RWSNode));
    }

    if (comptage3 == 0) {
        return 44;
    }

    setLength(RWSNode, getSumLengthChildren(RWSNode));

    return 0;
}

int detect_segment(Node *parent, const char *ptr) {
    Node *segmentNode = newChild(parent);
    initNode(segmentNode, "segment", ptr, 0);

    while (detect_pchar(segmentNode, ptr) == 0) {
        ptr += getLength(getLastChild(segmentNode));
    }

    setLength(segmentNode, getSumLengthChildren(segmentNode));

    return 0;
}

int detect_tchar(Node *parent, const char *ptr) {
    Node *tcharNode = newChild(parent);
    initNode(tcharNode, "tchar", ptr, 0);

    if (strchr("!#$%&'*+-.^_`|~", *ptr) != NULL) {
        initNode(newChild(tcharNode), "case_insensitive_string", ptr, 1);
        setLength(tcharNode, 1);
    } else if (detect_DIGIT(tcharNode, ptr) == 0) {
        initNode(newChild(tcharNode), "__digit", ptr, 1);
        setLength(tcharNode, 1);
    } else if (detect_ALPHA(tcharNode, ptr) == 0) {
        initNode(newChild(tcharNode), "__alpha", ptr, 1);
        setLength(tcharNode, 1);
    } else {
        delNode(tcharNode, parent);
        return 45;
    }

    return 0;
}

int detect_transfert_coding(Node *parent, const char *ptr) {
    Node *transfert_codingNode = newChild(parent);
    initNode(transfert_codingNode, "transfert-coding", ptr, 0);

    if (startWith("chunked", ptr)) {
        initNode(newChild(transfert_codingNode), "case_insensitive_string", ptr, 7);
        ptr += getLength(getLastChild(transfert_codingNode));
    } else if (startWith("compress", ptr)) {
        initNode(newChild(transfert_codingNode), "case_insensitive_string", ptr, 8);
        ptr += getLength(getLastChild(transfert_codingNode));
    } else if (startWith("deflate", ptr)) {
        initNode(newChild(transfert_codingNode), "case_insensitive_string", ptr, 7);
        ptr += getLength(getLastChild(transfert_codingNode));
    } else if (startWith("gzip", ptr)) {
        initNode(newChild(transfert_codingNode), "case_insensitive_string", ptr, 4);
        ptr += getLength(getLastChild(transfert_codingNode));
    } else if (detect_transfert_extension(transfert_codingNode, ptr) == 0) {
        ptr += getLength(getLastChild(transfert_codingNode));
    } else {
        return 46;
    }

    setLength(transfert_codingNode, getSumLengthChildren(transfert_codingNode));

    return 0;
}

int detect_transfert_extension(Node *parent, const char *ptr) {
    Node *transfert_extensionNode = newChild(parent);
    initNode(transfert_extensionNode, "transfert-extension", ptr, 0);

    if (detect_token(transfert_extensionNode, ptr) == 0) {
        ptr += getLength(getLastChild(transfert_extensionNode));

        while (detect_OWS(transfert_extensionNode, ptr) == 0) {
            ptr += getLength(getLastChild(transfert_extensionNode));

            if (*ptr == ';') {
                initNode(newChild(transfert_extensionNode), "case_insensitive_string", ptr, 4);
                ptr += getLength(getLastChild(transfert_extensionNode));

                if (detect_OWS(transfert_extensionNode, ptr) == 0) {
                    ptr += getLength(getLastChild(transfert_extensionNode));

                    if (detect_transfert_parameter(transfert_extensionNode, ptr) == 0) {
                        ptr += getLength(getLastChild(transfert_extensionNode));
                    } else {
                        return 47;
                    }
                } else {
                    return 47;
                }
            } else {
                return 47;
            }
        }
    } else {
        return 47;
    }

    setLength(transfert_extensionNode, getSumLengthChildren(transfert_extensionNode));

    return 0;
}

int detect_transfert_parameter(Node *parent, const char *ptr) {
    Node *transfert_parameterNode = newChild(parent);
    initNode(transfert_parameterNode, "transfert-parameter", ptr, 0);

    if (detect_token(transfert_parameterNode, ptr) == 0) {
        ptr += getLength(getLastChild(transfert_parameterNode));

        if (detect_OWS(transfert_parameterNode, ptr) == 0) {
            ptr += getLength(getLastChild(transfert_parameterNode));

            if (*ptr == '=') {
                initNode(newChild(transfert_parameterNode), "case_insensitive_string", ptr, 4);
                ptr += getLength(getLastChild(transfert_parameterNode));

                if (detect_OWS(transfert_parameterNode, ptr) == 0) {
                    ptr += getLength(getLastChild(transfert_parameterNode));

                    if (detect_token(transfert_parameterNode, ptr) == 0) {
                        ptr += getLength(getLastChild(transfert_parameterNode));
                    } else if (detect_quoted_string(transfert_parameterNode, ptr) == 0) {
                        ptr += getLength(getLastChild(transfert_parameterNode));
                    } else {
                        return 48;
                    }
                } else {
                    return 48;
                }
            } else {
                return 48;
            }
        } else {
            return 48;
        }
    } else {
        return 48;
    }

    setLength(transfert_parameterNode, getSumLengthChildren(transfert_parameterNode));

    return 0;
}

int detect_weight(Node *parent, const char *ptr) {
    Node *weightNode = newChild(parent);
    initNode(weightNode, "weight", ptr, 0);

    if (detect_OWS(weightNode, ptr) == 0) {
        ptr += getLength(getLastChild(weightNode));

        if (*ptr == ';') {
            initNode(newChild(weightNode), "case_insensitive_string", ptr, 4);
            ptr += getLength(getLastChild(weightNode));

            if (detect_OWS(weightNode, ptr) == 0) {
                ptr += getLength(getLastChild(weightNode));

                if (startWith("q=", ptr)) {
                    initNode(newChild(weightNode), "case_insensitive_string", ptr, 2);
                    ptr += getLength(getLastChild(weightNode));

                    if (detect_qvalue(weightNode, ptr) == 0) {
                        ptr += getLength(getLastChild(weightNode));
                    } else {
                        return 49;
                    }
                } else {
                    return 49;
                }
            } else {
                return 49;
            }
        } else {
            return 49;
        }
    } else {
        return 49;
    }

    setLength(weightNode, getSumLengthChildren(weightNode));

    return 0;
}