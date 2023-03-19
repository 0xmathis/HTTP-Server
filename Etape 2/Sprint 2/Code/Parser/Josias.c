#include "utils.h"
#include <stdio.h>

detect_HTTP_message(const char *ptr){
    Node *HTTPMessageNode = newNode();
    initNode( , "HTTP_message", ptr, 0)
    if (detect_start_line(HTTPMessageNode, ptr)){
        ptr += getLenght(getLastChild(HTTPMessageNode));
        
        if(detect_header_field(HTTPMessageNode, ptr)){
            ptr += getLenght(getLastChild(HTTPMessageNode));
            
            if detect_CRLF(HTTPMessageNode, ptr){
                ptr += getLenght(getLastChild(HTTPMessageNode));

                if detect_CRLF(HTTPMessageNode, ptr){
                    ptr += getLenght(getLastChild(HTTPMessageNode));
                }
                else{
                    return 1
                }
            }
            else{
                return 1
            }
        }
        else{
            return 1
        }
    }
    else{
            return 1
    }
    if (detect_mesage_body(HTTPMessageNode, ptr)){
        ptr += getLenght(getLastChild(HTTPMessageNode));
    }
    
    setLength(HTTPMessageNode, getSumLengthChildren(HTTPMessageNode));
    return 0;
}

int detect_ALPHA(Node *parent, const char *ptr){
    if (isalpha(*ptr)){
        initNode(newChild(parent), "ALPHA", ptr, 1);
    }
    else{
        return 2;
    }
    return 0;
}

int detect_alphanum(Node *parent, const char *ptr){
    if (detect_ALPHA(parent, ptr) || detect_DIGIT(parent, ptr)){
        initNode(newChild(parent), "alphanum", ptr, 1);
    }
    else{
        return 3;
    }
    return 0;
}

int detect_DIGIT(Node *parent, const char *ptr){
    if (isdigit(*ptr)){
        initNode(newChild(parent), "DIGIT", ptr, 1);
    }
    else{
        return 4;
    }
    return 0;
}

