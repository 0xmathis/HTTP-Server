#include "Mathis.h"
#include "Nathan.h"
#include "Josias.h"
#include "Hugo.h"
#include "utils.h"
#include <stdio.h>

// à la fin d'un nœud, on set sa longueur à la somme des longueurs de ses fils
// detect_OWS renvoie 0 même si on ne trouve rien puisque c'est optionnel, mais si ça trouve un OWS ça l'ajoute quand même à l'arbre ??
// on ne met pas detect_OWS dans un if ou un while puisque c'est optionnel et qu'on ne peut pas avoir plusieurs OWS à la suite par definition

/*
int detect_Transfer_Encoding_header(Node *parent, const char *ptr) { // pas demandé ?
    if (startWith("Transfert-Encoding", ptr)) {
        initNode(newChild(parent), "case_insensitive_string", ptr, 18);
        ptr += getLength(getLastChild(parent));

        if (*ptr == ':') {
            initNode(newChild(parent), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(parent));

            if (detect_OWS(parent, ptr) == 0) {
                ptr += getLength(getLastChild(parent));
            }

            if (detect_Transfert_Encoding(parent, ptr) == 0) {
                if (detect_OWS(parent, ptr) == 0) {
                    ptr += getLength(getLastChild(parent));
                }

            } else {
                return -1;
            }

        } else {
            return -1;
        }

    } else {
        return -1;
    }

    return 0;
}

int detect_Transfert_Encoding(Node *parent, const char *ptr) { // pas demandé ?
    while (*ptr == ',') {
        initNode(newChild(parent), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(parent));

        if (detect_OWS(parent, ptr) == 0) {
            ptr += getLength(getLastChild(parent));
        } else {
            return -1;
        }
    }

    return 0;
}
 */

