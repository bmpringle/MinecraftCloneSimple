#include "TextureFetcher.h"
#include "RenderInclude.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"

TextureFetcher::TextureFetcher() {

}

unsigned int TextureFetcher::getOrLoadTexture(std::string name) {
    if(textureIDs.find(name) != textureIDs.end()) {
        return textureIDs.at(name);
    }

    int width, height, nrChannels;
    unsigned char *data = stbi_load(("./src/assets/"+name).c_str(), &width, &height, &nrChannels, 0); 

    if(data == nullptr) {
        return -1;
    }

    unsigned int TBO;

    glGenTextures(1, &TBO);

    glBindTexture(GL_TEXTURE_2D, TBO); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    if(nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindVertexArray(0);

    textureIDs.insert(std::make_pair(name, TBO));
    return TBO;
}