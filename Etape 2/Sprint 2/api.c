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

        _Token * liste;

        if(!strcmp(start->__label,name)){
            liste  = malloc(sizeof(_Token));
            liste->node = start;
            liste->next = NULL;

            if(start->__child!=NULL){
                liste->next = searchTree(start->__child, name);
            }

            if(start->__brother!=NULL){
                _Token *p = liste;

                while(p->next != NULL){
                    p = p->next;
                }

                p->next = searchTree(start->__brother, name);
            }

            if(start->__child==NULL && start->__brother==NULL){
                return NULL;
            }

        }
        else{
            if(start->__child!=NULL){
                searchTree(start->__child, name);
            }

            if(start->__brother!=NULL){
                searchTree(start->__brother, name);
            }

            if(start->__child==NULL && start->__brother==NULL){
                return NULL;
            }
        }

        return liste;

        
    }
    
}

// void searchTree_Recursif(void *start, char *name, _Token ** p){

//     if(!strcmp(start->__label,name)){
//         _Token * newElement = malloc(sizeof(_Token));

//         newElement->node = start;
//         newElement->next = NULL;
//         (*p)->next = newElement;
//         (*p) = (*p)->next;
//     }

//     if(start->__child!=NULL){
//         searchTree_Recursif(start->__child, name, p);
//     }

//     if(start->__brother!=NULL){
//         searchTree_Recursif(start->__brother, name, p);
//     }

// }