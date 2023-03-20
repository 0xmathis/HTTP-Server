#include "Nathan.h"
#include "utils.h"
#include <stdio.h>

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
    Node *ls32 = newChild(parent);
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

    while (*ptr == ' ' || *ptr == '\t') {
        initNode(newChild(OWSNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(OWSNode));
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

        while (detect_qdtext(quoted_stringNode, ptr) == 0 || detect_quotedstring(quoted_stringNode, ptr) == 0) {
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

    if (detect_unreserved(pcharNode, ptr) == 0){
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
        ptr += getLength(getLastChild(unreservedNode));
    } else if (detect_DIGIT(unreservedNode, ptr) == 0) {
        ptr += getLength(getLastChild(unreservedNode));
    } else if (*ptr == '-') {
        initNode(newChild(unreservedNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(unreservedNode));
    } else if (*ptr == '.') {
        initNode(newChild(unreservedNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(unreservedNode));
    } else if (*ptr == '_') {
        initNode(newChild(unreservedNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(unreservedNode));
    } else if (*ptr == '~') {
        initNode(newChild(unreservedNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(unreservedNode));
    } else {
        return 36;
    }

    setLength(unreservedNode, getSumLengthChildren(unreservedNode));

    return 0;
}

int detect_pct_encoded(Node *parent, const char *ptr) {
    Node *pct_encodedNode = newChild(parent);
    initNode(pct_encodedNode, "pct-encoded", ptr, 0);

    if (*ptr == '%') {
        initNode(newChild(pct_encodedNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(pct_encodedNode));

        if (detect_HEXDIGIT(pct_encodedNode, ptr) == 0) {
            ptr += getLength(getLastChild(pct_encodedNode));

            if (detect_HEXDIGIT(pct_encodedNode, ptr) == 0) {
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
    } else if (*ptr == '$')  {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '&')  {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == "'")  {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '(')  {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == ')')  {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '*')  {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '+')  {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == ',')  {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == ';')  {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else if (*ptr == '=')  {
        initNode(newChild(sub_delimsNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(sub_delimsNode));
    } else {
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
        return 40;
    }

    if (*ptr == '/') {
        initNode(newChild(productNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(productNode));

        if (detect_product_version(productNode, ptr) == 0) {
            ptr += getLength(getLastChild(productNode));
        } else {
            return 40;
        }
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
    } else if (0x23 <= *ptr && *ptr <=0x5B){
        initNode(newChild(qdtextNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(qdtextNode));
    } else if (0x5D <= *ptr && *ptr <=0x7E){
        initNode(newChild(qdtextNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(qdtextNode));
    } else if (detect_obs_text(qdtextNode, ptr) == 0) {
        ptr += getLength(getLastChild(qdtextNode));
    } else {
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

    setLength(qdtextNode, getSumLengthChildren(qdtextNode));

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
                return 43
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

    return 0