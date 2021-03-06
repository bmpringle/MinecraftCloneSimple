#ifndef FONTLOADER_H
#define FONTLOADER_H

#include <string>

#include "stb/stb_truetype.h"

class FontLoader {
    public:
        FontLoader(std::string file);

        unsigned char* getTextBitmap(std::string text);

        int getLH();
        int getBH();
        int getBW();
    private:
        stbtt_fontinfo info;
        unsigned char* fontBuffer = nullptr;
        unsigned char* bitmap = nullptr;
        int l_h = 0;
        int b_w = 0;
        int b_h = 0;
};
#endif