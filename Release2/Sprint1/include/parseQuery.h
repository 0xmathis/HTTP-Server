#define MAX_PAIRS 100

typedef struct {
    char name[100];
    char value[100];
    int count;
} Pair;

Pair *parseQuery(const char *);