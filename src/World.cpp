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

    auto func2 = [](GLFWwindow* w, double xpos, double ypos) {
        static_cast<World*>(glfwGetWindowUserPointer(w))->internalMouseCallback(w, xpos, ypos);
    };

    glfwSetKeyCallback(window, func);
    glfwSetCursorPosCallback(window, func2);
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
    for(int x = 0; x < 20; ++x) {
        for(int y = 0; y < 1; ++y) {
            for(int z = 0; z < 20; ++z) {
                internalBlockData.setBlockAtPosition(BlockPos(x, y, z), std::shared_ptr<Block>(new Block()));
            }
        } 
    }

    internalBlockData.setBlockAtPosition(BlockPos(0, 1, 10), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(0, 2, 10), std::shared_ptr<Block>(new Block()));

    internalBlockData.setBlockAtPosition(BlockPos(4, 1, 10), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(5, 2, 10), std::shared_ptr<Block>(new Block()));

    internalBlockData.setBlockAtPosition(BlockPos(0, 1, 10), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(7, 2, 10), std::shared_ptr<Block>(new Block()));

    internalBlockData.setBlockAtPosition(BlockPos(15, 3, 2), std::shared_ptr<Block>(new Block()));
    internalBlockData.setBlockAtPosition(BlockPos(15, 3, 2), std::shared_ptr<Block>(new Block()));
}

void World::internalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    input->handleInput(window, key, scancode, action, mods, worldEventQueue, &timerMap);
}

void World::internalMouseCallback(GLFWwindow* window, double xpos, double ypos) {
    input->handleMouseInput(window, xpos, ypos, worldEventQueue, &timerMap);
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

void World::renderGame() {
    renderer.renderFrame(this);
}

std::shared_ptr<Player> World::getPlayer() {
    return thePlayer;
}

TimerMap* World::getTimerMap() {
    return &timerMap;
}