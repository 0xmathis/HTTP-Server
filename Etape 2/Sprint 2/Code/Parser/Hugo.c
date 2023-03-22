#include "Nathan.h"
#include "Josias.h"
#include "Mathis.h"
#include "Hugo.h"
#include "utils.h"
#include <stdio.h>

int detect_start_line(Node *parent, const char *ptr){
    Node *startLineNode = newChild(parent);
    initNode(startLineNode, "start_line", ptr, 0);

    if(detect_request_line() == 0){
        ptr += getLength(getLastChild(startLineNode));
    }
    else{
        return 61;
    }

    setLength(startLineNode, getSumLengthChildren(startLineNode));

    return 0;
}

int detect_request_line(Node *parent, const char *ptr){
    Node *requestLineNode = newChild(parent);
    initNode(requestLineNode, "request_line", ptr, 0);

    if(!detect_method(requestLineNode, ptr)){
        ptr += getLength(getLastChild(requestLineNode));
    }else{
        return 62;
    }

    if(!detect_SP(requestLineNode, ptr)){
        ptr += getLength(getLastChild(requestLineNode));
    }else{
        return 62;
    }

    if(!detect_request_target(requestLineNode, ptr)){
        ptr += getLength(getLastChild(requestLineNode));
    }else{
        return 62;
    }

    if(!detect_SP(requestLineNode, ptr)){
        ptr += getLength(getLastChild(requestLineNode));
    }else{
        return 62;
    }

    if(!detect_HTTP_version(requestLineNode, ptr)){
        ptr += getLength(getLastChild(requestLineNode));
    }else{
        return 62;
    }

    if(!detect_CRLF(requestLineNode, ptr)){
        ptr += getLength(getLastChild(requestLineNode));
    }else{
        return 62;
    }

    setLength(requestLineNode, getSumLengthChildren(requestLineNode));

    return 0;
}

int detect_method(Node *parent, const char *ptr){
    Node *methodNode = newChild(parent);
    initNode(methodNode, "method_line", ptr, 0);

    if(!detect_token(methodNode, ptr)){
        ptr += getLength(getLastChild(methodNode));
    }else{
        return 63
    }

    setLength(methodNode, getSumLengthChildren(methodNode));

    return 0;
}

int detect_request_target(Node *parent, const char *ptr){

    Node *requestTargetNode = newChild(parent);
    initNode(requestTargetNode, "request_target", ptr, 0);

    if(!detect_origin_form(requestTargetNode,ptr)){
        ptr += getLength(getLastChild(requestTargetNode));
    }else{
        return 64;
    }

    setLength(requestTargetNode, getSumLengthChildren(requestTargetNode));

    return 0;
}

int detect_origin_form(Node *parent, const char *ptr){
    Node *originFormNode = newChild(parent);
    initNode(originFormNode, "origin_form", ptr, 0);

    if(!detect_absolute_path(originFormNode,ptr)){
        ptr += getLength(getLastChild(originFormNode));
    }else{
        return 65;
    }

    setLength(originFormNode, getSumLengthChildren(originFormNode));

    return 0;
}

int detect_absolute_path(Node *parent, const char *ptr){
    Node *absolutePathNode = newChild(parent);
    initNode(absolutePathNode, "absolute_path", ptr, 0);

    if(*ptr == '/'){
        Node *SlashNode = newChild(absolutePathNode);
        initNode(SlashNode, "/", ptr, 1);
        ptr += 1;
    }else{
        return 66;
    }

    if(!detect_segment(absolutePathNode, ptr)){
        ptr += getLength(getLastChild(absolutePathNode));
    }else{
        return 66;
    }

    setLength(absolutePathNode, getSumLengthChildren(absolutePathNode)+1);

    return 0;
}

int detect_HTTP_version(Node *parent, const char *ptr){
    Node *HTTPVersionNode = newChild(parent);
    initNode(HTTPVersionNode, "HTTP_version", ptr, 0);

    if(!detect_HTTP_name(HTTPVersionNode,ptr)){
        ptr += getLength(getLastChild(HTTPVersionNode));
    }else{
        return 67;
    }

    if(*ptr == '/'){
        Node *SlashNode = newChild(HTTPVersionNode);
        initNode(SlashNode, "/", ptr, 1);
        ptr += 1;
    }else{
        return 67;
    }

    if(!detect_DIGIT(HTTPVersionNode,ptr)){
        ptr += getLength(getLastChild(HTTPVersionNode));
    }else{
        return 67;
    }

    if(*ptr == '.'){
        Node *DotNode = newChild(HTTPVersionNode);
        initNode(DotNode, ".", ptr, 1);
        ptr += 1;
    }else{
        return 67;
    }

    if(!detect_DIGIT(HTTPVersionNode,ptr)){
        ptr += getLength(getLastChild(HTTPVersionNode));
    }else{
        return 67;
    }

    setLength(HTTPVersionNode, getSumLengthChildren(HTTPVersionNode) +2);

    return 0;
}

