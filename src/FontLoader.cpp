#include "FontLoader.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stbi/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION 
#include "stbi/stb_truetype.h"

FontLoader::FontLoader(std::string file) {
    /* load font file */
    long size;
    unsigned char* fontBuffer;
    
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
        printf("failed to init stbtruetype library\n");
    }
    
    free(fontBuffer);
}

unsigned char* FontLoader::getTextBitmap(std::string text) {    
    l_h = 32; /* line height */
    b_h = l_h; /* bitmap height */
    b_w = text.size() * 32; /* bitmap width */

    /* create a bitmap for the phrase */
    unsigned char* bitmap = (unsigned char*)calloc(b_w * b_h, sizeof(unsigned char));
    
    /* calculate font scaling */
    float scale = stbtt_ScaleForPixelHeight(&info, l_h);
    
    int x = 0;
       
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);
    
    int i;
    for (i = 0; i < strlen(text.c_str()); ++i)
    {
        /* how wide is this character */
        int ax;
		int lsb;
        stbtt_GetCodepointHMetrics(&info, text.c_str()[i], &ax, &lsb);

        /* get bounding box for character (may be offset to account for chars that dip above or below the line */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, text.c_str()[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        
        /* compute y (different characters have different heights */
        int y = ascent + c_y1;
        
        /* render character (stride and offset is important here) */
        int byteOffset = x + roundf(lsb * scale) + (y * b_w);
        stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, text.c_str()[i]);

        /* advance x */
        x += roundf(ax * scale);
        
        /* add kerning */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(&info, text.c_str()[i], text.c_str()[i + 1]);
        x += roundf(kern * scale);
    }
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