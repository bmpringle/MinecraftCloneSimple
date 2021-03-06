#include "World.h"
#include <iostream>
#include "Blocks.h"
#include <thread>
#include "PlatformFilesystem.h"
#include "ItemBlockDoor.h"

/**
 * @brief 
 * This will eventually be taken out and replaced with a 
 * different system, since the world size should eventually 
 * be more than 100x100x100. But for now this is here to be
 * used instead of 100 in order to make it easier
 * to replace later and to make the current code clearer.
 */
#define WORLDSIZE_CONST 100

World::World(GLFWwindow* window_, EventQueue* queue, InputHandler* inputHandler, Renderer* renderer, TimerMap* map, GameSettings* settings, std::string _name, std::string worldFolder, int seed) : name(_name), timerMap(map), worldEventQueue(queue), input(inputHandler), renderer(renderer), window(window_), settings(settings), internalBlockData(BlockArrayData(WORLDSIZE_CONST, WORLDSIZE_CONST, WORLDSIZE_CONST, worldFolder, seed)), thePlayer(std::make_shared<Player>(this)) {
    thePlayer->setBufferedChunkLocation(getBlockData()->getChunkWithBlock(thePlayer->getPos().toBlockPos())->getChunkCoordinates());
    internalBlockData.updateLoadedChunks(getBlockData()->getChunkWithBlock(thePlayer->getPos().toBlockPos())->getChunkCoordinates(), this);
    renderer->updateWorldVBO(this);

    thePlayer->getInventory()->setItemStackInSlot(0, ItemStack(std::make_shared<ItemBlock>(Blocks::dirt), 64));
    thePlayer->getInventory()->setItemStackInSlot(1, ItemStack(std::make_shared<ItemBlock>(Blocks::cobblestone), 64));
    thePlayer->getInventory()->setItemStackInSlot(2, ItemStack(std::make_shared<ItemBlock>(Blocks::grass), 64));
    thePlayer->getInventory()->setItemStackInSlot(3, ItemStack(std::make_shared<ItemBlock>(Blocks::leaf), 64));
    thePlayer->getInventory()->setItemStackInSlot(4, ItemStack(std::make_shared<ItemBlock>(Blocks::log), 64));
    thePlayer->getInventory()->setItemStackInSlot(5, ItemStack(std::make_shared<ItemBlock>(Blocks::water), 64));
    thePlayer->getInventory()->setItemStackInSlot(6, ItemStack(std::make_shared<ItemBlock>(Blocks::planks), 64));
    thePlayer->getInventory()->setItemStackInSlot(7, ItemStack(std::make_shared<ItemBlockDoor>(), 64));

    internalBlockData.getChunkWithBlock(BlockPos(0, 0, 0))->addEntityAtPositionOfType(std::make_shared<Entity>(), Pos(0, 46, 0));
}

void World::updateGame() {
    input->callRegularEvents(worldEventQueue, timerMap);
    
    thePlayer->updateEntity(this);

    for(Chunk& chunk : internalBlockData.getChunkArrayReference()) {
        std::map<int, std::shared_ptr<Entity>> entityMap = chunk.getEntitiesInChunk();

        for(std::pair<const int, std::shared_ptr<Entity>>& entity : entityMap) {
            entity.second->updateEntity(this);
        }
    }
}

void World::generateWorld() {

}

