#include "Renderer.h"
#include "World.h"
#include <algorithm>
#include <thread>
#include "Blocks.h"
#include <math.h>
#include "EntityVertex.h"

#define WORLDSIZE_CONST 100

Renderer::Renderer() : internalRenderer(INTERNAL_RENDERER()){

}

void Renderer::updateWorldVBO(World* world) {
    internalRenderer.updateWorldVBO(world);
}

void Renderer::renderFrame(World* world) {
    internalRenderer.renderFrame(world);
}

void Renderer::updateAspectRatio(GLFWwindow* window) {
    internalRenderer.updateAspectRatio(window);
}

void Renderer::renderBlockInWireframe(World* world, BlockPos pos) {
    internalRenderer.renderBlockInWireframe(world, pos);
}

int Renderer::getWidth() {
    return internalRenderer.getWidth();
}

int Renderer::getHeight() {
    return internalRenderer.getHeight();
}

template<class T>
void Renderer::appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T>& vectorToAppend) {
    #ifndef VULKAN_BACKEND
    OpenGLRenderer::appendVectorWithVector(vectorToAppendTo, vectorToAppend);
    #endif
    throw std::runtime_error("appendVectorWithVector isn't defined for the Vulkan backend");
    return;
}

glm::mat4x4 Renderer::calculatePerspectiveMatrix(double FOV, double aspectRatio, double zNear, double zFar) {
    #ifndef VULKAN_BACKEND
    return OpenGLRenderer::calculatePerspectiveMatrix(FOV, aspectRatio, zNear, zFar);
    #else
    throw std::runtime_error("calculatePerspectiveMatrix isn't defined for the Vulkan backend");
    //return VulkanRenderer::calculatePerspectiveMatrix(FOV, aspectRatio, zNear, zFar);
    #endif
}

glm::mat3x3 Renderer::calculateXRotationMatrix(double xRotation) {
    #ifndef VULKAN_BACKEND
    return OpenGLRenderer::calculateXRotationMatrix(xRotation);
    #else
    //throw std::runtime_error("calculateXRotationMatrix isn't defined for the Vulkan backend");
    return VulkanRenderer::calculateXRotationMatrix(xRotation);
    #endif
}

glm::mat3x3 Renderer::calculateYRotationMatrix(double yRotation) {
    #ifndef VULKAN_BACKEND
    return OpenGLRenderer::calculateYRotationMatrix(yRotation);
    #else
    //throw std::runtime_error("calculateYRotationMatrix isn't defined for the Vulkan backend");
    return VulkanRenderer::calculateYRotationMatrix(yRotation);
    #endif
}

std::array<int, 2> Renderer::overlayDimensions() {
    return internalRenderer.overlayDimensions();
}

void Renderer::textTextureBuffer(std::string id, std::string text) {
    internalRenderer.textTextureBuffer(id, text);
}

std::pair<unsigned int, unsigned int> Renderer::getTextureDimensions(std::string id) {
    return internalRenderer.getTextureDimensions(id);
}

void Renderer::setupEntityRenderer() {
    internalRenderer.setupEntityRenderer();
}


void Renderer::cleanup() {
    internalRenderer.cleanup();
}

void Renderer::setOverlayData(std::string overlayID, float rectangle[48], std::string texture) {
    internalRenderer.setOverlayData(overlayID, rectangle, texture);
}

void Renderer::removeOverlayData(std::string overlayID) {
    internalRenderer.removeOverlayData(overlayID);
}

void Renderer::setOverlayData(std::string overlayID, float rectangle[36]) {
    internalRenderer.setOverlayData(overlayID, rectangle);
}

GLFWwindow* Renderer::getWindowPtr() {
    return internalRenderer.getWindowPtr();
}