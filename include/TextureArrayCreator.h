#ifndef TEXTUREARRAYCREATOR_H
#define TEXTUREARRAYCREATOR_H

#include <string>
#include <vector>

class TextureArrayCreator {
    public:
        TextureArrayCreator();

        void addTextureToList(std::string texPath);

        unsigned int generateTextureArray();

        unsigned int getGeneratedTextureArray();
    private:
        std::vector<std::string> texturePaths = std::vector<std::string>();

        unsigned int arrayID = -1;
};

#endif