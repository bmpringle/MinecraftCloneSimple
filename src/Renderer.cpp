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
    OpenGLRenderer::appendVectorWithVector(vectorToAppendTo, vectorToAppend);
}

glm::mat4x4 Renderer::calculatePerspectiveMatrix(double FOV, double aspectRatio, double zNear, double zFar) {
    return OpenGLRenderer::calculatePerspectiveMatrix(FOV, aspectRatio, zNear, zFar);
}

glm::mat3x3 Renderer::calculateXRotationMatrix(double xRotation) {
    return OpenGLRenderer::calculateXRotationMatrix(xRotation);
}

glm::mat3x3 Renderer::calculateYRotationMatrix(double yRotation) {
    return OpenGLRenderer::calculateYRotationMatrix(yRotation);
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