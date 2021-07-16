#include "TextureFetcher.h"
#include "RenderInclude.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <iostream>

TextureFetcher::TextureFetcher() {

}

unsigned int TextureFetcher::getOrLoadTexture(std::string name) {
    return getOrLoadTexture(name, GL_REPEAT, GL_NEAREST);
}

unsigned int TextureFetcher::getOrLoadAbsolutePathTexture(std::string name, int gl_setting1, int gl_setting2) {
    if(textureIDs.find(name) != textureIDs.end()) {
        return textureIDs.at(name);
    }

    int width, height, nrChannels;
    unsigned char *data = stbi_load((name).c_str(), &width, &height, &nrChannels, 0); 

    if(data == nullptr) {
        return -1;
    }

    unsigned int TBO;

    glGenTextures(1, &TBO);

    glBindTexture(GL_TEXTURE_2D, TBO); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_setting1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_setting1);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, gl_setting2); 
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, gl_setting2);
    
    if(nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindVertexArray(0);

    textureIDs.insert(std::make_pair(name, TBO));

    stbi_image_free(data);

    return TBO;
}

unsigned int TextureFetcher::getOrLoadTexture(std::string name, int gl_setting1, int gl_setting2) {
    if(textureIDs.contains(name)) {
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_setting1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_setting1);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, gl_setting2); 
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, gl_setting2);
    
    if(nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindVertexArray(0);

    textureIDs.insert(std::make_pair(name, TBO));

    stbi_image_free(data);

    return TBO;
}
