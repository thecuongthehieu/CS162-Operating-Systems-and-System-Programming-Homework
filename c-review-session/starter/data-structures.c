#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct ListNode {
    char *value;
    struct ListNode *next;
};

typedef struct ListNode LinkNode;

LinkNode *create_node (char *value, LinkNode *next) {
    LinkNode *node = malloc(sizeof(LinkNode));
    node->value = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy (node->value, value);
    node->next = next;
    return node;
}


void remove_nodes (LinkNode **node_addr, char *str) {
    while (*node_addr != NULL) { // Iterate through the list
        if (!strcmp((*node_addr)->value, str)) { // 0 is a match
            // YOUR CODE HERE
        } else {
            // YOUR CODE HERE
        }
    }
}

void print_lst (LinkNode *node) {
    while (node != NULL) {
        printf ("%s\n", node->value);
        node = node->next;
    }
}


int main () {
    char* strlst[] = {"hello", "I", "like", "operating", "systems"};
    LinkNode *node = NULL;
    for (int i = 4; i >= 0; i--) {
        node = create_node (strlst[i], node);
    }
    print_lst (node);
    remove_nodes (&node, "hello");
    print_lst (node);
}
