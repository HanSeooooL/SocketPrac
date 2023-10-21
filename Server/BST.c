#include "Parking_server.h"

typedef Parkcar Element;

typedef struct _node {
    Element data;
    struct _node* left, *right;
} Node;

Node *root;

void init_root() {}