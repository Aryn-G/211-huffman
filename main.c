#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHA 26
#define MAX_INPUT 100

typedef struct Node {
    int weight;
    char symbol;                // 'a'..'z' for leaves, 0 for internal nodes
    struct Node *left, *right;  // NULL for leaves
} Node;

Node* new_node(int weight, char symbol, Node* left, Node* right);
void free_tree(Node* n);

int count_freqs(const char* s, int freq[ALPHA]);

typedef struct {
    Node* items[ALPHA];
    int head, tail;
} Queue;

int q_size(const Queue* q);
void q_push(Queue* q, Node* n);
Node* q_front(const Queue* q);
Node* q_pop(Queue* q);
Node* pop_min(Queue* q1, Queue* q2);

int cmp_weight(const void* a, const void* b);

Node* build_tree(const int freq[ALPHA]);
void build_codes(const Node* n, char* buf, int depth, char codes[ALPHA][ALPHA]);

char* encode(const char* s, char codes[ALPHA][ALPHA]);
char* decode(const char* bits, const Node* root);

int main(void) {
    char input[MAX_INPUT + 2];
    printf("Enter a lowercase message: ");
    if (fgets(input, sizeof input, stdin) == NULL) return 1;
    input[strcspn(input, "\n")] = '\0';

    int freq[ALPHA];
    if (count_freqs(input, freq) != 0) {
        printf("Input must be lowercase a-z only\n");
        return 1;
    }

    Node* root = build_tree(freq);
    if (root == NULL) {
        printf("Empty input\n");
        return 0;
    }

    char codes[ALPHA][ALPHA] = {{0}};
    char buf[ALPHA];
    build_codes(root, buf, 0, codes);

    char* bits = encode(input, codes);
    char* back = decode(bits, root);

    printf("input:   %s\n", input);
    printf("encoded: %s\n", bits);
    printf("decoded: %s\n", back);
    printf("bits:    %zu vs %zu\n", strlen(bits), strlen(input) * 8);
    printf("match:   %s\n", strcmp(input, back) == 0 ? "yes" : "no");

    free(bits);
    free(back);
    free_tree(root);
    return 0;
}

Node* new_node(int weight, char symbol, Node* left, Node* right) {
    Node* n = malloc(sizeof *n);
    n->weight = weight;
    n->symbol = symbol;
    n->left = left;
    n->right = right;
    return n;
}

void free_tree(Node* n) {
    if (n == NULL) return;
    free_tree(n->left);
    free_tree(n->right);
    free(n);
}

int count_freqs(const char* s, int freq[ALPHA]) {
    for (int i = 0; i < ALPHA; i++) freq[i] = 0;
    for (; *s; s++) {
        if (*s < 'a' || *s > 'z') return -1;
        freq[*s - 'a']++;
    }
    return 0;
}

int q_size(const Queue* q) { return q->tail - q->head; }
void q_push(Queue* q, Node* n) { q->items[q->tail++] = n; }
Node* q_front(const Queue* q) { return q->items[q->head]; }
Node* q_pop(Queue* q) { return q->items[q->head++]; }

// Take the lighter of the two fronts, handling an empty queue.
Node* pop_min(Queue* q1, Queue* q2) {
    if (q_size(q1) == 0) return q_pop(q2);
    if (q_size(q2) == 0) return q_pop(q1);
    return (q_front(q1)->weight <= q_front(q2)->weight) ? q_pop(q1) : q_pop(q2);
}

int cmp_weight(const void* a, const void* b) {
    const Node* x = *(Node* const*)a;
    const Node* y = *(Node* const*)b;
    if (x->weight != y->weight) return x->weight - y->weight;
    return x->symbol - y->symbol;  // tie-break by letter
}

Node* build_tree(const int freq[ALPHA]) {
    Queue q1 = {0}, q2 = {0};
    for (int i = 0; i < ALPHA; i++)
        if (freq[i] > 0) q_push(&q1, new_node(freq[i], 'a' + i, NULL, NULL));
    if (q_size(&q1) == 0) return NULL;  // empty input

    qsort(q1.items, q_size(&q1), sizeof(Node*), cmp_weight);

    // Merge the two smallest until one node is left.
    while (q_size(&q1) + q_size(&q2) > 1) {
        Node* a = pop_min(&q1, &q2);
        Node* b = pop_min(&q1, &q2);
        q_push(&q2, new_node(a->weight + b->weight, 0, a, b));
    }
    return pop_min(&q1, &q2);
}

// Left = '0', right = '1'. A code is at most 25 bits, plus '\0'.
void build_codes(const Node* n, char* buf, int depth,
                 char codes[ALPHA][ALPHA]) {
    if (n->left == NULL) {                   // leaf
        if (depth == 0) buf[depth++] = '0';  // single-letter input
        buf[depth] = '\0';
        strcpy(codes[n->symbol - 'a'], buf);
        return;
    }
    buf[depth] = '0';
    build_codes(n->left, buf, depth + 1, codes);
    buf[depth] = '1';
    build_codes(n->right, buf, depth + 1, codes);
}

char* encode(const char* s, char codes[ALPHA][ALPHA]) {
    size_t total = 0;
    for (const char* p = s; *p; p++) total += strlen(codes[*p - 'a']);

    char* out = malloc(total + 1);
    char* w = out;
    for (const char* p = s; *p; p++) {
        const char* c = codes[*p - 'a'];
        size_t len = strlen(c);
        memcpy(w, c, len);
        w += len;
    }
    *w = '\0';
    return out;
}

char* decode(const char* bits, const Node* root) {
    char* out = malloc(strlen(bits) + 1);
    char* w = out;

    if (root->left == NULL) {  // single-letter tree: every bit is that letter
        for (const char* p = bits; *p; p++) *w++ = root->symbol;
        *w = '\0';
        return out;
    }

    const Node* n = root;
    for (const char* p = bits; *p; p++) {
        n = (*p == '0') ? n->left : n->right;
        if (n->left == NULL) {  // reached a leaf
            *w++ = n->symbol;
            n = root;
        }
    }
    *w = '\0';
    return out;
}