int detect_comment(Node *parent, const char *ptr){
    Node *commentNode = newChild(parent);
    initNode(commentNode, "comment", ptr, 0);

    if (startWith("(", ptr)){
        initNode(newChild(commentNode), "case_insensitive_string", ptr, 1);
        ptr += getLength(getLastChild(commentNode));
        
        if (detect_ctext(commentNode, ptr)){
            ptr += getLenght(getLastChild(commentNode));
        }
        else if (detect_quoted_pair(commentNode, ptr)){
            ptr += getLenght(getLastChild(commentNode));
        }
        else if (detect_comment(commentNode, ptr)){
            ptr += getLenght(getLastChild(commentNode));
        }        

        if(*ptr == ")"){
            initNode(newChild(commentNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(commentNode));
        }

        else{
            return 5;
        }
    }
    else{
        return 5;
    }
    setLength(commentNode, getSumLengthChildren(commentNode));
    return 0;
}

int detect_ctext(Node *parent, const char *ptr){
    Node *ctextNode = newChild(parent);
    initNode(ctextNode, "ctext", ptr, 0);

    if(detect_HTAB(ctext, ptr)){
        ptr += getLength(getLastChild(ctextNode)); //remplacable par +=1
    }
    else if(detect_SP(ctext, ptr)){
        ptr += getLength(getLastChild(ctextNode));
    }
    else if(0x21 <= *ptr && *ptr <= 0x27){
        ptr += getLength(getLastChild(ctextNode));
    }
    else if(0x2A <= *ptr && *ptr <= 0x5B){
        ptr += getLength(getLastChild(ctextNode));
    }
    else if(0x5D <= *ptr && *ptr <= 0x7E){
        ptr += getLength(getLastChild(ctextNode));
    }
    else if(detect_obs_text(parent, ptr)){
        ptr += getLength(getLastChild(ctextNode));
    }
    else{
        return 6;
    }
    setLength(ctextNode, getSumLengthChildren(ctextNode));
    return 0;
}

int detect_quoted_pair(Node *parent, const char *ptr){
    Node *quotedPairNode = newChild(parent);
    initNode(quotedPairNode, "quoted-pair", ptr, 0);
    
    if (startWith("", ptr)){
        
    }
    
}
/*A finir*/


int detect_connection_option(Node *parent, const char *ptr){
    Node *connectionOptionNode = newChild(parent);
    initNode(connectionOptionNode, "connection-option", ptr, 0);

    if (detect_token(parent, ptr)){
        ptr += getLength(getLastChild(connectionOptionNode));
    }
    else{
        return 7;
    }
    setLength(connectionOptionNode, getSumLengthChildren(connectionOptionNode));
    return 0;
}

int detect_token(Node *parent, const char *ptr){
Node *tokenNode = newChild(parent);
    initNode(tokenNode, "token", ptr, 0);

    if (detect_tchar(tokenNode, ptr)){
        while (detect_tchar(tokenNode,ptr)){
            ptr += 1;
        }
    }
    else{
        return 8;
    }
    setLength(tokenNode, getSumLengthChildren(tokenNode));
    return 0;
}


int detect_codings(Node *parent, const char *ptr){
    Node *codingsNode = newChild(parent);
    initNode(codingsNode, "codingsNode", ptr, 0);

    if (detect_content_coding(codingsNode, ptr)){
        ptr += getLength(getLastChild(codingsNode));
    }
    else if(){

    }
    else{
        return 9;
    }
    setLength(codingsNode, getSumLengthChildren(codingsNode));
    return 0;
}
/*A finir*/

int detect_content_coding(Node *parent, const char *ptr){
    Node *contentCodingNode = newChild(parent);
    initNode(contentCodingNode, "content-coding", ptr, 0);

    if(detect_token(contentCodingNode, ptr)){
        ptr += getLength(getLastChild(contentCodingNode));
    }
    else{
        return 10;
    }
    setLength(contentCodingNode, getSumLengthChildren(contentCodingNode));
    return 0;
}

int detect_cookie_octet(Node *parent, const char *ptr){
    Node *cookieOctetNode = newChild(parent);
    initNode(cookieOctetNode, "cookieOctetNode", ptr, 0);

    if(){
        ptr +=1;
    }
    else if(){
        ptr +=1;
    }
    else if(){
        ptr +=1;
    }
    else if(){
        ptr +=1;
    }
    else if(){
        ptr +=1;
    }
}

int detect_cookie_pair(Node *parent, const char *ptr){
    Node *cookiePairNode = newChild(parent);
    initNode(cookiePairNode, "cookiePairNode", ptr, 0);

    if (detect_cookie_name(cookiePairNode, ptr)){
        ptr += getLength(getLastChild(cookiePairNode));

        if(ptr = "="){
            initNode(newChild(cookiePairNode), "case_insensitive_string", ptr, 1);
            ptr += getLength(getLastChild(cookiePairNode));

            if (detect_cookie_value(cookiePairNode, ptr)){
                ptr += getLength(getLastChild(cookiePairNode));
            }
            else{
                return 12;
            }
        }
        else {
            return 12;
        }        
    }
    else{
        return 12;
    }
    setLength(cookiePairNode, getSumLengthChildren(cookiePairNode));
    return 0;    
}

int detect_cookie_name(Node *parent, const char *ptr){
    
}

int detect_cookie_value(Node *parent, const char *ptr){
    
}

int detect_CRLF(Node *parent, const char *ptr){
    
}

int detect_HTAB(Node *parent, const char *ptr){
    
}

int detect_SP(Node *parent, const char *ptr){
    
}

int detect_obs_text(Node *parent, const char *ptr){
    
}

int detect_dec_octet(Node *parent, const char *ptr){
    
}

int detect_field_vchar(Node *parent, const char *ptr){
    
}

int detect_VCHAR(Node *parent, const char *ptr){
    
}

int detect_h16(Node *parent, const char *ptr){
    
}

int detect_HEXDIG(Node *parent, const char *ptr){
    
}

