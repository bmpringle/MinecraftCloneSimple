#include "Renderer.h"
#include "World.h"
#include <algorithm>
#include <thread>
#include "Blocks.h"
#include <math.h>
#include "EntityVertex.h"

#define WORLDSIZE_CONST 100

Renderer::Renderer() : openGLRenderer(OpenGLRenderer()){

}

void Renderer::updateWorldVBO(World* world) {
    openGLRenderer.updateWorldVBO(world);
}

void Renderer::renderFrame(World* world) {
    openGLRenderer.renderFrame(world);
}

void Renderer::renderOverlay(float rectangle[48], std::string texture) {
    openGLRenderer.renderOverlay(rectangle, texture);
}

void Renderer::renderOverlay(float rectangle[48], unsigned int TBO) {
    openGLRenderer.renderOverlay(rectangle, TBO);
}

void Renderer::updateAspectRatio(GLFWwindow* window) {
    openGLRenderer.updateAspectRatio(window);
}

void Renderer::renderBlockInWireframe(World* world, BlockPos pos) {
    openGLRenderer.renderBlockInWireframe(world, pos);
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

int Renderer::getWidth() {
    return openGLRenderer.getWidth();
}

int Renderer::getHeight() {
    return openGLRenderer.getHeight();
}

std::array<int, 2> Renderer::overlayDimensions() {
    return openGLRenderer.overlayDimensions();
}

unsigned int Renderer::textTextureBuffer(std::string text) {
    return openGLRenderer.textTextureBuffer(text);
}

int Renderer::getBitmapHeight() {
    return openGLRenderer.getBitmapHeight();
}

int Renderer::getBitmapWidth() {
    return openGLRenderer.getBitmapWidth();
}


void Renderer::renderRectangle(float rectangle[36]) {
    openGLRenderer.renderRectangle(rectangle);
}

void Renderer::setupEntityRenderer() {
    openGLRenderer.setupEntityRenderer();
}


void Renderer::cleanup() {
    openGLRenderer.cleanup();
}