int detect_HTTP_name(Node *parent, const char *ptr){
    Node *HTTPNameNode = newChild(parent);
    initNode(HTTPNameNode, "HTTP_name", ptr, 0);

    if(*ptr == 'H' && *ptr+1 == 'T' && *ptr+2 == 'T' & *ptr+3 == 'P'){
        Node *HTTPNode = newChild(HTTPNameNode);
        initNode(HTTPNode, "HTTP", ptr, 1);
        ptr += 4;
    }else{
        return 68;
    }
    
    setLength(HTTPNameNode, 4);

    return 0;
}

int detect_header_field(Node *parent, const char *ptr){
    Node *headerFieldNode = newChild(parent);
    initNode(headerFieldNode, "header_field", ptr, 0);

    while(1){

        if(!detect_field_name(headerFieldNode,ptr)){

            ptr += getLength(getLastChild(headerFieldNode));

            if(*ptr == ':'){
                Node *TDNode = newChild(headerFieldNode);
                initNode(TDNode, ":", ptr, 1);
                ptr += 1;
            }else{
                return 69;
            }

            if(!detect_OWS(headerFieldNode,ptr)){
                ptr += getLength(getLastChild(headerFieldNode));
            }else{
                return 69;
            }

            if(!detect_field_value(headerFieldNode,ptr)){
                ptr += getLength(getLastChild(headerFieldNode));
            }else{
                return 69;
            }

            if(!detect_OWS(headerFieldNode,ptr)){
                ptr += getLength(getLastChild(headerFieldNode));
            }else{
                return 69;
            }

        }else if(!detect_Connection_header(headerFieldNode,ptr)){
            ptr += getLength(getLastChild(headerFieldNode));

        }else if(!detect_Content_Length_header(headerFieldNode,ptr)){
            ptr += getLength(getLastChild(headerFieldNode));

        }else if(!detect_Content_Type_header(headerFieldNode,ptr)){
            ptr += getLength(getLastChild(headerFieldNode));

        }else if(!detect_Cookie_header(headerFieldNode,ptr)){
            ptr += getLength(getLastChild(headerFieldNode));

        }else if(!detect_Transfer_Encoding_header(headerFieldNode,ptr)){
            ptr += getLength(getLastChild(headerFieldNode));

        }else if(!detect_Expect_header(headerFieldNode,ptr)){
            ptr += getLength(getLastChild(headerFieldNode));

        }else if(!detect_Host_header(headerFieldNode,ptr)){
            ptr += getLength(getLastChild(headerFieldNode));

        }else if(!detect_User_Agent_header(headerFieldNode,ptr)){
            ptr += getLength(getLastChild(headerFieldNode));
            
        }else if(!detect_Accept_Language_header(headerFieldNode,ptr)){
            ptr += getLength(getLastChild(headerFieldNode));

        }else if(!detect_Accept_header(headerFieldNode,ptr)){
            ptr += getLength(getLastChild(headerFieldNode));

        }else if(!detect_Accept_Encoding_header(headerFieldNode,ptr)){
            ptr += getLength(getLastChild(headerFieldNode));

        }else if(!detect_CRLF(headerFieldNode, ptr)){
            break;
        }else{
            return 69;
        }
    }
    
    setLength(headerFieldNode, getSumLengthChildren(HTTPVersionNode));

    return 0;
}

int detect_field_name(Node *parent, const char *ptr){
    Node *fieldNameNode = newChild(parent);
    initNode(fieldNameNode, "field_name", ptr, 0);

    if(!detect_token(fieldNameNode,ptr)){
        ptr += getLength(getLastChild(fieldNameNode));
    }else{
        return 70;
    }
    
    setLength(fieldNameNode, getSumLengthChildren(fieldNameNode));

    return 0;
}

