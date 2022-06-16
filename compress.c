#include "compress.h"

unsigned char buf_out = 0;
int buf_free = 8;
unsigned char rbuf = 0;
unsigned char rbuf_available = 0;

long file_size(FILE *file) {
    if (!file) {
        return 0;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}


nodeptr addToList(nodeptr curr, nodeptr v) {
    if (curr == NULL) {
        return v;
    }
    nodeptr first = curr;
    if (curr->freq >= v->freq) {
        v->next = curr;
        return v;
    }
    while (curr->next != NULL) {
        if (curr->next->freq >= v->freq) {
            break;
        }
        curr = curr->next;
    }
    v->next = curr->next;
    curr->next = v;
    return first;
}

nodeptr buildList(long *freq, int *listSize) {
    nodeptr list = NULL;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            nodeptr vert = (nodeptr) malloc(sizeof(NODE));
            vert->left = NULL;
            vert->right = NULL;
            vert->freq = freq[i];
            vert->isSymb = 1;
            vert->symb = (unsigned char) i;
            vert->next = NULL;
            vert->level = 0;
            list = addToList(list, vert);
            (*listSize)++;
        }
    }
    return list;
}

int max(int a, int b) {
    return (a > b ? a : b);
}

void buildCodes(nodeptr tree, char **map, char *curr, int ind) {
    if (tree->isSymb) {
        map[(unsigned char) tree->symb] = (char *) malloc(strlen(curr) + 1);
        memcpy(map[(unsigned char) tree->symb], curr, strlen(curr) + 1);
        return;
    }
    curr[ind] = '0';
    buildCodes(tree->left, map, curr, ind + 1);
    curr[ind] = '1';
    buildCodes(tree->right, map, curr, ind + 1);
    curr[ind] = '\0';
}

void print_bit(FILE *out, unsigned char bit) {
    buf_out <<= 1;
    buf_out += (bit ? 1 : 0);
    buf_free--;
    if (buf_free == 0) {
        fputc(buf_out, out);
        buf_out = 0;
        buf_free = 8;
    }
}

void print_char(FILE *out, unsigned char c) {
    for (int i = 7; i >= 0; --i) {
        print_bit(out, (c >> i) & 1);
    }
}

void close(FILE *out) {
    if (buf_free != 8) {
        fputc(buf_out << buf_free, out);
    }
    fclose(out);
}

void compress(const char *input_filename, const char *output_filename) {
    FILE *input = fopen(input_filename, "rb");
    long size = file_size(input);
    long freq[256] = {0};
    for (long i = 0; i < size; i++) {
        ++freq[(unsigned char) fgetc(input)];
    }
    int listSize = 0;
    nodeptr list = buildList(freq, &listSize);
    while (listSize > 1) {
        nodeptr le = list;
        nodeptr ri = le->next;
        nodeptr nl = ri->next;

        nodeptr par = (nodeptr) malloc(sizeof(NODE));
        par->left = le;
        par->right = ri;
        par->level = max(le->level, ri->level) + 1;
        par->next = NULL;
        par->freq = le->freq + ri->freq;
        par->isSymb = 0;

        le->next = NULL;
        ri->next = NULL;
        list = addToList(nl, par);
        listSize--;
    }

    nodeptr tree = list;
    char *map[256] = {NULL};
    char curr[256] = {0};
    int len[256] = {0};
    buildCodes(tree, map, curr, 0);

    int alphabet_size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            len[i] = strlen(map[i]);
            alphabet_size++;
        }
    }

    FILE *output = fopen(output_filename, "wb");
    print_char(output, alphabet_size - 1);
    long sum = 0;

    for (int i = 0; i < 256; i++) {
        sum += freq[i] * len[i];
        if (freq[i]) {
            print_char(output, i);
            print_char(output, len[i]);
            for (int j = 0; j < len[i]; j++) {
                print_bit(output, map[i][j] == '1');
            }
        }
    }

    print_char(output, 255 & (sum >> 24));
    print_char(output, 255 & (sum >> 16));
    print_char(output, 255 & (sum >> 8));
    print_char(output, 255 & (sum >> 0));
    printf("Compression(bytes):\n%d orig\n%d result", size, sum / 8);

    fseek(input, 0, SEEK_SET);
    for (int i = 0; i < size; i++) {
        unsigned char c = getc(input);
        for (int j = 0; j < len[c]; ++j) {
            print_bit(output, map[c][j] == '1');
        }
    }

    close(output);
}

