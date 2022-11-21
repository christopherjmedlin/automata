/**
 * Simple hash map with chaining for the rule map
 */

#define P 1234
#define M 5293

#include <math.h>

typedef struct Node {
    int* key;
    int val;
    Node* next;
} Node;

typedef Node** HashMap;

HashMap hm_init(int size) {
    HashMap hm = (Node**) malloc(size * sizeof(Node*));
    for (int i = 0; i < size; i++) {
        hm[i] = 0;
    } 
    return hm;
}

// keep this simple for now
int hash(int* neighborhood, int size) {
    int h = 0;
    for (int i = 0; i < size; i++) {
        h += neighborhood[i] * pow(P, i);
    }
    return h % M;
}

int hm_lookup(HashMap m, int size, int* key, int keySize, int val) {
    int i = hash(key, keySize) % size;
    Node* n = m[i];
    while (n != 0) {
        for (int i = 0; i < keySize; i++) {
            if (n->key[i] != key[i]) break;
            if (i == keySize-1) return n->val;
        }
        n = n->next;
    }
    return -1;
}

void hm_insert(HashMap m, int size, int* key, int keySize, int val) {
    int i = hash(key, keySize) % size;
    Node* n = (Node*) malloc(sizeof(Node));
    n->key = key;
    n->val = val;
    n->next = 0;
    if (m[i] == 0) {
        m[i] = n;
        return;
    }
    Node* c = m[i];
    while (c->next != 0) {
        c = c->next; 
    }
    c->next = n;
}