int detect_field_value(Node *parent, const char *ptr){
    Node *fieldValueNode = newChild(parent);
    initNode(fieldValueNode, "field_value", ptr, 0);

    while(1){
        if(!detect_field_content(fieldValueNode,ptr)){
            ptr += getLength(getLastChild(fieldValueNode));

        }else if(!detect_obs_fold(fieldValueNode,ptr)){
            ptr += getLength(getLastChild(fieldValueNode));
            
        }else if(!detect_OWS(fieldValueNode,ptr)){
            // supprimer le dernier fils qui vient d'être créer
            break;
            
        }else{
            return 71;
        }
    }
    
    setLength(fieldValueNode, getSumLengthChildren(fieldValueNode));

    return 0;
}

int detect_field_content(Node *parent, const char *ptr){
    Node *fieldContentNode = newChild(parent);
    initNode(fieldContentNode, "field_content", ptr, 0);

    if(!detect_field_vchar(fieldContentNode,ptr)){
        ptr += getLength(getLastChild(fieldContentNode));
    }else{
        return 72;
    }

    if(!detect_RWS(fieldContentNode,ptr)){
        ptr += getLength(getLastChild(fieldContentNode));

        if(!detect_field_vchar(fieldContentNode,ptr)){
            ptr += getLength(getLastChild(fieldContentNode));
        }else{
            return 72;
        }
    }
   
    setLength(fieldContentNode, getSumLengthChildren(fieldContentNode));

    return 0;
}

int detect_obs_fold(Node *parent, const char *ptr){
    Node *obsFoldNode = newChild(parent);
    initNode(obsFoldNode, "obs_fold", ptr, 0);

    if(!detect_CRLF(obsFoldNode,ptr)){
        ptr += getLength(getLastChild(obsFoldNode));
    }else{
        return 73;
    }

    if(!detect_RWS(fieldContentNode,ptr)){
        ptr += getLength(getLastChild(obsFoldNode));
    }else{
        return 73;
    }
   
    setLength(obsFoldNode, getSumLengthChildren(obsFoldNode));

    return 0;
}

int detect_Connection_header(Node *parent, const char *ptr){
    Node *ConnectionheaderNode = newChild(parent);
    initNode(ConnectionheaderNode, "Connection_header", ptr, 0);

    if(*ptr == 'C' && *ptr+1 == 'o' && *ptr+2 == 'n' & *ptr+3 == 'n' && *ptr +4== 'e' && *ptr+5 == 'c' && *ptr+6 == 't' & *ptr+7 == 'i'
        && *ptr+8 == 'o' && *ptr+9 == 'n' && *ptr+10 == ':'){

        Node *ConnNode = newChild(ConnectionheaderNode);
        initNode(ConnNode, "Connection", ptr, 10);
        ptr += 10
        Node *TDNode = newChild(ConnectionheaderNode);
        initNode(TDNode, ":", ptr, 1);
        ptr += 1;
    }else{
        return 74;
    }

    if(!detect_OWS(ConnectionheaderNode,ptr)){
        ptr += getLength(getLastChild(ConnectionheaderNode));
    }else{
        return 74;
    }

    if(!detect_Connection(ConnectionheaderNode,ptr)){
        ptr += getLength(getLastChild(ConnectionheaderNode));
    }else{
        return 74;
    }

    if(!detect_OWS(ConnectionheaderNode,ptr)){
        ptr += getLength(getLastChild(ConnectionheaderNode));
    }else{
        return 74;
    }
   
    setLength(ConnectionheaderNode, getSumLengthChildren(ConnectionheaderNode));

    return 0;
}

int detect_Connection(Node *parent, const char *ptr){
    Node *ConnectionNode = newChild(parent);
    initNode(ConnectionNode, "Connection", ptr, 0);

    while(1){
        if(*ptr == ','){
            Node *VirNode = newChild(ConnectionNode);
            initNode(VirNode, ",", ptr, 1);
            ptr += 1;

            if(!detect_OWS(ConnectionNode,ptr)){
                ptr += getLength(getLastChild(ConnectionNode));
            }else{
                return 75;
            }
        }else{
            break;
        }
    }

    if(!detect_connection_option(ConnectionNode,ptr)){
        ptr += getLength(getLastChild(ConnectionNode));
    }else{
        return 76;
    }

    while(1){    //problème ici comment savoir si c'est l'OWS du début du * ou de la fin du connection header
        if(!detect_OWS(ConnectionNode,ptr)){
            ptr += getLength(getLastChild(ConnectionNode)); 
        }

        if(*ptr == ','){
            Node *VirNode = newChild(ConnectionNode);
            initNode(VirNode, ",", ptr, 1);
            ptr += 1;
        }else{
            return 75;
        }

        if(!detect_OWS(ConnectionNode,ptr)){
            ptr += getLength(getLastChild(ConnectionNode)); 
        }

        if(!detect_connection_option(ConnectionNode,ptr)){
            ptr += getLength(getLastChild(ConnectionNode));
        }
    }
   
    setLength(ConnectionNode, getSumLengthChildren(ConnectionNode));

    return 0;
}

