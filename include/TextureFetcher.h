#ifndef TEXTUREFETCHER_H
#define TEXTUREFETCHER_H

#include <string>
#include <map>

class TextureFetcher {
    public:
        TextureFetcher();
        unsigned int getOrLoadTexture(std::string name);
        unsigned int getOrLoadTexture(std::string name, int gl_setting1, int gl_setting2);
    
    private:
        std::map<std::string, unsigned int> textureIDs = std::map<std::string, unsigned int>();
};
#endif