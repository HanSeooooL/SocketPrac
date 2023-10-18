#include "Parking_client.h"

typedef Parkcar Element;

typedef struct _node {
    Element data;
    struct _node* next;
} Node;

Node *root;