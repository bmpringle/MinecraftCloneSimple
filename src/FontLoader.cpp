#include "FontLoader.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

/*
 *for debugging only
 *#define STB_IMAGE_WRITE_IMPLEMENTATION
 *#include "stb/stb_image_write.h"
 */

#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb/stb_truetype.h"

FontLoader::FontLoader(std::string file) {
    /* load font file */
    long size;
    
    FILE* fontFile = fopen(file.c_str(), "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset */
    
    fontBuffer = (unsigned char*)malloc(size);
    
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);

    /* prepare font */
    if (!stbtt_InitFont(&info, fontBuffer, 0))
    {
        printf("failed\n");
    }
}

unsigned char* FontLoader::getTextBitmap(std::string text) {    
    b_w = 0; /* bitmap width */
    b_h = 64; /* bitmap height */
    l_h = 64; /* line height */

    char* word = (char*)calloc(strlen(text.c_str())+1, sizeof(char));
    memcpy(word, text.c_str(), strlen(text.c_str())+1);

    float scale = stbtt_ScaleForMappingEmToPixels(&info, l_h);

    std::vector<int> lineb_ws = std::vector<int>();

    int b_w_buffer = 0;

    for (int i = 0; i < strlen(word); ++i) {
        if(word[i] == '\n') {
            b_w = std::max(b_w, b_w_buffer);
            lineb_ws.push_back(b_w_buffer);
            b_w_buffer = 0;
        }
        int ax;
        int lsb;
        stbtt_GetCodepointHMetrics(&info, word[i], &ax, &lsb);
        b_w_buffer += scale * ax;
    }
    b_w = std::max(b_w, b_w_buffer);
    lineb_ws.push_back(b_w_buffer);
    
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);

    b_h = abs(ascent) + abs(descent);
    
    int newlines = 0;
    for(char c : text) {
        if(c == '\n') {
            ++newlines;
        }
    }

    b_h = b_h * (newlines + 1);

    int x = (b_w - lineb_ws[0])/2;

    /* create a bitmap for the phrase */
    bitmap = (unsigned char*)calloc(b_w * b_h, sizeof(unsigned char));
    
    int i;
    int l = 0;
    for (i = 0; i < strlen(word); ++i)
    {
        if(word[i] == '\n') {
            ++i;
            ++l;
            x = (b_w - lineb_ws[l])/2;
        }

        /* how wide is this character */
        int ax;
        int lsb;
        stbtt_GetCodepointHMetrics(&info, word[i], &ax, &lsb);

        /* get bounding box for character (may be offset to account for chars that dip above or below the line */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        
        /* compute y (different characters have different heights */
        int y = ascent + c_y1 + l * l_h;
        
        /* render character (stride and offset is important here) */
        int byteOffset = x + roundf(lsb * scale) + (y * b_w);
        stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);

        /* advance x */
        x += roundf(ax * scale);
        
        /* add kerning */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
        x += roundf(kern * scale);
    }
    
    free(word);
    return bitmap;
}

int FontLoader::getLH() {
    return l_h;
}

int FontLoader::getBH() {
    return b_h;
}

int FontLoader::getBW() {
    return b_w;
}
