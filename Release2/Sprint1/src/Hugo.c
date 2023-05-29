#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/api.h"
#include "../include/constantes.h"
#include "../include/Josias.h"
#include "../include/Mathis.h"
#include "../include/request.h"
#include "../include/utils.h"

int chek_version(Node *root, int clientId) {
    char *version = getHeaderValue(root, "HTTP_version");

    int ret = 0;

    if(!strcmp(version, "HTTP/1.1")){
        ret = 1;
    }

    free(version);

    return ret;
}

int check_connection(Node *root, int clientId) {
    char *Connection = getHeaderValue(root, "Connection");

    if (!strcmp(Connection, "close")) {
        free(Connection);
        return 1;
    }

    free(Connection);
    
    return 0;
}

int check_Transfer_Encoding(Node *root, int clientId) {
    _Token *transfer_coding =  searchTree(root, "transfer_coding");
    
    while(transfer_coding != NULL){

        char *transfer_coding_value = getElementValue(transfer_coding->node, NULL);

        if(strcmp(transfer_coding_value, "chunked") && strcmp(transfer_coding_value, "compress") && strcmp(transfer_coding_value, "deflate") && strcmp(transfer_coding_value, "gzip") && strcmp(transfer_coding_value, "identity")){
            
            free(transfer_coding_value);
            free(transfer_coding);
            sendErrorCode(root, clientId, 501, "Unknown transfer coding");
            return 1;
        }

        transfer_coding = transfer_coding->next;
    }

    return 1;
}

int remove_dot_segments(char reduct_path[]) {

    printf("path : %s\n", reduct_path);

    for (int i = 0; i < (int) strlen(reduct_path) - 1; i++) {
        
        // Si on rencontre /./ on l'enlève 
        if (reduct_path[i] == '/' && reduct_path[i + 1] == '.' && (i + 2 >= strlen(reduct_path) || reduct_path[i + 2] == '/')) {
            
            for(int j = i; j < (int) strlen(reduct_path) - 1; j++){
                
                reduct_path[j] = reduct_path[j+2];
            }

            i--;
            //printf("%s\n", reduct_path);
        }
        
        // Si on rencontre /../ on l'enleve si il est pas au début on enlève le répertoire précédent 
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
            //printf("%s\n", reduct_path);
        }
    }

    //return reduct_path;
}

int main(){

    char path[] = "/.././.././../../../.././salut/azdef/../salut.txt";
    char reduct_path[strlen(path)];

    strcpy(reduct_path, path);

    remove_dot_segments(reduct_path);

    printf("reduct path : %s\n", reduct_path);

    return 0;
}


