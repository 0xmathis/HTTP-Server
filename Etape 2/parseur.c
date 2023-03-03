#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "Node.h"



int main() {

    Request = *newNode();

    if(detectStart() = NULL){
    	printf("Error\n");
    }
    else{
    	int *i = detectStart() + 5;

        // Si le caractère est un chiffre
        if (isdigit(*i)) {
        	// Si on ne se trouve pas déjà dans un nombre
            if(){
            	n = newNode();
            	n.__start = i*;
            	int j = 0;
            }
            // Si on est déjà dans un nombre
            else{
            	while(! strchr(",.;:!?-", *i)){
                	j++;	
            	}
            }

        // Si le caractère est une ponctuation
        else if (strchr(",.;:!?-", *i) != NULL) {
            n = newNode();
            n.__start = i*;
            n.__length = 1;
        }

        // Si le caractère est un espace
        else if (isspace(str[i])) {
            struct Node n;
            n.start = &str[i];
            n.lenght = j;
            separators[n_sep++] = n;
            i++;
        }

        // Si le caractère est une lettre
        else if (isalpha(*i)) {
            // Si on ne se trouve pas déja dans un mot
            if(){
            	n = newNode();
            	n.__start = i*;
            	int j = 0;
            }
            // Si on est déjà dans un mot
            else{
            	while(! strchr(",.;:!?-", *i)){
                	j++;	
            	}
            	n.__length = j;
            }
    	}

    // Affichage des résultats
    printf("Nombres :\n");
    for (int i = 0; i < n_num; i++){
        for (int j = 0; j < numbers[i].len; j++){
            printf("(%c", *(numbers[i].pointeur + j));
        }
        printf("\n")
    }

    printf("Ponctuation :\n");
    for (int i = 0; i < n_punct; i++){
        for (int j = 0; j < punctuation[i].len; j++){
            printf("%c", *(punctuation[i].pointeur + j));
        }
        printf("\n");
    }


    printf("Séparateurs :\n");
    for (int i = 0; i < n_sep; i++) {
        for (int j = 0; j < separators[i].len; j++){
            printf("%c", *(separators[i].pointeur + j));
        }
        printf("\n");
    }

    printf("Mots :\n");
    for (int i = 0; i < n_word; i++) {
        for (int j = 0; j < words[i].len; j++){
        printf("%c", *(words[i].pointeur + j));
        }
        printf("\n");
    }
}



int detectStart(){
	char *pointeur;
	if (strstr( fi,"start") != NULL){
		*pointeur = strstr(( fi,"start"))
	}
	return(*pointeur);
}

int detectFin(){
	char *pointeur;
	if (strstr( fi,"fin") != NULL){
		*pointeur = strstr(( fi,"fin"))
	}
	return(*pointeur);
}