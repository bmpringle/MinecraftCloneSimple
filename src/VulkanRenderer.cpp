#ifdef VULKAN_BACKEND

#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer() {
    renderer.setClearColor(glm::vec4(0, 0, 1, 1));
    renderer.setOverlayBounds(1000, 1000, 1000);
    glfwSetInputMode(getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void VulkanRenderer::renderFrame(World* world) {
    renderer.recordCommandBuffers();

    renderer.renderFrame();
}

void VulkanRenderer::setOverlayData(std::string overlayID, float rectangle[48], std::string texture) {
    int textureID = -1;
    try {
        textureID = renderer.getTextureID(texture);
    } catch(std::runtime_error ex) {
        renderer.addTexture(texture, "src/assets/" + texture);
        textureID = renderer.getTextureID(texture);
    }
    
    std::vector<OverlayVertex> rectangleOverlay;
    for(int i = 5; i >= 0; --i) {
        OverlayVertex v;
        /*
        999+z is because the opengl engine/mccsimple overlay system says that 0 is as far away as you can get from the screen, 
        and negative numbers bring you closer. So, to adjust that to the VulkanEngine coordinate system, you must negate the input 
        z, then subtract that from 999.  
        */
        v.position = glm::vec3(rectangle[8*i], aspectRatio * rectangle[8*i+1], 999+rectangle[8*i+2]); 
        v.color = glm::vec3(rectangle[8*i+3], rectangle[8*i+4], rectangle[8*i+5]);
        v.texCoord = glm::vec2(rectangle[8*i+6], rectangle[8*i+7]);
        v.texID = textureID;
        rectangleOverlay.push_back(v);
    }

    renderer.setOverlayVertices(overlayID, rectangleOverlay);
}

void VulkanRenderer::setOverlayData(std::string overlayID, float rectangle[36]) {
    /*std::vector<OverlayVertex> rectangleOverlay;
    for(int i = 0; i < 6; ++i) {
        OverlayVertex v;
        v.position = glm::vec2(rectangle[6*i], rectangle[6*i+1]);
        v.color = glm::vec3(rectangle[6*i+3], rectangle[6*i+4], rectangle[6*i+5]);
        v.texCoord = glm::vec2(0, 0);
        v.texID = 0;
        rectangleOverlay.push_back(v);
    }
    renderer.setOverlayVertices(overlayID, rectangleOverlay);*/
}

void VulkanRenderer::removeOverlayData(std::string overlayID) {
    renderer.removeOverlayVertices(overlayID);
}

void VulkanRenderer::renderBlockInWireframe(World* world, BlockPos pos) {

}

void VulkanRenderer::updateAspectRatio(GLFWwindow* window) {
    int width;
    int height;
    glfwGetWindowSize(getWindowPtr(), &width, &height);

    aspectRatio = (float)width / (float)height;
}

void VulkanRenderer::updateWorldVBO(World* world) {

}

void VulkanRenderer::textTextureBuffer(std::string id, std::string text) {
    bool isTextSpaces = true;
    for(char c : text) {
        if(c != ' ') {
            isTextSpaces = false;
            break;
        }
    }
    if(isTextSpaces) {
        renderer.addTexture(id, "src/assets/transparent.png");
        return;
    }
    renderer.addTextTexture(id, text);
}

int VulkanRenderer::getWidth() {
    int width;
    glfwGetWindowSize(getWindowPtr(), &width, nullptr);
    return width;
}

int VulkanRenderer::getHeight() {
    int height;
    glfwGetWindowSize(getWindowPtr(), nullptr, &height);
    return height;
}

template<class T>
void VulkanRenderer::appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T>& vectorToAppend) {

}

glm::mat3x3 VulkanRenderer::calculateXRotationMatrix(double xRotation) {
    return VKRenderer::calculateXRotationMatrix(xRotation);
}
glm::mat3x3 VulkanRenderer::calculateYRotationMatrix(double yRotation) {
    return VKRenderer::calculateYRotationMatrix(yRotation);
}

glm::mat4x4 VulkanRenderer::calculatePerspectiveMatrix(double FOV, double aspectRatio, double zNear, double zFar) {
    //return VKRenderer::calculatePerspectiveMatrix(FOV, aspectRatio, zNear, zFar);
    return glm::mat4x4();
}

std::pair<unsigned int, unsigned int> VulkanRenderer::getTextureDimensions(std::string id) {
    return renderer.getTextureDimensions(id);
}

std::array<int, 2> VulkanRenderer::overlayDimensions() {
    return {1000, 1000};
}

void VulkanRenderer::setupEntityRenderer() {

}

void VulkanRenderer::cleanup() {

}

GLFWwindow* VulkanRenderer::getWindowPtr() {
    return renderer.getEngine()->getDisplay()->getInternalWindow();
}

#endif