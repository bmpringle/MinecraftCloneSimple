#include "TextureArrayCreator.h"
#include <algorithm>
#include "RenderInclude.h"
#include "stbi/stb_image.h"
#include <iostream>

TextureArrayCreator::TextureArrayCreator() {

}

void TextureArrayCreator::addTextureToList(std::string texPath) {
    if(std::find(texturePaths.begin(), texturePaths.end(), texPath) != texturePaths.end()) {
        return;
    }else {
        texturePaths.push_back(texPath);
    }
}

unsigned int TextureArrayCreator::generateTextureArray() {
    unsigned int texBuffer;
    glGenBuffers(1, &texBuffer);

    glBindTexture(GL_TEXTURE_2D_ARRAY, texBuffer);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, 16, 16, texturePaths.size(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    textureDepths.clear();

    for(int i = 0; i < texturePaths.size(); ++i) {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(("./src/assets/"+texturePaths.at(i)).c_str(), &width, &height, &nrChannels, 0); 


        if(data == nullptr) {
            return -1;
        }

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 16, 16, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

        textureDepths.push_back(i);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    }

    
    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 1);

    arrayID = texBuffer;

    return texBuffer;
}

unsigned int TextureArrayCreator::getGeneratedTextureArray() {
    if(arrayID == -1) {
        return generateTextureArray();
    }else {
        return arrayID;
    }
}

int TextureArrayCreator::getTextureLayer(std::string tex) {
    std::vector<std::string>::iterator it = std::find(texturePaths.begin(), texturePaths.end(), tex);
    if(it == texturePaths.end()) {
        return -1;
    }

    int index = it - texturePaths.begin();

    if(textureDepths.at(index) == -1) {
        generateTextureArray();
        
        it = std::find(texturePaths.begin(), texturePaths.end(), tex);
        if(textureDepths.at(index) == -1) {
            return -1;
        }
    }
    

    return textureDepths.at(index);
}