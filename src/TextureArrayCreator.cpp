#include "TextureArrayCreator.h"
#include <algorithm>
#include "RenderInclude.h"
#include "stbi/stb_image.h"

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

    glBindBuffer(GL_TEXTURE_2D_ARRAY, texBuffer);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, 16, 16, texturePaths.size(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    for(int i = 0; i < texturePaths.size(); ++i) {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(("./src/assets/"+texturePaths.at(i)).c_str(), &width, &height, &nrChannels, 0); 


        if(data == nullptr) {
            return -1;
        }

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 16, 16, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
    //glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 1);

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

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