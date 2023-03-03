#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "Node.h"

#define taille 100


int main() {
    char str[taille];
    struct Node numbers[taille];
    struct Node punctuation[taille];
    struct Node separators[taille];
    struct Node words[taille];
    int i, j, n_num, n_punct, n_sep, n_word;

    // Entrée de la requête
    printf("Entrez une chaîne de caractères : ");
    scanf("%99[^\n]", str);
    str[strcspn(str, "\n")] = '\0';

    // Initialisation des compteurs
    n_num = n_punct = n_sep = n_word = 0;
    i = 0;

    // Parcours de chaque caractère de la chaîne de caractères
    while (str[i]) {
        // Si le caractère est un chiffre
        if (isdigit(str[i])) {
            // Parcours de la chaîne pour récupérer tout le mot
            j = 0;
            while (isdigit(str[i+j])) {
                j++;
            }
            struct Node n;
            n.start = &str[i];
            n.lenght = j;
            numbers[n_num++] = n;
            i += j;
        }
        // Si le caractère est une ponctuation
        else if (strchr(",.;:!?-", str[i]) != NULL) {
            struct Node n;
            n.start = &str[i];
            n.lenght = j;
            punctuation[n_punct++] = n;
            i++;
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
        else if (isalpha(str[i])) {
            // Parcours de la chaîne pour récupérer tout le mot
            j = 0;
            while (isalpha(str[i+j])) {
                j++;
            }
            struct Node n;
            n.start = &str[i];
            n.lenght = j;
            words[n_word++] = n;
            i += j;
        }
        else {
            i++;
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