#include "compress.h"
#include "decompress.h"

int main() {
    compress("aaa.txt", "compressed.bin");
    decompress("compressed.bin", "work.txt");
    return 0;
}
