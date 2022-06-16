#ifndef HUFFMAN_COMPRESS_H
#define HUFFMAN_COMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern unsigned char buf_out;
extern int buf_free;
extern unsigned char rbuf;
extern unsigned char rbuf_available;

typedef struct node {
    unsigned char symb;
    unsigned char isSymb;
    unsigned int freq;
    int level;
    struct node *left, *right, *next;
} NODE;

typedef NODE *nodeptr;

long file_size(FILE *file);
nodeptr addToList(nodeptr curr, nodeptr v);
nodeptr buildList(long *freq, int *listSize);
int max(int a, int b);
void buildCodes(nodeptr tree, char **map, char *curr, int ind);
void print_bit(FILE *out, unsigned char bit);
void print_char(FILE *out, unsigned char c);
void close(FILE *out);
void compress(const char *input_filename, const char *output_filename);

#endif //HUFFMAN_COMPRESS_H
