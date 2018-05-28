#include <iostream>
using namespace std;

typedef struct {
    unsigned char  b1, b2;
    unsigned short  bfSize;
    unsigned short  bfReserved1;
    unsigned short  bfReserved2;
    unsigned short  padding;
    unsigned short  bfOffBits;
} BM;
typedef struct{
    int32_t infoSize;
    int32_t width;
    int32_t depth;
    int16_t biPlanes;
    int16_t bits;
    int32_t biCompression;
    int32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    int32_t biClrUsed;
    int32_t biClrImportant;
}  BM2;
struct PIXELDATA{
    int8_t redComponent;
    int8_t blueComponent;
    int8_t greenComponent;
};

int main(int argc, char *argv[]) {

    int mult = atoi(argv[3]);

    FILE *file;
    file = fopen(argv[1], "rb");
    BM bm;
    BM2 bm2;
    fread(&bm, 1, 14, file);
    fread(&bm2, 1, 40, file);

    int real_width = 3 * bm2.width + (4 - ((3 * bm2.width) % 4)) % 4;

    FILE *output;
    output = fopen(argv[2], "wb");

    PIXELDATA map[bm2.depth][bm2.width];
    PIXELDATA map2[bm2.depth * abs(mult)][bm2.width * abs(mult)];

    int new_real_width = abs(mult) * 3 * bm2.width + (4 - ((abs(mult) * 3 * bm2.width) % 4)) % 4;

    bm.bfSize = 54 + (long) new_real_width * bm2.depth * mult;

    const char null_byte = ' ';
    char garbage;
    int garbage_bytes = real_width - bm2.width * 3;


    for (int i = bm2.depth - 1; i >= 0; i--) {
        for (int j = 0; j < bm2.width; j++)
            fread(&map[i][j], 1, 3, file);
        if (garbage_bytes)
            fread(&garbage, 1, garbage_bytes, file);
    }
    if(mult > 0) {
        int p = 0, s = 0;
        for (int i = 0; i < bm2.depth; i++) {
            for (int l = 0; l < abs(mult); l++) {
                for (int j = 0; j < bm2.width; j++) {
                    for (int k = 0; k < abs(mult); k++) {
                        map2[p][s] = map[i][j];
                        s++;
                    }
                }
                s = 0;
                p++;
            }
        }
    }
    else if(mult < 0) {
        int p = 0, s = bm2.width - real_width / 3;
        for (int i = 0; i < bm2.depth; i++) {
            for (int l = 0; l < abs(mult); l++) {
                for (int j = 0; j < bm2.width; j++) {
                    for (int k = 0; k < abs(mult); k++) {
                        map2[p][s] = map[i][j];
                        s--;
                    }
                }
                s = bm2.width - real_width / 3;
                p++;
            }
        }
    }

    bm2.depth *= abs(mult);
    bm2.width *= abs(mult);

    fwrite(&bm, 1, 14, output);
    fwrite(&bm2, 1, 40, output);

    int garbage_bytes_out = new_real_width - bm2.width * 3;
    for (int y = bm2.depth - 1; y >= 0; y--) {
        for (int x = 0; x < bm2.width; x++)
            fwrite(&map2[y][x], 1, 3, output);
        if (garbage_bytes_out)
            fwrite(&null_byte, 1, garbage_bytes_out, output);
    }

    fclose(output);
    fclose(file);

    return 0;
}

