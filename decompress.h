#ifndef HUFFMAN_DECOMPRESS_H
#define HUFFMAN_DECOMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern unsigned char buf_out;
extern int buf_free;
extern unsigned char rbuf;
extern unsigned char rbuf_available;

char read_bit(FILE *in);
unsigned char read_char(FILE *in);
void decompress(const char *input_fname, const char *output_fname);


#endif //HUFFMAN_DECOMPRESS_H
