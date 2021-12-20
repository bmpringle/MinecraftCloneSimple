#ifdef VULKAN_BACKEND

#include "VulkanRenderer.h"

#include "Blocks.h"

#include "World.h"

VulkanRenderer::VulkanRenderer() {
    renderer.setClearColor(glm::vec4(0, 0, 1, 1));
    renderer.setOverlayBounds(1000, 1000, 1000);
    glfwSetInputMode(getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    initTexturesAndModels();
}

void VulkanRenderer::renderFrame(World* world) {
    if(world != nullptr) {
        renderer.getXRotation() = world->getPlayer()->getXRotation();
        renderer.getYRotation() = -world->getPlayer()->getYRotation();

        renderer.getCameraPosition() = glm::vec3(-world->getPlayer()->getCameraPosition().x, world->getPlayer()->getCameraPosition().y, world->getPlayer()->getCameraPosition().z);
    }

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
    /*std::cout << "a" << std::endl;
    std::shared_ptr<Block> blockType = Blocks::dirt;

    std::string modelName = blockType->getName() + "-" + "0";

    std::vector<InstanceData> instanceDataCube;

    for(int x = 0; x < 60; ++x) {
        for(int y = 0; y < 60; ++y) {
            for(int z = 0; z < 60; ++z) {
                instanceDataCube.push_back(InstanceData({{x, y, z}}));
            }
        }
    }

    renderer.addInstancesToModel(modelName, "set1", instanceDataCube);*/

    BlockArrayData* data = world->getBlockData();

    std::map<BlockPos, LoadedChunkInfo> lChunksLocations = data->getLoadedChunkLocations();

    std::vector<int> indicesToRemove = std::vector<int>();

    int i = 0;

    for(BlockPos& chunkPos : chunkPositionsCurrentlyLoaded) {
        if(lChunksLocations.count(chunkPos) > 0) {
            lChunksLocations.at(chunkPos);
        }else {
            indicesToRemove.push_back(i);
        }
        ++i;
    }

    for(int remove : indicesToRemove) {
        chunkPositionsCurrentlyLoaded[remove] = chunkPositionsCurrentlyLoaded.back();
        chunkPositionsCurrentlyLoaded.pop_back();
    }
    
    for(std::pair<const BlockPos, LoadedChunkInfo>& lchunk : lChunksLocations) {
        if(lchunk.second.update > 0) {
            Chunk* c = data->getChunkWithBlock(lchunk.first);

            BlockPos pos = lchunk.first;
            if(std::find(chunkPositionsCurrentlyLoaded.begin(), chunkPositionsCurrentlyLoaded.end(), pos) != chunkPositionsCurrentlyLoaded.end()) {
                updateChunkData(c);
            }else {
                chunkPositionsCurrentlyLoaded.push_back(pos);
                updateChunkData(c);
            }      
        }
    }
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

void VulkanRenderer::initTexturesAndModels() {
    std::vector<std::string> texturePaths = std::vector<std::string>();

    for(std::pair<const std::string, std::shared_ptr<Block>>& nameBlockPair : Blocks::blockMap) {
        for(int i = 0; i < 6; ++i) {
            for(unsigned int meta = 0; meta < nameBlockPair.second->getNumberOfVariants(); ++meta) {
                if(std::find(texturePaths.begin(), texturePaths.end(), "src/assets/" + nameBlockPair.second->getTextureName(SideEnum(i), meta)) == texturePaths.end()) {
                    texturePaths.push_back("src/assets/" + nameBlockPair.second->getTextureName(SideEnum(i), meta));
                }
            }
        }
    }

    texturePaths.push_back("src/assets/transparent.png"); //todo: find some way for the wireframe to work with the array texture setup

    renderer.loadTextureArray("block-textures", texturePaths);
    renderer.setCurrentTextureArray("block-textures");

    int wireframeID = renderer.getTextureArrayID("block-textures", "src/assets/transparent.png"); //todo: find some way for the wireframe to work with the array texture setup

    for(std::pair<const std::string, std::shared_ptr<Block>>& nameBlockPair : Blocks::blockMap) {
        for(unsigned int meta = 0; meta < nameBlockPair.second->getNumberOfVariants(); ++meta) {
            std::vector<Vertex> modelVertices;

            BlockRenderedModel model = nameBlockPair.second->getRenderedModel(meta);

            for(BlockFace face : model.renderedBlockModel) {
                int texID = renderer.getTextureArrayID("block-textures", "src/assets/"+nameBlockPair.second->getTextureName(face.side, meta));

                for(RenderedTriangle triangle : face.triangles) {
                    Vertex v1;
                    v1.position = glm::vec3(triangle.a.x, triangle.a.y, triangle.a.z);
                    v1.color = glm::vec4(1, 1, 1, 1);
                    v1.texCoord = glm::vec3(triangle.a.u, triangle.a.v, texID);

                    Vertex v2;
                    v2.position = glm::vec3(triangle.b.x, triangle.b.y, triangle.b.z);
                    v2.color = glm::vec4(1, 1, 1, 1);
                    v2.texCoord = glm::vec3(triangle.b.u, triangle.b.v, texID);

                    Vertex v3;
                    v3.position = glm::vec3(triangle.c.x, triangle.c.y, triangle.c.z);
                    v3.color = glm::vec4(1, 1, 1, 1);
                    v3.texCoord = glm::vec3(triangle.c.u, triangle.c.v, texID);

                    
                    modelVertices.push_back(v1);
                    modelVertices.push_back(v2);
                    modelVertices.push_back(v3);
                }
            }
            renderer.setModel(nameBlockPair.second->getName() + "-" + std::to_string(meta), modelVertices);

            for(Vertex& v : modelVertices) {
                v.texCoord[2] = wireframeID;
            }

            renderer.setModel(nameBlockPair.second->getName() + "-" + std::to_string(meta) + "-wireframe", modelVertices);
        }
    }
}

void VulkanRenderer::updateChunkData(Chunk* chunk) {
    std::string setName = std::to_string(chunk->getChunkCoordinates().x) + "-" + std::to_string(chunk->getChunkCoordinates().y) + "-" + std::to_string(chunk->getChunkCoordinates().z);

    std::vector<BlockData> chunkData = chunk->getBlocksInChunk();
    std::map<std::string, std::vector<InstanceData>> blockTypesToInstanceVectors;

    for(BlockData& block : chunkData) {
        std::string typeString = block.getBlockType()->getName() + "-" + std::to_string(block.getData());
        blockTypesToInstanceVectors[typeString].push_back(InstanceData({{-1-block.getPos().x, block.getPos().y, block.getPos().z}}));
    }

    for(std::pair<const std::string, std::vector<InstanceData>>& instanceSet : blockTypesToInstanceVectors) {
        renderer.addInstancesToModel(instanceSet.first, setName, instanceSet.second);
    }
}

#endif