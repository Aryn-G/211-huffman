#include <stdio.h>
#include <stdlib.h>

char* encode(char* input);
char* decode(char* bytes);

void heapify(int* arr);
void heappush(int* arr, int value);

int main(int argc, char* argv[]) {
    // printf("%d\n", argc);
    // for (int i = 0; i < argc; i++) {
    //     printf("%s\n", argv[i]);
    // }
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <encode|decode> <input>\n", argv[0]);
        return 1;
    }

    if (argv[1] == "encode") {
        char* encoded = encode(argv[2]);
    }

    if (argv[1] == "decode") {
        char* decoded = decode(argv[2]);
    }

    return 0;
}

char* encode(char* input) { return NULL; }

char* decode(char* bytes) { return NULL; }