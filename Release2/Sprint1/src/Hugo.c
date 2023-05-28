#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/api.h"
#include "../include/constantes.h"
#include "../include/Mathis.h"
#include "../include/request.h"
#include "../include/utils.h"

// int check_connection(Node *root, int clientId) {
//     char *Connection = getHeaderValue(root, "Connection");

//     if (!strcmp(Connection, "keep-alive")) {
        
//         return 1;
//     }

//     free(Connection);

//     return 0;
// }

int check_path(Node *root, int clientId) {
    char *path = getFilePath(root);
    char reduct_path[strlen(path)];

    strcpy(path,reduct_path)
    printf("path : %s\n", reduct_path);

    for (int i = 0; i < (int) strlen(reduct_path) - 1; i++) {
        
        if (reduct_path[i] == '/' && reduct_path[i + 1] == '.' && (i + 2 >= strlen(reduct_path) || reduct_path[i + 2] == '/')) {
            
            for(int j = i; j < (int) strlen(reduct_path) - 1; j++){
                
                reduct_path[j] = reduct_path[j+2];
            }

            printf("%s\n", reduct_path);
        }

        if (reduct_path[i] == '/' && reduct_path[i + 1] == '.' && reduct_path[i + 2] == '.' && (i + 3 >= strlen(reduct_path) || reduct_path[i + 3] == '/')) {
            
            int temp = i;

            if(i > 0){
                i --;
            }
            
            while(reduct_path[i] != '/'){
                i --;
            }
            
            for(int j = i; j < (int) strlen(reduct_path) - 1; j++){
                
                reduct_path[j] = reduct_path[j+3+(temp - i)];
                
            }

            i--;
            printf("%s\n", reduct_path);
        }
    }

    FILE *file = fopen(reduct_path, "rb");

    return 1;
}

int main(){
    char path[] = "/salut1/.././../../../salut/../../bonsoir/../.././";
    
    check_path(path);

    printf("%s\n", path);

    return 0;
}


