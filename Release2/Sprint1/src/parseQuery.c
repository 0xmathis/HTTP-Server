#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char **parseQuery(char *query) {
	char **res = (char **) malloc(sizeof(char*) * strlen(query));
	int i = 0;
	int j;
	int k = 0;
	char *name = (char *) malloc(20 * sizeof(char));
	char *value = (char *) malloc(20 * sizeof(char));

	while (query[i] != 0) {
		j = 0;
		while (query[i] != '=') {
			name[j] = query[i];
			j++;
			i++;
		}
		name[j] = 0;
		j = 0;
		while (query[i] != '&' || query[i] != 0) {
			value[j] = query[i];
			i++;
			j++;
		}
		value[j] = 0;
		strcpy(res[k],name);
		strcpy(res[k+1],value);
		k+=2;
	}

	return res;
}

int main(int argc, char *argv){
	char **res = parseQuery("bolos=Josias&Monstre=Nathan");
	int i = 0;
	while (res[i]!=NULL) {
		printf("%s\n",res[i]);
		i++;
	}
	return 0;
}
