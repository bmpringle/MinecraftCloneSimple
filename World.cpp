#include "World.h"
#include "Player.h"
#include <iostream>

/**
 * @brief 
 * This will eventually be taken out and replaced with a 
 * different system, since the world size should eventually 
 * be more than 100x100x100. But for now this is here to be
 * used instead of 100 in order to make it easier
 * to replace later and to make the current code clearer.
 */
#define WORLDSIZE_CONST 100

World::World(GLFWwindow* window_, EventQueue* queue, InputHandler* inputHandler) : worldEventQueue(queue), input(inputHandler), renderer(WorldRenderer()), thePlayer(std::shared_ptr<Player>(new Player(this))), internalBlockData(BlockArrayData(WORLDSIZE_CONST, WORLDSIZE_CONST, WORLDSIZE_CONST)), window(window_), timerMap(TimerMap()) {
    generateWorld();
    glfwSetWindowUserPointer(window, this);

    auto func = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        static_cast<World*>(glfwGetWindowUserPointer(w))->internalKeyCallback(w, key, scancode, action, mods);
    };

    glfwSetKeyCallback(window, func);
}

void World::updateGame() {
    //too slow need to impl better system for this and collision
    /*for(int x = -WORLDSIZE_CONST; x < WORLDSIZE_CONST; ++x) {
        for(int y = -WORLDSIZE_CONST; y < WORLDSIZE_CONST; ++y) {
            for(int z = -WORLDSIZE_CONST; z < WORLDSIZE_CONST; ++z) {
                internalBlockData.updateBlockAtPosition(BlockPos(x, y, z));
            }
        }
    }*/
    input->callRegularEvents(worldEventQueue, &timerMap);
    thePlayer->updatePlayerInWorld(this);
}

void World::generateWorld() {
    //generate a 20x20x3 layer of blocks for now, will change later
    /*for(int x = 0; x < 20; ++x) {
        for(int y = 0; y < 3; ++y) {
            for(int z = 0; z < 20; ++z) {
                internalBlockData.setBlockAtPosition(BlockPos(x, y, z), std::shared_ptr<Block>(new Block()));
            }
        } 
    }*/

    internalBlockData.setBlockAtPosition(BlockPos(0, 1, 10), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 2, 10), std::shared_ptr<Block>(new Block()));

    internalBlockData.setBlockAtPosition(BlockPos(0, 0, 0), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 0, 1), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 0, 2), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 0, 3), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 0, 4), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 0, 5), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 0, 6), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 0, 7), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 0, 8), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 0, 9), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 0, 10), std::shared_ptr<Block>(new Block()));

    internalBlockData.setBlockAtPosition(BlockPos(1, -1, 10), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(1, -1, 9), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(2, -1, 10), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(2, -1, 9), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(3, -1, 10), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(3, -1, 9), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(-1, -1, 10), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(-1, -1, 9), std::shared_ptr<Block>(new Block()));
}

void World::internalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    input->handleInput(window, key, scancode, action, mods, worldEventQueue, &timerMap);
}

void World::mainLoop() {
    glfwSwapInterval(1);
    glClearColor(0, 0, 1, 1);

    worldEventQueue->addEventListener(thePlayer);

    while(!glfwWindowShouldClose(window) && !paused) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        updateGame();
        renderGame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    worldEventQueue->removeEventListener(thePlayer);
}

void World::pause() {
    paused = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
}

void World::resume() {
    paused = false;
    timerMap.resetAllTimers();
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

bool AABBIntersectedByPoint(AABB box, float x, float y, float z) {
    if(box.startX < x && box.startX + box.xSize > x) {
        if(box.startY < y && box.startY + box.ySize > y) {
            if(box.startZ < z && box.startZ + box.zSize > z) {
                return true;
            }
        }   
    }
    return false;
}

bool AABBIntersectedByAABB(AABB box1, AABB box2) {
    int i = 0;

    i += (AABBIntersectedByPoint(box1, box2.startX, box2.startY, box2.startZ) ? 1 : 0);
    i += (AABBIntersectedByPoint(box1, box2.startX, box2.startY, box2.startZ + box2.zSize) ? 1 : 0);
    i += (AABBIntersectedByPoint(box1, box2.startX, box2.startY + box2.ySize, box2.startZ) ? 1 : 0);
    i += (AABBIntersectedByPoint(box1, box2.startX, box2.startY + box2.ySize, box2.startZ + box2.zSize) ? 1 : 0);
    i += (AABBIntersectedByPoint(box1, box2.startX + box2.xSize, box2.startY, box2.startZ) ? 1 : 0);
    i += (AABBIntersectedByPoint(box1, box2.startX + box2.xSize, box2.startY, box2.startZ + box2.zSize) ? 1 : 0);
    i += (AABBIntersectedByPoint(box1, box2.startX + box2.xSize, box2.startY + box2.ySize, box2.startZ) ? 1 : 0);
    i += (AABBIntersectedByPoint(box1, box2.startX + box2.xSize, box2.startY + box2.ySize, box2.startZ + box2.zSize) ? 1 : 0);

    i += (AABBIntersectedByPoint(box2, box1.startX, box1.startY, box1.startZ) ? 1 : 0);
    i += (AABBIntersectedByPoint(box2, box1.startX, box1.startY, box1.startZ + box1.zSize) ? 1 : 0);
    i += (AABBIntersectedByPoint(box2, box1.startX, box1.startY + box1.ySize, box1.startZ) ? 1 : 0);
    i += (AABBIntersectedByPoint(box2, box1.startX, box1.startY + box1.ySize, box1.startZ + box1.zSize) ? 1 : 0);
    i += (AABBIntersectedByPoint(box2, box1.startX + box1.xSize, box1.startY, box1.startZ) ? 1 : 0);
    i += (AABBIntersectedByPoint(box2, box1.startX + box1.xSize, box1.startY, box1.startZ + box1.zSize) ? 1 : 0);
    i += (AABBIntersectedByPoint(box2, box1.startX + box1.xSize, box1.startY + box1.ySize, box1.startZ) ? 1 : 0);
    i += (AABBIntersectedByPoint(box2, box1.startX + box1.xSize, box1.startY + box1.ySize, box1.startZ + box1.zSize) ? 1 : 0);

    if(i == 0) {
        return false;
    }
    return true;
} 

void World::renderGame() {
    renderer.renderFrame(this);
}

std::shared_ptr<Player> World::getPlayer() {
    return thePlayer;
}

TimerMap* World::getTimerMap() {
    return &timerMap;
}