int detect_Expect_header(Node *parent, const char *ptr) {
    Node *expectNode = newChild(parent);
    initNode(expectNode, "Expect_header", ptr, 0);

    if (startWith("Expect", ptr)) {
        initNode(newChild(expectNode), "case_insensitive_string", ptr, 6);
        ptr += getLength(getLastChild(expectNode));

        if (*ptr == ':') {
            initNode(newChild(expectNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(expectNode));

            if (detect_OWS(expectNode, ptr) == 0) {
                ptr += getLength(getLastChild(expectNode));
            }

            if (detect_Expect(expectNode, ptr) == 0) {

                ptr += getLength(getLastChild(expectNode));
                if (detect_OWS(expectNode, ptr) == 0) {
                    ptr += getLength(getLastChild(expectNode));
                }

            } else {
                delNode(expectNode, parent);
                return 91;
            }

        } else {
            delNode(expectNode, parent);
            return 91;
        }

    } else {
        delNode(expectNode, parent);
        return 91;
    }

    setLength(expectNode, getSumLengthChildren(expectNode));

    return 0;
}

int detect_Expect(Node *parent, const char *ptr) {
    Node *expectNode = newChild(parent);
    initNode(expectNode, "Expect", ptr, 0);

    if (startWith("100-continue", ptr)) {
        initNode(newChild(expectNode), "case_insensitive_string", ptr, 12);
        ptr += getLength(getLastChild(expectNode));
    } else {
        return 92;
    }

    setLength(expectNode, getSumLengthChildren(expectNode));

    return 0;
}

int detect_Host_header(Node *parent, const char *ptr) {
    Node *hostNode = newChild(parent);
    initNode(hostNode, "Host_header", ptr, 0);

    if (startWith("Host", ptr)) {
        initNode(newChild(hostNode), "case_insensitive_string", ptr, 4);
        ptr += getLength(getLastChild(hostNode));

        if (*ptr == ':') {
            initNode(newChild(hostNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(hostNode));

            if (detect_OWS(hostNode, ptr) == 0) {
                ptr += getLength(getLastChild(hostNode));
            }

            if (detect_Host(hostNode, ptr) == 0) {
                ptr += getLength(getLastChild(hostNode));

                if (detect_OWS(hostNode, ptr) == 0) {
                    ptr += getLength(getLastChild(hostNode));
                }

            } else {
                delNode(hostNode, parent);
                return 93;
            }

        } else {
            delNode(hostNode, parent);
            return 93;
        }

    } else {
        delNode(hostNode, parent);
        return 93;
    }

    setLength(hostNode, getSumLengthChildren(hostNode));

    return 0;
}

int detect_Host(Node *parent, const char *ptr) {
    Node *hostNode = newChild(parent);
    initNode(hostNode, "Host", ptr, 0);

    if (detect_uri_host(hostNode, ptr) == 0) {
        ptr += getLength(getLastChild(hostNode));
    } else {
        delNode(hostNode, parent);
        return 94;
    }

    if (*ptr == ':') {
        initNode(newChild(hostNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(hostNode));

        if (detect_port(hostNode, ptr) == 0) {
            ptr += getLength(getLastChild(hostNode));
        } else {
            delNode(hostNode, parent);
            return 94;
        }
    }

    setLength(hostNode, getSumLengthChildren(hostNode));

    return 0;
}

int detect_uri_host(Node *parent, const char *ptr) {
    Node *uriHostNode = newChild(parent);
    initNode(uriHostNode, "uri_host", ptr, 0);

    if (detect_host(uriHostNode, ptr) == 0) {
        ptr += getLength(getLastChild(uriHostNode));
    } else {
        delNode(uriHostNode, parent);
        return 95;
    }

    setLength(uriHostNode, getSumLengthChildren(uriHostNode));

    return 0;
}

int detect_host(Node *parent, const char *ptr) {
    Node *hostNode = newChild(parent);
    initNode(hostNode, "host", ptr, 0);

    if (detect_IP_literal(hostNode, ptr) == 0) {
        ptr += getLength(getLastChild(hostNode));
    } else if (detect_IPv4address(hostNode, ptr) == 0) {
        ptr += getLength(getLastChild(hostNode));
    } else if (detect_reg_name(hostNode, ptr) == 0) {
        ptr += getLength(getLastChild(hostNode));
    } else {
        delNode(hostNode, parent);
        return 96;
    }

    setLength(hostNode, getSumLengthChildren(hostNode));

    return 0;
}

int detect_IP_literal(Node *parent, const char *ptr) {
    Node *ipLiteralNode = newChild(parent);
    initNode(ipLiteralNode, "IP_literal", ptr, 0);

    if (*ptr == '[') {
        initNode(newChild(ipLiteralNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(ipLiteralNode));

        if (detect_IPv6address(ipLiteralNode, ptr) == 0) {
            ptr += getLength(getLastChild(ipLiteralNode));
        } else if (detect_IPvFuture(ipLiteralNode, ptr) == 0) {
            ptr += getLength(getLastChild(ipLiteralNode));
        } else {
            delNode(ipLiteralNode, parent);
            return 97;
        }

        if (*ptr == ']') {
            initNode(newChild(ipLiteralNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(ipLiteralNode));
        } else {
            delNode(ipLiteralNode, parent);
            return 97;
        }

    } else {
        delNode(ipLiteralNode, parent);
        return 97;
    }

    setLength(ipLiteralNode, getSumLengthChildren(ipLiteralNode));

    return 0;
}

int detect_IPv4address(Node *parent, const char *ptr) {
    Node *ipv4adressNode = newChild(parent);
    initNode(ipv4adressNode, "IPv4address", ptr, 0);

    for (int i = 0; i < 3; i++) {
        if (detect_dec_octet(ipv4adressNode, ptr) == 0) {
            ptr += getLength(getLastChild(ipv4adressNode));

            if (*ptr == '.') {
                initNode(newChild(ipv4adressNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(ipv4adressNode));
            } else {
                delNode(ipv4adressNode, parent);
                return 98;
            }

        } else {
            delNode(ipv4adressNode, parent);
            return 98;
        }
    }

    if (detect_dec_octet(ipv4adressNode, ptr) == 0) {
        ptr += getLength(getLastChild(ipv4adressNode));

    } else {
        delNode(ipv4adressNode, parent);
        return 98;
    }

    setLength(ipv4adressNode, getSumLengthChildren(ipv4adressNode));

    return 0;
}

int detect_IPv6address(Node *parent, const char *ptr) {
    Node *ipv6addressNode = newChild(parent);
    initNode(ipv6addressNode, "IPv6address", ptr, 0);

    int compteur = 0;

    while (1) {
        if (detect_h16(ipv6addressNode, ptr) == 0) {
            ptr += getLength(getLastChild(ipv6addressNode));
            compteur++;

            if (compteur == 7) {
                if (startWith("::", ptr)) {
                    initNode(newChild(ipv6addressNode), "case_insensitive_string", ptr, 2);
                    ptr += getLength(getLastChild(ipv6addressNode));
                    break;
                } else {
                    return 99;
                }
            } else if (startWith("::", ptr)) {
                break;
            } else if (*ptr == ':') {
                initNode(newChild(ipv6addressNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(ipv6addressNode));

                if (compteur == 6) {
                    if (detect_ls32(ipv6addressNode, ptr) == 0) {
                        ptr += getLength(getLastChild(ipv6addressNode));
                        break;
                    } else {
                        return 99;
                    }
                }
            } else {
                return 99;
            }
        } else {
            break;
        }
    }

    if (startWith("::", ptr)) {
        if (compteur == 6) {
            if (detect_h16(ipv6addressNode, ptr) == 0) {
                return 0;
            } else {
                return 99;
            }
        }
    }

    while (1) {
        if (detect_ls32(ipv6addressNode, ptr) == 0) {
            ptr += getLength(getLastChild(ipv6addressNode));

            if (compteur <= 5) {
                break;
            } else {
                return 99;
            }
        } else if (detect_h16(ipv6addressNode, ptr) == 0) {
            compteur++;
            ptr += getLength(getLastChild(ipv6addressNode));

            if (*ptr == ':') {
                initNode(newChild(ipv6addressNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(ipv6addressNode));
            } else {
                return 99;
            }
        }
    }

    setLength(ipv6addressNode, getSumLengthChildren(ipv6addressNode));

    return 0;
}

int detect_IPvFuture(Node *parent, const char *ptr) {
    Node *ipvFutureNode = newChild(parent);
    initNode(ipvFutureNode, "IPvFuture", ptr, 0);

    if (*ptr == 'v' || *ptr == 'V') {
        initNode(newChild(ipvFutureNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(ipvFutureNode));

        int i = 0;

        while (1) {
            if (detect_HEXDIG(ipvFutureNode, ptr) == 0) {
                ptr += getLength(getLastChild(ipvFutureNode));
            } else {
                break;
            }

            i++;
        }

        if (i < 1) {
            delNode(ipvFutureNode, parent);
            return 100;
        }

        if (*ptr == '.') {
            initNode(newChild(ipvFutureNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(ipvFutureNode));

            int j = 0;
            while (1) {
                if (detect_unreserved(ipvFutureNode, ptr) == 0) {
                    ptr += getLength(getLastChild(ipvFutureNode));
                } else if (detect_sub_delims(ipvFutureNode, ptr) == 0) {
                    ptr += getLength(getLastChild(ipvFutureNode));
                } else if (*ptr == ':') {
                    initNode(newChild(ipvFutureNode), "case_insensitive_string", ptr, 1);
                    ptr += getLength(getLastChild(ipvFutureNode));
                } else {
                    break;
                }

                j++;
            }

            if (j < 0) {
                delNode(ipvFutureNode, parent);
                return 100;
            }

        } else {
            delNode(ipvFutureNode, parent);
            return 100;
        }

    } else {
        delNode(ipvFutureNode, parent);
        return 100;
    }

    setLength(ipvFutureNode, getSumLengthChildren(ipvFutureNode));

    return 0;
}

int detect_reg_name(Node *parent, const char *ptr) {
    Node *regNameNode = newChild(parent);
    initNode(regNameNode, "reg_name", ptr, 0);

    while (1) {
        if (detect_unreserved(regNameNode, ptr) == 0) {
            ptr += getLength(getLastChild(regNameNode));
        } else if (detect_pct_encoded(regNameNode, ptr) == 0) {
            ptr += getLength(getLastChild(regNameNode));
        } else if (detect_sub_delims(regNameNode, ptr) == 0) {
            ptr += getLength(getLastChild(regNameNode));
        } else {
            break;
        }
    }

    setLength(regNameNode, getSumLengthChildren(regNameNode));

    return 0;
}

int detect_port(Node *parent, const char *ptr) {
    Node *portNode = newChild(parent);
    initNode(portNode, "port", ptr, 0);
    int compteur = 0;

    while (1) {
        if (detect_DIGIT(portNode, ptr) == 0) {
            initNode(newChild(portNode), "__digit", ptr, 1);
            ptr += getLength(getLastChild(portNode));
            compteur++;
        } else {
            break;
        }
    }

    if (compteur == 0) {
        delNode(portNode, parent);
    }

    setLength(portNode, getSumLengthChildren(portNode));

    return 0;
}

int detect_User_Agent_header(Node *parent, const char *ptr) {
    Node *userAgentNode = newChild(parent);
    initNode(userAgentNode, "User_Agent_header", ptr, 0);

    if (startWith("User-Agent", ptr)) {
        initNode(newChild(userAgentNode), "case_insensitive_string", ptr, 10);
        ptr += getLength(getLastChild(userAgentNode));

        if (*ptr == ':') {
            initNode(newChild(userAgentNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(userAgentNode));

            if (detect_OWS(userAgentNode, ptr) == 0) {
                ptr += getLength(getLastChild(userAgentNode));
            }

            if (detect_User_Agent(userAgentNode, ptr) == 0) {
                ptr += getLength(getLastChild(userAgentNode));
                if (detect_OWS(userAgentNode, ptr) == 0) {
                    ptr += getLength(getLastChild(userAgentNode));
                }
            } else {
                delNode(userAgentNode, parent);
                return 101;
            }
        } else {
            delNode(userAgentNode, parent);
            return 101;
        }
    } else {
        delNode(userAgentNode, parent);
        return 101;
    }

    setLength(userAgentNode, getSumLengthChildren(userAgentNode));

    return 0;
}

int detect_User_Agent(Node *parent, const char *ptr) {
    Node *userAgentNode = newChild(parent);
    initNode(userAgentNode, "User_Agent", ptr, 0);

    if (detect_product(userAgentNode, ptr) == 0) {
        ptr += getLength(getLastChild(userAgentNode));

        while (1) {
            if (detect_RWS(userAgentNode, ptr) == 0) {
                ptr += getLength(getLastChild(userAgentNode));

                if (detect_product(userAgentNode, ptr) == 0) {
                    ptr += getLength(getLastChild(userAgentNode));
                } else if (detect_comment(userAgentNode, ptr) == 0) {
                    ptr += getLength(getLastChild(userAgentNode));
                } else {
                    delNode(getLastChild(userAgentNode), userAgentNode);
                    break;
                }
            } else {
                break;
            }

        }
    } else {
        delNode(userAgentNode, parent);
        return 102;
    }

    setLength(userAgentNode, getSumLengthChildren(userAgentNode));

    return 0;
}

int detect_Accept_Language_header(Node *parent, const char *ptr) {
    Node *acceptLanguageNode = newChild(parent);
    initNode(acceptLanguageNode, "Accept_Language_header", ptr, 0);

    if (startWith("Accept-Language", ptr)) {
        initNode(newChild(acceptLanguageNode), "case_insensitive_string", ptr, 15);
        ptr += getLength(getLastChild(acceptLanguageNode));

        if (*ptr == ':') {
            initNode(newChild(acceptLanguageNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(acceptLanguageNode));

            if (detect_OWS(acceptLanguageNode, ptr) == 0) {
                ptr += getLength(getLastChild(acceptLanguageNode));
            }

            if (detect_Accept_Language(acceptLanguageNode, ptr) == 0) {
                ptr += getLength(getLastChild(acceptLanguageNode));
                if (detect_OWS(acceptLanguageNode, ptr) == 0) {
                    ptr += getLength(getLastChild(acceptLanguageNode));
                }
            } else {
                delNode(acceptLanguageNode, parent);
                return 103;
            }
        } else {
            delNode(acceptLanguageNode, parent);
            return 103;
        }
    } else {
        delNode(acceptLanguageNode, parent);
        return 103;
    }

    setLength(acceptLanguageNode, getSumLengthChildren(acceptLanguageNode));

    return 0;
}

int detect_Accept_Language(Node *parent, const char *ptr) {
    Node *acceptLanguageNode = newChild(parent);
    initNode(acceptLanguageNode, "Accept_Language", ptr, 0);

    while (*ptr == ',') {
        initNode(newChild(acceptLanguageNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(acceptLanguageNode));

        if (detect_OWS(acceptLanguageNode, ptr) == 0) {
            ptr += getLength(getLastChild(acceptLanguageNode));
        }
    }

    if (detect_language_range(acceptLanguageNode, ptr) == 0) {
        ptr += getLength(getLastChild(acceptLanguageNode));

        if (detect_weight(acceptLanguageNode, ptr) == 0) {
            ptr += getLength(getLastChild(acceptLanguageNode));
        }

        while (1) {
            if (detect_OWS(acceptLanguageNode, ptr) == 0 && *(ptr + getLength(getLastChild(acceptLanguageNode))) == ',') {
                ptr += getLength(getLastChild(acceptLanguageNode));
                initNode(newChild(acceptLanguageNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(acceptLanguageNode));

            } else if (*ptr == ',') {
                initNode(newChild(acceptLanguageNode), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(acceptLanguageNode));
            } else {
                if (startWith("OWS", getLabel(getLastChild(acceptLanguageNode)))) {
                    delNode(getLastChild(acceptLanguageNode), acceptLanguageNode);
                }
                break;
            }

            if (detect_OWS(acceptLanguageNode, ptr) == 0) {
                ptr += getLength(getLastChild(acceptLanguageNode));
            }

            if (detect_language_range(acceptLanguageNode, ptr) == 0) {
                ptr += getLength(getLastChild(acceptLanguageNode));

                if (detect_weight(acceptLanguageNode, ptr) == 0) {
                    ptr += getLength(getLastChild(acceptLanguageNode));
                }
            }
        }

    } else {
        delNode(acceptLanguageNode, parent);
        return 104;
    }

    setLength(acceptLanguageNode, getSumLengthChildren(acceptLanguageNode));

    return 0;
}

int detect_Accept_header(Node *parent, const char *ptr) {
    Node *acceptNode = newChild(parent);
    initNode(acceptNode, "Accept_header", ptr, 0);

    if (startWith("Accept", ptr)) {
        initNode(newChild(acceptNode), "case_insensitive_string", ptr, 6);
        ptr += getLength(getLastChild(acceptNode));

        if (*ptr == ':') {
            initNode(newChild(acceptNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(acceptNode));

            if (detect_OWS(acceptNode, ptr) == 0) {
                ptr += getLength(getLastChild(acceptNode));
            }

            if (detect_Accept(acceptNode, ptr) == 0) {
                ptr += getLength(getLastChild(acceptNode));

                if (detect_OWS(acceptNode, ptr) == 0) {
                    ptr += getLength(getLastChild(acceptNode));
                }

            } else {
                delNode(acceptNode, parent);
                return 105;
            }

        } else {
            delNode(acceptNode, parent);
            return 105;
        }

    } else {
        delNode(acceptNode, parent);
        return 105;
    }

    setLength(acceptNode, getSumLengthChildren(acceptNode));

    return 0;
}

int detect_Accept(Node *parent, const char *ptr) {
    Node *acceptNode = newChild(parent);
    initNode(acceptNode, "Accept", ptr, 0);

    if (*ptr == ',') {
        initNode(newChild(acceptNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(acceptNode));
    } else if (detect_media_range(acceptNode, ptr) == 0) {
        ptr += getLength(getLastChild(acceptNode));

        if (detect_accept_params(acceptNode, ptr) == 0) {
            ptr += getLength(getLastChild(acceptNode));
        }
    } else {
        delNode(acceptNode, parent);
        return 106;
    }

    while (1) {
        if (detect_OWS(acceptNode, ptr) == 0 && *(ptr + getLength(getLastChild(acceptNode))) == ',') {
            ptr += getLength(getLastChild(acceptNode));
            initNode(newChild(acceptNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(acceptNode));

        } else if (*ptr == ',') {
            initNode(newChild(acceptNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(acceptNode));
        } else {
            if (startWith("OWS", getLabel(getLastChild(acceptNode)))) {
                delNode(getLastChild(acceptNode), acceptNode);
            }
            break;
        }

        if (detect_OWS(acceptNode, ptr) == 0) {
            ptr += getLength(getLastChild(acceptNode));
        }

        if (detect_media_range(acceptNode, ptr) == 0) {
            ptr += getLength(getLastChild(acceptNode));

            if (detect_accept_params(acceptNode, ptr) == 0) {
                ptr += getLength(getLastChild(acceptNode));
            }
        }
    }

    setLength(acceptNode, getSumLengthChildren(acceptNode));

    return 0;
}

int detect_media_range(Node *parent, const char *ptr) {
    Node *mediaRangeNode = newChild(parent);
    initNode(mediaRangeNode, "media_range", ptr, 0);

    if (startWith("*/*", ptr)) {
        initNode(newChild(mediaRangeNode), "case_insensitive_string", ptr, 3);
        ptr += getLength(getLastChild(mediaRangeNode));

    } else if (detect_type(mediaRangeNode, ptr) == 0) {
        ptr += getLength(getLastChild(mediaRangeNode));

        if (*ptr == '/') {
            initNode(newChild(mediaRangeNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(mediaRangeNode));

            if (detect_subtype(mediaRangeNode, ptr) == 0) {
                ptr += getLength(getLastChild(mediaRangeNode));

            } else {
                delNode(mediaRangeNode, parent);
                return 107;
            }

        } else {
            delNode(mediaRangeNode, parent);
            return 107;
        }

    } else {
        delNode(mediaRangeNode, parent);
        return 107;
    }

    while (1) {
        if (detect_OWS(mediaRangeNode, ptr) == 0 && *(ptr + getLength(getLastChild(mediaRangeNode))) == ',') {
            ptr += getLength(getLastChild(mediaRangeNode));
            initNode(newChild(mediaRangeNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(mediaRangeNode));

        } else if (*ptr == ';') {
            initNode(newChild(mediaRangeNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(mediaRangeNode));

        } else {
            if (startWith("OWS", getLabel(getLastChild(mediaRangeNode)))) {
                delNode(getLastChild(mediaRangeNode), mediaRangeNode);
            }
            break;
        }

        if (detect_OWS(mediaRangeNode, ptr) == 0) {
            ptr += getLength(getLastChild(mediaRangeNode));
        }

        if (detect_parameter(mediaRangeNode, ptr) == 0) {
            ptr += getLength(getLastChild(mediaRangeNode));

        } else {
            delNode(mediaRangeNode, parent);
            return 107;
        }
    }

    setLength(mediaRangeNode, getSumLengthChildren(mediaRangeNode));

    return 0;
}

int detect_accept_params(Node *parent, const char *ptr) {
    Node *parameterNode = newChild(parent);
    initNode(parameterNode, "parameter", ptr, 0);

    if (detect_weight(parameterNode, ptr) == 0) {
        ptr += getLength(getLastChild(parameterNode));

        while (1) {
            if (detect_accept_ext(parameterNode, ptr) == 0) {
                ptr += getLength(getLastChild(parameterNode));
            } else {
                break;
            }
        }

    } else {
        delNode(parameterNode, parent);
        return 108;
    }

    setLength(parameterNode, getSumLengthChildren(parameterNode));

    return 0;
}

int detect_accept_ext(Node *parent, const char *ptr) {
    while (1) {
        if (detect_OWS(parent, ptr) == 0 && ptr[1] == ';') {
            ptr += getLength(getLastChild(parent));
            initNode(newChild(parent), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(parent));

        } else if (*ptr == ';') {
            initNode(newChild(parent), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(parent));

        } else {
            break;
        }

        if (detect_OWS(parent, ptr) == 0) {
            ptr += getLength(getLastChild(parent));
        }

        if (detect_token(parent, ptr) == 0) {
            ptr += getLength(getLastChild(parent));

            if (*ptr == '=') {
                initNode(newChild(parent), "case_insensitive_string", ptr, 1);
                ptr += getLength(getLastChild(parent));

                if (detect_token(parent, ptr) == 0) {
                    ptr += getLength(getLastChild(parent));
                } else if (detect_quoted_string(parent, ptr) == 0) {
                    ptr += getLength(getLastChild(parent));
                } else {
                    return 109;
                }

            } else {
                return 109;
            }

        } else {
            return 109;
        }
    }

    return 0;
}

int detect_Accept_Encoding_header(Node *parent, const char *ptr) {
    Node *acceptEncodingNode = newChild(parent);
    initNode(acceptEncodingNode, "Accept_Encoding_header", ptr, 0);

    if (startWith("Accept-Encoding", ptr)) {
        initNode(newChild(acceptEncodingNode), "case_insensitive_string", ptr, 15);
        ptr += getLength(getLastChild(acceptEncodingNode));

        if (*ptr == ':') {
            initNode(newChild(acceptEncodingNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(acceptEncodingNode));

            if (detect_OWS(acceptEncodingNode, ptr) == 0) {
                ptr += getLength(getLastChild(acceptEncodingNode));
            }

            if (detect_Accept_Encoding(acceptEncodingNode, ptr) == 0) {
                ptr += getLength(getLastChild(acceptEncodingNode));

                if (detect_OWS(acceptEncodingNode, ptr) == 0) {
                    ptr += getLength(getLastChild(acceptEncodingNode));
                }

            } else {
                delNode(acceptEncodingNode, parent);
                return 110;
            }

        } else {
            delNode(acceptEncodingNode, parent);
            return 110;
        }

    } else {
        delNode(acceptEncodingNode, parent);
        return 110;
    }

    setLength(acceptEncodingNode, getSumLengthChildren(acceptEncodingNode));

    return 0;
}

int detect_Accept_Encoding(Node *parent, const char *ptr) {
    Node *acceptEncodingNode = newChild(parent);
    initNode(acceptEncodingNode, "Accept_Encoding", ptr, 0);

    if (*ptr == ',') {
        initNode(newChild(acceptEncodingNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(acceptEncodingNode));
    } else if (detect_codings(acceptEncodingNode, ptr) == 0) {
        ptr += getLength(getLastChild(acceptEncodingNode));
        if (detect_weight(acceptEncodingNode, ptr) == 0) {
            ptr += getLength(getLastChild(acceptEncodingNode));
        }
    } else {
        delNode(acceptEncodingNode, parent);
        return 111;
    }

    while (1) {
        if (detect_OWS(acceptEncodingNode, ptr) == 0 && *(ptr + getLength(getLastChild(acceptEncodingNode))) == ',') {
            ptr += getLength(getLastChild(acceptEncodingNode));
            initNode(newChild(acceptEncodingNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(acceptEncodingNode));

        } else if (*ptr == ',') {
            initNode(newChild(acceptEncodingNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(acceptEncodingNode));
        } else {
            if (startWith("OWS", getLabel(getLastChild(acceptEncodingNode)))) {
                delNode(getLastChild(acceptEncodingNode), acceptEncodingNode);
            }
            break;
        }

        if (detect_OWS(acceptEncodingNode, ptr) == 0) {
            ptr += getLength(getLastChild(acceptEncodingNode));
        }

        if (detect_codings(acceptEncodingNode, ptr) == 0) {
            ptr += getLength(getLastChild(acceptEncodingNode));

            if (detect_weight(acceptEncodingNode, ptr) == 0) {
                ptr += getLength(getLastChild(acceptEncodingNode));
            }
        }
    }

    setLength(acceptEncodingNode, getSumLengthChildren(acceptEncodingNode));

    return 0;
}

int detect_message_body(Node *parent, const char *ptr) {
    Node *messageBodyNode = newChild(parent);
    initNode(messageBodyNode, "message_body", ptr, 0);
    int compteur = 0;

    while (1) {
        if (detect_OCTET(messageBodyNode, ptr) == 0) {
            ptr += getLength(getLastChild(messageBodyNode));
            compteur++;
        } else {
            break;
        }
    }

    if (compteur == 0) {
        delNode(messageBodyNode, parent);
    }

    setLength(messageBodyNode, getSumLengthChildren(messageBodyNode));

    return 0;
}

int detect_OCTET(Node *parent, const char *ptr) {
    if ((unsigned int) 0x01 <= (unsigned int) *ptr && (unsigned int) *ptr <= (unsigned int) 0xFF) {
        initNode(newChild(parent), "__octet", ptr, 1);
    } else {
        return 113;
    }

    return 0;
}

int detect_DQUOTE(Node *parent, const char *ptr) {
    if (*ptr == '"') {
        initNode(newChild(parent), "__dquote", ptr, 1);
    } else {
        return 114;
    }

    return 0;
}
