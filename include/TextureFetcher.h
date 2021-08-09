#ifndef TEXTUREFETCHER_H
#define TEXTUREFETCHER_H

#include <string>
#include <map>

class TextureFetcher {
    public:
        TextureFetcher();
        unsigned int getOrLoadTexture(std::string name);
        unsigned int getOrLoadTexture(std::string name, int gl_setting1, int gl_setting2);
        unsigned int getOrLoadAbsolutePathTexture(std::string name, int gl_setting1, int gl_setting2);

        void loadBitmapToTexture(std::string id, unsigned int TBO, unsigned int width, unsigned int height);
        std::pair<unsigned int, unsigned int> getTextureDimensions(std::string name);
    
    private:
        std::map<std::string, unsigned int> textureIDs = std::map<std::string, unsigned int>();
        std::map<std::string, std::pair<unsigned int, unsigned int>> textureDimensions = std::map<std::string, std::pair<unsigned int, unsigned int>>();
};
#endif