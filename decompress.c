#include "decompress.h"
#include "compress.h"

char read_bit(FILE *in) {
    if (rbuf_available == 0) {
        rbuf = getc(in);
        rbuf_available = 8;
    }
    rbuf_available--;
    return (rbuf >> rbuf_available) & 1;
}

unsigned char read_char(FILE *in) {
    unsigned char c = 0;
    for (int i = 0; i < 8; i++) {
        c <<= 1;
        c += read_bit(in);
    }
    return c;
}

void decompress(const char *input_fname, const char *output_fname) {
    buf_out = 0;
    buf_free = 8;
    rbuf = 0;
    rbuf_available = 0;
    FILE *input = fopen(input_fname, "rb");
    int alphabet = read_char(input) + 1;
    char *map[256] = {NULL};
    char len[256] = {0};

    for (int i = 0; i < alphabet; ++i) {
        unsigned char c = read_char(input);
        len[c] = read_char(input);
        map[c] = (char *) malloc(len[c] + 1);
        memset(map[c], 0, len[c] + 1);
        for (int j = 0; j < len[c]; j++) {
            map[c][j] = (read_bit(input) ? '1' : '0');
        }
    }
    int sm = read_char(input);
    sm = (sm << 8) + read_char(input);
    sm = (sm << 8) + read_char(input);
    sm = (sm << 8) + read_char(input);

    int curr_len = 0;
    char curr[256] = {0};
    FILE *output = fopen(output_fname, "wb");
    for (int bit = 0; bit < sm; bit++) {
        curr[curr_len] = (read_bit(input) ? '1' : '0');
        curr_len++;
        for (int i = 0; i < 256; i++) {
            if (len[i] == curr_len && strcmp(curr, map[i]) == 0) {
                print_char(output, i);
                curr_len = 0;
                memset(curr, 0, sizeof curr);
                break;
            }
        }
    }

    fclose(output);
}

