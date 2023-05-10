#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/api.h"
#include "../include/constantes.h"
#include "../include/Mathis.h"
#include "../include/request.h"
#include "../include/utils.h"

int check_connection(Node *root, int clientId) {
    char *Connection = getHeaderValue(root, "Connection");

    if (!strcmp(Connection, "keep-alive")) {
        
        return 1;
    }

    free(Connection);

    return 0;
}


