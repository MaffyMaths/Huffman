#include "compress.h"
#include "decompress.h"

int main() {
    compress("����.mp3", "compressed.bin");
    decompress("compressed.bin", "out.mp3");
    return 0;
}
