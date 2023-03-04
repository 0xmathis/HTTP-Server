#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "Node.h"





int parser(Node *parent_node, Node *current_node, char *i) {
        // Si le caractère est un chiffre
        if (isdigit(*i)){
            // Si on ne se trouve pas déjà dans un nombre
            if(current_node = NULL){
                n = newChild(parent_node);
        	    setStart(n, i);
        	    setLength(n, 1);
                current_node = n;
            }
            // Si on se trouve dans un Node
            else{
                //Si on était dans un mot
                if(!(strcmp(current_node.__label, "mot"))){
                    return(-1);
                }
                else{
                    current_node->__length += 1;
                }
            }
        }
            
        // Si le caractère est une ponctuation
        else if (strchr(",.:!?", *i) != NULL) {
            n = newChild(parent_node);
            setStart(n, i);
            setLength(n, 1);
            current_node = NULL;
        }

        // Si le caractère est un séparateur
        else if (strchr("\t -_", *i) != NULL) {
            // Si le séparateur termine un mot
            if(!(strcmp(current_node.__label, "mot"))){
                k = getLength(current_node);
                setLength(current_node, k+1);
            }
            else{
                n =newChild(parent_node);
                setStart(n, i);
                setLength(n, 1);

            }
            current_node = NULL;
        }

        // Si le caractère est une lettre
        else if (isalpha(*i)) {
            // Si on ne se trouve pas déja dans un mot
            if(current_node = NULL){
                n = newChild(parent_node);
        	    setStart(n, i);
        	    setLength(n, 1);
                current_node = n;
            }
            // Si on se trouve dans un Node
            else{
                //Si on était dans un mot
                if(!(strcmp(current_node.__label, "mot"))){
                    k = getLength(current_node);
                    setLength(current_node, k+1);
                }
                else{
                    return(-1);
                }
            }
            
    	}
        parser(parent_node,current_node,i++);
}



int detectStart(char *i){
	bool rv = False ;
	while(*i != "s"){
        (*i)++;
    }
    if(*i='s'){
        i ++;
        if(i = 't'){
            i++;
            if(i = 'a'){
                i++;
                if(i = 'r'){
                    i++;
                    if(i = 't'){
                        i++;
                        rv = True;
                    }
                }
            }
        }
    }        
	return(rv);
}

int detectFin(char *i){
	bool rv = False;
	while(*i != 'f'){
        (*i)++;
    }
    if(i='f'){
        i ++;
        if(i = 'i'){
            i++;
            if(i = 'n'){
                i ++;
                rv = True;
            }
        }
    }        
	return(rv);
}