#ifndef TEXTUREFETCHER_H
#define TEXTUREFETCHER_H

#include <string>
#include <map>

class TextureFetcher {
    public:
        TextureFetcher();
        unsigned int getOrLoadTexture(std::string name);
    
    private:
        std::map<std::string, unsigned int> textureIDs = std::map<std::string, unsigned int>();
};
#endif