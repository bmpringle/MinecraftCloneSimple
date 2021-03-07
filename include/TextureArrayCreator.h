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

        int getTextureLayer(std::string tex);

    private:
        std::vector<std::string> texturePaths = std::vector<std::string>();
        std::vector<int> textureDepths = std::vector<int>();

        unsigned int arrayID = 0;
        bool arrayInit = false;
};

#endif