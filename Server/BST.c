#include "Parking_server.h"

typedef Parkcar Element;

typedef struct _node {
    Element data;
    struct _node* left, *right;
} Node;

Node *intimeroot;
<<<<<<< HEAD
Node *Carnumberroot;

void init_root() {}

Node* insertData_intime(Node* node, Element data) {
    if(node == NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode -> data = data;
        newNode -> left = NULL;
        newNode -> right = NULL;
        node = newNode;
        return newNode;
    }
    else {
        if (node -> data.intime > data.intime) {
            node -> left = Insert(node -> left, data);
        }
        else if(node -> data.intime < data.intime) {
            node -> right = Insert(node -> right, data);
        }
        else {
            //같은 데이터일 때 사용할 코드
        }
        return node;
    }
}

Node* serachData_intime(Node* node, Element data) {
    if (node == NULL) reutrn NULL;

    if(node -> data.intime == data.intime) {
        return node;
    }
    else {
        if(node -> data.intime > data.intime) {
            return Search(node -> left, data);
        }
        else {
            return search(node -> right, data);
        }
    }
}

Node* findMinData(Node* node, Node** minNode) {
    if(node -> left == NULL) {
        *minNode = node;
        node = node -> right;
        return node;
    }
    else {
        node -> left = findMinData(node -> left, minNode);
        return minNode;
    }
}

Node* deleteData(Node* node, int data) {
    if(node == NULL) return NULL;
    if(node -> data.intime == data) {
        Node* deleteNode = node;
        if(node -> left == NULL && node -> right == NULL) {
            node = NULL;
        }
        else if(node -> left != NULL && node -> right == NULL) {
            node = node -> left;
        }
        else if(node -> left == NULL && node -> right != NULL) {
            node = node -> right;
        }
        else {
            Node *minNode = NULL;
            node -> right = findMinData(node -> right, &minNode);
            minNode -> left = deleteNode -> left;
            minNode -> right = deleteNode -> right;
            node = minNode;
        }

        free(deleteNode);
        return node;
    }
}

void printinorderBST(Node* root) {
    if(root == NULL) return;

    printinorderBST(root -> left);
    printf("%d\n", root -> data.intime);
    printinorderBST(root -> right);
}
=======
Node *Carnumberroot;
>>>>>>> ebe7b9e ([update] BST파일 초기화)
