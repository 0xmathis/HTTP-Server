#include "api.h"
#include "/Code/Node.h"
#include <string.h>

char *getElementTag(void *node,int *len){
    noeud = (Node*) node; 

    if(len != NULL){
        *len = strlen(noeud->__label); 
    }

    return noeud->__label;
}

char *getElementValue(void *node,int *len){
    noeud = (Node*) node; 

    if(len != NULL){
        *len = noeud->__length; 
    }

    return noeud->__start;
}

void purgeTree(void *root){
    racine = (Node*) root;

    if(racine->__child!=NULL){
        Supprimer_arbre(racine->__child);
    }
    if(racine->__brother!=NULL){

        Supprimer_arbre(racine->__brother);
    }

    free(racine);
}

void purgeElement(_Token **r){

    if((*r)->next != NULL){
        purgeElement(&((*r)->next));
    }

    free(*r);
}

_Token *searchTree(void *start,char *name){

    if(start == NULL){
        searchTree(getRootTree(), name);
    }
    else{

        _Token * liste = NULL;
        
        searchTree_Recursif(start, name, &liste);
        
        return Element;
    }
    
}

void searchTree_Recursif(void *start, char *name, _Token ** liste){

    if(!strcmp(start->__label,name)){
        _Token * newElement = malloc(sizeof(_Token));

        newElement->node = start;
        newElement->next = NULL;
        (*liste)->next = newElement;
    }

    if(racine->__child!=NULL){
        searchTree_Recursif(start->__child, name, liste);
    }

    if(racine->__brother!=NULL){
        searchTree_Recursif(start->__brother, name, liste);
    }

}