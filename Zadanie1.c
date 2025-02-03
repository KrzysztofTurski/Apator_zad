#include <stdio.h>
#include <stdlib.h>

#define MAX_MASK 32
#define INVALID_MASK -1

typedef struct Node {
    struct Node *child[2];
    char mask;
} Node;

Node *root = NULL;

Node *create_node() {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node) {
        node->child[0] = node->child[1] = NULL;
        node->mask = INVALID_MASK;
    }
    return node;
}

int add(unsigned int base, char mask) {
    if (mask < 0 || mask > MAX_MASK) return -1;
    if (!root) root = create_node();

    Node *node = root;
    for (int i = 31; i >= (32 - mask); i--) {
        int bit = (base >> i) & 1;
        if (!node->child[bit])
            node->child[bit] = create_node();
        node = node->child[bit];
    }
    if (node->mask == INVALID_MASK)
        node->mask = mask;
    
    return 0;
}

int del(unsigned int base, char mask) {
    if (mask < 0 || mask > MAX_MASK || !root) return -1;
    
    Node *node = root;
    for (int i = 31; i >= (32 - mask); i--) {
        int bit = (base >> i) & 1;
        if (!node->child[bit]) return -1;
        node = node->child[bit];
    }
    
    node->mask = INVALID_MASK;
    return 0;
}

char check(unsigned int ip) {
    if (!root) return -1;
    
    Node *node = root;
    char match = -1;
    
    for (int i = 31; i >= 0; i--) {
        int bit = (ip >> i) & 1;
        if (!node->child[bit]) break;
        node = node->child[bit];
        if (node->mask != INVALID_MASK)
            match = node->mask;
    }
    
    return match;
}

void free_t(Node *node) {
    if (!node) return;
    free_t(node->child[0]);
    free_t(node->child[1]);
    free_t(node);
}

int main() {
    unsigned int ip1 = 0b00001010000101000000000000000000; // 10.20.0.0
    unsigned int ip2 = 0b00100000010000001000000000000000; // 32.64.128.0
    unsigned int test_ip1 = 0b00001010000101000000111100000001; // 10.20.15.1
    unsigned int test_ip2 = 0b00100000010000001000001000000101; // 32.64.130.5
    unsigned int test_ip3 = 0b11000000101010000000000100000001; // 192.168.1.1

    add(ip1, 16);
    add(ip2, 20);

    printf("Check 10.20.15.1: /%d\n", check(test_ip1));
    printf("Check 32.64.130.5: /%d\n", check(test_ip2));
    printf("Check 192.168.1.1: /%d\n", check(test_ip3));

    del(ip1, 16);
    printf("Check 10.20.15.1 after deletion: /%d\n", check(test_ip1));
    
    free(root);
    return 0;
}