void World::mainLoop() {
    glfwSwapInterval(1);
    glClearColor(0, 0, 1, 1);

    worldEventQueue->addEventListener(thePlayer);

    auto previous = std::chrono::high_resolution_clock::now();
    double lag = 0.0;
    const double MS_PER_UPDATE = 16.666;
    
    while (!glfwWindowShouldClose(window) && !paused && !quitting) {
        auto current = std::chrono::high_resolution_clock::now();
        double elapsed = (std::chrono::duration_cast<std::chrono::milliseconds>(current - previous)).count();
        previous = current;
        lag += elapsed;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        while (lag >= MS_PER_UPDATE) {
            updateGame();
            lag -= MS_PER_UPDATE;
        }

        renderGame();
        glfwSwapBuffers(window);
        glfwPollEvents();

        #if DBG
        dumpFrameTime();
        #endif
    }
    
    worldEventQueue->removeEventListener(thePlayer);

    std::vector<Chunk>& chunks = internalBlockData.getChunkArrayReference();
    if(!fs::exists("./worlds/"+name+"/data/")) {
        fs::create_directories("./worlds/"+name+"/data/");
    }
    for(Chunk& c : chunks) {
        auto tree = c.getBlockTree();
        std::string data = tree->serialize();
        std::string dataname = std::to_string(c.getChunkCoordinates().x / Chunk::getChunkSize()[0]) + "-" + std::to_string(c.getChunkCoordinates().z / Chunk::getChunkSize()[2]);
        std::ofstream chunkFile("./worlds/"+name+"/data/"+dataname+".cdat");
        chunkFile.write(data.c_str(), data.length());
    }
    std::string seed = std::to_string(internalBlockData.getSeed());
    std::ofstream chunkFile("./worlds/"+name+"/data/seed.cdat");
    chunkFile.write(seed.c_str(), seed.length());

    //cleanup renderer
    renderer->cleanup();
}

void World::pause() {
    paused = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
}

void World::resume() {
    paused = false;
    timerMap->resetAllTimers();
    input->setFirstMouse();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    mainLoop();
}

bool World::isPaused() {
    return paused;
}

BlockArrayData* World::getBlockData() {
    return &internalBlockData;
}

float World::getWorldGravity() {
    return worldGravity;
}

bool rangesOverlap(float startA, float endA, float startB, float endB) {
    return (startA < endB && endA > startB);
}

bool AABBIntersectedByAABB(AABB box1, AABB box2) {
    if(rangesOverlap(box1.startX, box1.startX + box1.xSize, box2.startX, box2.startX + box2.xSize)) {
        if(rangesOverlap(box1.startY, box1.startY + box1.ySize, box2.startY, box2.startY + box2.ySize)) {
            if(rangesOverlap(box1.startZ, box1.startZ + box1.zSize, box2.startZ, box2.startZ + box2.zSize)) {
                return true;
            }       
        }       
    }
    return false;
} 

bool AABBIntersectedByAABBVocal(AABB box1, AABB box2) {
    if(rangesOverlap(box1.startX, box1.startX + box1.xSize, box2.startX, box2.startX + box2.xSize)) {
        if(rangesOverlap(box1.startY, box1.startY + box1.ySize, box2.startY, box2.startY + box2.ySize)) {
            if(rangesOverlap(box1.startZ, box1.startZ + box1.zSize, box2.startZ, box2.startZ + box2.zSize)) {
                return true;
            }else {
                std::cout << "failedzcheck" << std::endl;
            }    
        }else {
            std::cout << "failedycheck" << std::endl;
        }      
    }else {
        std::cout << "failedxcheck" << std::endl;
    }
    return false;
} 

void World::renderGame() {
    if(internalBlockData.shouldUpdateRenderer()) {
        renderer->updateWorldVBO(this);
        internalBlockData.sendChunkUpdates();
        internalBlockData.hasUpdatedRenderer();
    }
    renderer->updateAspectRatio(window);
    renderer->renderFrame(this);
    if(thePlayer->getBlockLookingAt() != nullptr) {
        if(internalBlockData.getBlockAtPosition(*thePlayer->getBlockLookingAt()).getBlockType() != nullptr) { //this is needed to fix some whack bug on windows :/
            renderer->renderBlockInWireframe(this, *thePlayer->getBlockLookingAt());
        }
    }

    glClear(GL_DEPTH_BUFFER_BIT);
    renderOverlays();
    glClear(GL_DEPTH_BUFFER_BIT);
}

