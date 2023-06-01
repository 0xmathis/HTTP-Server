#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/parseQuery.h"

Pair *parseQuery(const char *input) {
    Pair *pairs = malloc(MAX_PAIRS * sizeof(Pair));
    if (pairs == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        exit(1);
    }

    char *token = strtok((char *) input, "&");
    pairs->count = 0;
    while (token != NULL && pairs->count < MAX_PAIRS) {
        char *equalSign = strchr(token, '=');
        if (equalSign != NULL) {
            *equalSign = '\0';
            strncpy(pairs[pairs->count].name, token, sizeof(pairs[pairs->count].name) - 1);
            strncpy(pairs[pairs->count].value, equalSign + 1, sizeof(pairs[pairs->count].value) - 1);
            pairs[pairs->count].name[sizeof(pairs[pairs->count].name) - 1] = '\0';
            pairs[pairs->count].value[sizeof(pairs[pairs->count].value) - 1] = '\0';
            pairs->count++;
        }
        token = strtok(NULL, "&");
    }

    return pairs;
}

//int main() {
//    const char input[] = "nom1=valeur1&nom2=valeur2&nom3=valeur3";
//
//    Pair* pairs = parseQuery(input);
//
//    for (int i = 0; i < pairs->count; i++) {
//        printf("Nom: %s, Valeur: %s\n", pairs[i].name, pairs[i].value);
//    }
//
//    free(pairs);
//
//    return 0;
//}