int detect_Content_Length_header(Node *parent, const char *ptr){
    Node *ContentLengthHeaderNode = newChild(parent);
    initNode(ContentLengthHeaderNode, "Content_Length_header", ptr, 0);

    if(*ptr == 'C' && *ptr+1 == 'o' && *ptr+2 == 'n' & *ptr+3 == 't' && *ptr +4== 'e' && *ptr+5 == 'n' && *ptr+6 == 't' & *ptr+7 == '-'
        && *ptr+8 == 'L' && *ptr+9 == 'e' && *ptr+10 == 'n' && *ptr+11 == 'g' && *ptr+12 == 't' && *ptr+13 == 'h' && *ptr+14 == ':'){

        Node *CLNode = newChild(ContentLengthHeaderNode);
        initNode(CLNode, "Content_Length", ptr, 14);
        ptr += 14;
        Node *DDNode = newChild(ContentLengthHeaderNode);
        initNode(DDNode, ":", ptr, 1);
        ptr += 1;
    }else{
        return 76;
    }

    if(!detect_OWS(ContentLengthHeaderNode,ptr)){
        ptr += getLength(getLastChild(ContentLengthHeaderNode));
    }else{
        return 76;
    }

    if(!detect_Content_Length(ContentLengthHeaderNode,ptr)){
        ptr += getLength(getLastChild(ContentLengthHeaderNode));
    }else{
        return 76;
    }

    if(!detect_OWS(ContentLengthHeaderNode,ptr)){
        ptr += getLength(getLastChild(ContentLengthHeaderNode));
    }else{
        return 76;
    }
   
    setLength(ContentLengthHeaderNode, getSumLengthChildren(ContentLengthHeaderNode));

    return 0;
}

int detect_Content_Length(Node *parent, const char *ptr){
    Node *ContentLengthNode = newChild(parent);
    initNode(ContentLengthNode, "Content_Length", ptr, 0);
    int i = 0;

    while(1){
        if(!detect_DIGIT(ContentLengthNode,ptr)){
            i ++;
            ptr += getLength(getLastChild(ContentLengthNode));
        }else{
            break;
        }
    }

    if(i < 1){
        return 77;
    }
    setLength(ContentLengthNode, getSumLengthChildren(ContentLengthNode));

    return 0;
}

int detect_Content_Type_header(Node *parent, const char *ptr){
    Node *ContentTypeHeaderNode = newChild(parent);
    initNode(ContentTypeHeaderNode, "Content_Type_header", ptr, 0);

    if(*ptr == 'C' && *ptr+1 == 'o' && *ptr+2 == 'n' & *ptr+3 == 't' && *ptr +4== 'e' && *ptr+5 == 'n' && *ptr+6 == 't' & *ptr+7 == '-'
        && *ptr+8 == 'T' && *ptr+9 == 'y' && *ptr+10 == 'p' && *ptr+11 == 'e' && *ptr+12 == ':'){

        Node *CLNode = newChild(ContentTypeHeaderNode);
        initNode(CLNode, "Content_Type", ptr, 12);
        ptr += 12;
        Node *DDNode = newChild(ContentLengthHeaderNode);
        initNode(DDNode, ":", ptr, 1);
        ptr += 1;
    }else{
        return 77;
    }

    if(!detect_OWS(ContentTypeHeaderNode,ptr)){
        ptr += getLength(getLastChild(ContentTypeHeaderNode));
    }else{
        return 77;
    }

    if(!detect_Content_Type(ContentTypeHeaderNode,ptr)){
        ptr += getLength(getLastChild(ContentTypeHeaderNode));
    }else{
        return 77;
    }

    if(!detect_OWS(ContentTypeHeaderNode,ptr)){
        ptr += getLength(getLastChild(ContentTypeHeaderNode));
    }else{
        return 77;
    }

    setLength(ContentTypeHeaderNode, getSumLengthChildren(ContentTypeHeaderNode));

    return 0;
}

int detect_Content_Type(Node *parent, const char *ptr){
    Node *ContentTypeNode = newChild(parent);
    initNode(ContentTypeNode, "Content_Type", ptr, 0);
    
    if(!detect_media_type(ContentTypeNode,ptr)){
        ptr += getLength(getLastChild(ContentTypeNode));
    }else{
        return 78;
    }

    setLength(ContentTypeNode, getSumLengthChildren(ContentTypeNode));

    return 0;
}