void World::renderOverlays() {

    float overlay[48] = {
        -11, -11, 0, 0, 0, 1, 0, 0,
        11, -11, 0, 0, 0, 1, 1, 0,
        -11, 11, 0, 0, 0, 1, 0, 1,

        -11, 11, 0, 0, 0, 1, 0, 1,
        11, -11, 0, 0, 0, 1, 1, 0,
        11, 11, 0, 0, 0, 1, 1, 1
    };

    renderer->renderOverlay(overlay, "crosshair.png");

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float xSize = 1200;
    float ySize = 227 * (float)height/(float)width;

    float xPos = -600;
    float yPos = -999 * (float)height/(float)width;

    float overlay2[48] = {
        xPos, yPos, 0, 0, 0, 1, 0, 0,
        xPos + xSize, yPos, 0, 0, 0, 1, 1, 0,
        xPos, yPos + ySize, 0, 0, 0, 1, 0, 1,

        xPos, yPos + ySize, 0, 0, 0, 1, 0, 1,
        xPos + xSize, yPos, 0, 0, 0, 1, 1, 0,
        xPos + xSize, yPos + ySize, 0, 0, 0, 1, 1, 1
    };

    renderer->renderOverlay(overlay2, "hotbar.png");

    Inventory* inv = thePlayer->getInventory();

    for(int i = 0; i < 9; ++i) {
        ItemStack stack = inv->getItemStackInSlot(i);

        if(stack.getItem() != nullptr && stack.getCount() > 0) {
            float sizeX = 16;
            float startX = (2 * (i+1) + (i) * (sizeX) + i) * xSize / 172.0f;
            float endX = (2 * (i+1) + (i + 1) * (sizeX) + i) * xSize / 172.0f;

            float startY = 2 * ySize / 20.0f;
            float endY = (2 + sizeX) * ySize / 20.0f;

            float iconoverlay[48] = {
                xPos + startX, yPos + startY, -1, 0, 0, 1, 0, 0,
                xPos + endX, yPos + startY, -1, 0, 0, 1, 1, 0,
                xPos + startX, yPos + endY, -1, 0, 0, 1, 0, 1,

                xPos + startX, yPos + endY, -1, 0, 0, 1, 0, 1,
                xPos + endX, yPos + startY, -1, 0, 0, 1, 1, 0,
                xPos + endX, yPos + endY, -1, 0, 0, 1, 1, 1
            };

            renderer->renderOverlay(iconoverlay, stack.getItem()->getIcon());
        }

        if(thePlayer->getItemInHandIndex() == i) {
            float sizeX = 16;
            float startX = (2 * (i + 1) + (i) * (sizeX) + i - 2) * xSize / 172.0f;
            float endX = (2 * (i + 1) + (i + 1) * (sizeX) + i + 2) * xSize / 172.0f;

            float startY = (2 - 2) * ySize / 20.0f;
            float endY = (2 + sizeX + 2) * ySize / 20.0f;

            float iconoverlay[48] = {
                xPos + startX, yPos + startY, -2, 0, 0, 1, 0, 0,
                xPos + endX, yPos + startY, -2, 0, 0, 1, 1, 0,
                xPos + startX, yPos + endY, -2, 0, 0, 1, 0, 1,

                xPos + startX, yPos + endY, -2, 0, 0, 1, 0, 1,
                xPos + endX, yPos + startY, -2, 0, 0, 1, 1, 0,
                xPos + endX, yPos + endY, -2, 0, 0, 1, 1, 1
            };

            renderer->renderOverlay(iconoverlay, "hotbar_select.png");
        }
    }
    thePlayer->displayGui(renderer);
}

std::shared_ptr<Player> World::getPlayer() {
    return thePlayer;
}

TimerMap* World::getTimerMap() {
    return timerMap;
}

int World::getChunkRenderDistance() {
    return chunkRenderDistance;
}

void World::dumpFrameTime() { 
    float frameTime = ((float)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - prevFrameTimePoint).count()) / 1000;
    frameTimeSum += frameTime;
    ++frameTimeCounter;

    if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - secondTimer).count() >= 1000) {
        std::cout << "frameTime avg: " << (frameTimeSum / frameTimeCounter) << std::endl;
        secondTimer = std::chrono::high_resolution_clock::now();
        frameTimeSum = 0;
        frameTimeCounter = 0;
    }
    prevFrameTimePoint = std::chrono::high_resolution_clock::now();
}

GLFWwindow* World::getWindowPtr() {
    return window;
}

Renderer* World::getRenderer() {
    return renderer;
}

void World::quit() {
    pause();
    quitting = true;
}

GameSettings* World::getSettings() {
    return settings;
}

std::string World::getName() {
    return name;
}
