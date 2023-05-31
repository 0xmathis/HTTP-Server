//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
//
//char **parseQuery(char *query) {
//	char **res = (char **) malloc(sizeof(char*) * strlen(query));
//	int i = 0;
//	int j;
//	int k = 0;
//	char *name = (char *) malloc(20 * sizeof(char));
//	char *value = (char *) malloc(20 * sizeof(char));
//
//	while (query[i] != 0) {
//		j = 0;
//		while (query[i] != '=') {
//			name[j] = query[i];
//			j++;
//			i++;
//		}
//		name[j] = 0;
//		j = 0;
//		while (query[i] != '&' || query[i] != 0) {
//			value[j] = query[i];
//			i++;
//			j++;
//		}
//		value[j] = 0;
//		strcpy(res[k],name);
//		strcpy(res[k+1],value);
//		k+=2;
//	}
//
//	return res;
//}
//
//int main(int argc, char *argv){
//	char **res = parseQuery("bolos=Josias&Monstre=Nathan");
//	int i = 0;
//	while (res[i]!=NULL) {
//		printf("%s\n",res[i]);
//		i++;
//	}
//	return 0;
//}



#include <stdio.h>
#include <string.h>

#define MAX_PAIRS 100

int parseQuery() {
        char imput[] = "Josias=bolos&Nathan=monstre&Mathis=bg";
        char pairs[MAX_PAIRS][100];
        int count = 0;

        char *pair = strtok(imput, "&=");
        while (pair != NULL && count < MAX_PAIRS) {
                strcpy(pairs[count], pair);
                count++;
                pair = strtok(NULL, "&=");
        }

        for (int i=0;i<count;i++) {
                printf("%s\n",pairs[i]);
	}

        return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PAIRS 100

typedef struct {
    char name[100];
    char value[100];
} Pair;

Pair* extractPairs(const char* input, int* count) {
    Pair* pairs = malloc(MAX_PAIRS * sizeof(Pair));
    if (pairs == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        exit(1);
    }

    char* token = strtok((char*)input, "&");
    int i = 0;
    while (token != NULL && i < MAX_PAIRS) {
        char* equalSign = strchr(token, '=');
        if (equalSign != NULL) {
            *equalSign = '\0';
            strncpy(pairs[i].name, token, sizeof(pairs[i].name) - 1);
            strncpy(pairs[i].value, equalSign + 1, sizeof(pairs[i].value) - 1);
            pairs[i].name[sizeof(pairs[i].name) - 1] = '\0';
            pairs[i].value[sizeof(pairs[i].value) - 1] = '\0';
            i++;
        }
        token = strtok(NULL, "&");
    }

    *count = i;
    return pairs;
}
///////////////////////
int main() {
    const char input[] = "nom1=valeur1&nom2=valeur2&nom3=valeur3";
    int count = 0;

    Pair* pairs = extractPairs(input, &count);

    // Affichage des paires extraites
    for (int i = 0; i < count; i++) {
        printf("Nom: %s, Valeur: %s\n", pairs[i].name, pairs[i].value);
    }

    // Libération de la mémoire allouée pour le tableau pairs
    free(pairs);

    return 0;
}