int detect_media_type(Node *parent, const char *ptr){
    Node *MediaTypeNode = newChild(parent);
    initNode(MediaTypeNode, "media_type", ptr, 0);
    
    if(!detect_type(MediaTypeNode,ptr)){
        ptr += getLength(getLastChild(MediaTypeNode));
    }else{
        return 79;
    }

    if(*ptr == '/'){
        Node *SlashNode = newChild(MediaTypeNode);
        initNode(SlashNode, "/", ptr, 1);
        ptr += 1;
    }else{
        return 79;
    }

    if(!detect_subtype(MediaTypeNode,ptr)){
        ptr += getLength(getLastChild(MediaTypeNode));
    }else{
        return 79;
    }

    while(1){ //pareil ici comment sortir de cette boucle
        if(!detect_OWS(MediaTypeNode,ptr)){
            ptr += getLength(getLastChild(MediaTypeNode));
        }else{
            return 79;
        }

        if(*ptr == ';'){
            Node *PVNode = newChild(MediaTypeNode);
            initNode(PVNode, ";", ptr, 1);
            ptr += 1;
        }else{
            return 79;
        }

        if(!detect_OWS(MediaTypeNode,ptr)){
            ptr += getLength(getLastChild(MediaTypeNode));
        }else{
            return 79;
        }

        if(!detect_parameter(MediaTypeNode,ptr)){
            ptr += getLength(getLastChild(MediaTypeNode));
        }else{
            return 79;
        }

    }

    setLength(MediaTypeNode, getSumLengthChildren(MediaTypeNode));

    return 0;
}

int detect_type(Node *parent, const char *ptr){
    Node *TypeNode = newChild(parent);
    initNode(TypeNode, "type", ptr, 0);
    
    if(!detect_token(TypeNode,ptr)){
        ptr += getLength(getLastChild(TypeNode));
    }else{
        return 80;
    }

    setLength(TypeNode, getSumLengthChildren(TypeNode));

    return 0;
}

int detect_subtype(Node *parent, const char *ptr){
    Node *SubTypeNode = newChild(parent);
    initNode(SubTypeNode, "subtype", ptr, 0);
    
    if(!detect_token(SubTypeNode,ptr)){
        ptr += getLength(getLastChild(SubTypeNode));
    }else{
        return 81;
    }

    setLength(SubTypeNode, getSumLengthChildren(SubTypeNode));

    return 0;
}

int  detect_Cookie_header(Node *parent, const char *ptr){
    Node *CookieHeaderNode = newChild(parent);
    initNode(CookieHeaderNode, "Cookie_header", ptr, 0);
    
    if(*ptr == 'C' && *ptr+1 == 'o' && *ptr+2 == 'o' & *ptr +3== 'k' && *ptr+4 == 'i' && *ptr+5 == 'e' & *ptr+6 == ':'){
        Node *CooNode = newChild(CookieHeaderNode);
        initNode(CLNode, "Cookie", ptr, 6);
        ptr += 6;
        Node *DDNode = newChild(CookieHeaderNode);
        initNode(DDNode, ":", ptr, 1);
        ptr += 1;
    }else{
        return 82;
    }

    if(!detect_Cookie_string(CookieHeaderNode,ptr)){
        ptr += getLength(getLastChild(CookieHeaderNode));
    }else{
        return 82;
    }

    setLength(CookieHeaderNode, getSumLengthChildren(CookieHeaderNode));

    return 0;
}


int detect_cookie_string(Node *parent, const char *ptr){
    Node *CookieStringNode = newChild(parent);
    initNode(CookieStringNode, "cookie_string", ptr, 0);
    
    if(!detect_cookie_pair(CookieStringNode,ptr)){
        ptr += getLength(getLastChild(CookieStringNode));
    }else{
        return 83;
    }

    while(1){
        if(*ptr == ';'){
            Node *PVNode = newChild(CookieStringNode);
            initNode(PVNode, ";", ptr, 1);
            ptr += 1;
        }else{
            break;
        }

        if(!detect_SP(CookieStringNode,ptr)){
            ptr += getLength(getLastChild(CookieStringNode));
        }else{
            return 83;
        }

        if(!detect_cookie_pair(CookieStringNode,ptr)){
            ptr += getLength(getLastChild(CookieStringNode));
        }else{
            return 83;
        }
    }

    setLength(CookieStringNode, getSumLengthChildren(CookieStringNode));

    return 0;
}
