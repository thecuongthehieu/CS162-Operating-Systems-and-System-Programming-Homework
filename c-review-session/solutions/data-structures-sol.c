#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct ListNode {
    char *value;
    struct ListNode *next;
};

typedef struct ListNode LinkNode;

LinkNode *create_node (char *value, struct LinkNode *next) {
    LinkNode *node = malloc(sizeof(LinkNode));
    node->value = malloc(sizeof(char) * (strlen(value) + 1));
    strcpy (node->value, value);
    node->next = next;
    return node;
}


void remove_nodes (LinkNode **node_addr, char *str) {
    while (*node_addr != NULL) {
        if (!strcmp((*node_addr)->value, str)) {
            LinkNode *to_free = *node_addr;
            *node_addr = to_free->next;
            free (to_free->value);
            free (to_free);
        } else {
            node_addr = &(*node_addr)->next;
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
