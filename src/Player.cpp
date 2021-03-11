#include "Player.h"
#include "World.h"
#include <iostream>
#include "Events.h"
#include <math.h>
#include "Blocks.h"
#include <thread> 

Player::Player(World* _world) : world(_world), settings(_world->getSettings()), pos(Pos(0, 40, 0)), bufferedChunkLocation(BlockPos(0, 0, 0)), gui(nullptr) {
    world->getTimerMap()->addTimerToMap("playerUpdateTimer");
    world->getTimerMap()->addTimerToMap("itemUseTimer");
}

void updatePlayerInWorld(World* _world) {

}

void Player::updateClient(World* world) {  
    //handle space bar and gravity from events
    /*
     *long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(world->getTimerMap()->getTimerDurationAndReset("playerUpdateTimer")).count();
    */

    updateHorizontalMotion();

    move(&motion);

    updatePlayerLookingAt(world);
}

void Player::updateServer(World* _world) {
    /* 
     * Not needed right now:
     * long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(world->getTimerMap()->getTimerDuration("playerUpdateTimer")).count();
     */
    if(isInWater(_world->getBlockData())) {
        waterPhysics = true;
    }else {
        waterPhysics = false;
    }

    if(!isBlockUnderPlayer()) {
        motion[1] -= (0.086 / 6) * ((waterPhysics) ? 0.5 : 1);
        motion[1] *= 0.98;
    }

    if(abs(motion[0]) < 0.005) {
        motion[0] = 0;
    }else {
        motion[0] *= 0.9998;
    }

    if(abs(motion[2]) < 0.005) {
        motion[2] = 0;
    }else {
        motion[2] *= 0.98;
    }

    if(bufferedChunkLocation != world->getBlockData()->getChunkWithBlock(getPos().toBlockPos())->getChunkCoordinates() && !world->getBlockData()->getChunkWithBlock(getPos().toBlockPos())->isFakeChunk()) {
        setBufferedChunkLocation(world->getBlockData()->getChunkWithBlock(getPos().toBlockPos())->getChunkCoordinates());
        world->getBlockData()->updateLoadedChunks(bufferedChunkLocation, world);
    }else if(!world->getBlockData()->getChunkWithBlock(getPos().toBlockPos())->isFakeChunk()){
        setBufferedChunkLocation(world->getBlockData()->getChunkWithBlock(getPos().toBlockPos())->getChunkCoordinates());
    }
}

//todo -> speed up validation checks and lookat checks
void Player::listenTo(std::shared_ptr<Event> e) {
    std::string key = "";

    if(e->getEventID() == "KEYPRESSED") {
        KeyPressedEvent event = *dynamic_cast<KeyPressedEvent*>(e.get());
        key = event.key;
    }

    if(e->getEventID() == "KEYHELD") {
        KeyHeldEvent event = *dynamic_cast<KeyHeldEvent*>(e.get());
        key = event.key;
    }

    if(e->getEventID() == "KEYRELEASED") {
        KeyReleasedEvent event = *dynamic_cast<KeyReleasedEvent*>(e.get());
        key = event.key;
    }
    processInput(e->getEventID(), key, e);
}

AABB Player::getAABB() {
    return AABB(pos.x, pos.y, pos.z, 0.6, (isSneaking) ? sneakingHeight : standingHeight, 0.6);
}

RenderedModel Player::getRenderedModel() {
    RenderedPoint p1 = RenderedPoint(0, 0, 0, /**uv coords*/ 0, 0);
    RenderedPoint p2 = RenderedPoint(0.6, 0, 0, /**uv coords*/ 0, 0);
    RenderedPoint p3 = RenderedPoint(0.6, 0, 0.6, /**uv coords*/ 0, 0);
    RenderedPoint p4 = RenderedPoint(0, 0, 0.6, /**uv coords*/ 0, 0);
    RenderedPoint p5 = RenderedPoint(0, 1.8, 0, /**uv coords*/ 0, 0);
    RenderedPoint p6 = RenderedPoint(0.6, 1.8, 0, /**uv coords*/ 0, 0);
    RenderedPoint p7 = RenderedPoint(0.6, 1.8, 0.6, /**uv coords*/ 0, 0);
    RenderedPoint p8 = RenderedPoint(0, 2, 0.6, /**uv coords*/ 0, 0);

    RenderedTriangle t1 = RenderedTriangle(p1, p2, p3, 1);
    RenderedTriangle t2 = RenderedTriangle(p1, p4, p3, 0);
    RenderedTriangle t3 = RenderedTriangle(p5, p6, p7, 1);
    RenderedTriangle t4 = RenderedTriangle(p5, p8, p7, 0);
    RenderedTriangle t5 = RenderedTriangle(p1, p4, p8, 1);
    RenderedTriangle t6 = RenderedTriangle(p1, p5, p8, 1);
    RenderedTriangle t7 = RenderedTriangle(p2, p3, p7, 0);
    RenderedTriangle t8 = RenderedTriangle(p2, p6, p7, 1);
    RenderedTriangle t9 = RenderedTriangle(p1, p2, p5, 0);
    RenderedTriangle t10 = RenderedTriangle(p2, p5, p6, 1);
    RenderedTriangle t11 = RenderedTriangle(p3, p4, p8, 0);
    RenderedTriangle t12 = RenderedTriangle(p3, p7, p8, 1);

    RenderedTriangle triangleArray[12] = {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12};
    std::vector<RenderedTriangle> triangles = std::vector<RenderedTriangle>();

    for(RenderedTriangle triangle : triangleArray) {
        triangles.push_back(triangle);
    }

    return RenderedModel(triangles);
}

Pos Player::getPos() {
    return pos;
}

double Player::getXRotation() {
    return yaw;
}
double Player::getYRotation() {
    return pitch;
}

bool Player::validatePosition(Pos newPosition, BlockArrayData* data) {
    AABB playerAABB = getAABB();
    float a;
    return data->isValidPosition(playerAABB, &a);
}

bool Player::validatePosition(Pos newPosition, BlockArrayData* data, float* yToSnapTo) {
    AABB playerAABB = getAABB();
    return data->isValidPosition(playerAABB, yToSnapTo);
}

Pos Player::getCameraPosition() {
    glm::vec3 nonRotatedPos = glm::vec3(getPos().x + getAABB().xSize / 2, getPos().y + getAABB().ySize * 3.0 / 4.0, getPos().z + getAABB().zSize / 2);
    glm::vec3 rotatedPos = (Renderer::calculateXRotationMatrix(-getXRotation()) * glm::vec3(0, 0, 0));
    
    return Pos(rotatedPos.x + nonRotatedPos.x, rotatedPos.y + nonRotatedPos.y, rotatedPos.z + nonRotatedPos.z);
}

void Player::updatePlayerLookingAt(World* world) {
    float previousT = -1;

    std::vector<Chunk*> chunksCrossed = std::vector<Chunk*>();

    for(const std::pair<const BlockPos, LoadedChunkInfo>& pair : world->getBlockData()->getLoadedChunkLocations()) {
        Chunk* chunk = world->getBlockData()->getChunkWithBlock(pair.first);
        if(!chunk->isFakeChunk()) {
            AABB aabb = chunk->getChunkAABB();

            SideEnum sideIntersect = NORTH;

            float t = raycast(aabb, &sideIntersect);

            if(t != -1 && (t < previousT || previousT == -1)) {
                chunksCrossed.push_back(chunk);
            }
        }
    }

    std::vector<float> tValues = std::vector<float>();
    std::vector<SideEnum> sideValues = std::vector<SideEnum>();
    std::vector<BlockPos> blockValues = std::vector<BlockPos>();

    for(int j = 0; j < chunksCrossed.size(); ++j) {
        auto tree = chunksCrossed.at(j)->getBlockTree();

        std::function<bool(AABB, bool, std::optional<SBDA>)> eval = [this, &tValues, &sideValues, &blockValues](AABB aabb, bool isLeaf, std::optional<SBDA> block) -> bool { 
            SideEnum sideIntersect = NORTH;

            float t = raycast(aabb, &sideIntersect);

            if(isLeaf) {
                if(t != -1) {
                    for(int i = 0; i < 256; ++i) {
                        AABB aabbPresice = AABB(aabb.startX, i, aabb.startZ, 1, 1, 1);
                        if(block.value().array.at(i).getBlockType() != nullptr && (block.value().array.at(i).isSolid())) {
                            float tPresice = raycast(aabbPresice, &sideIntersect);
                            float max = (tValues.size() > 1) ? tValues.at(tValues.size() - 1) : tPresice + 1;
                            if(tPresice != -1 && tPresice < max && tPresice <= 5) {
                                tValues.push_back(tPresice);
                                sideValues.push_back(sideIntersect);
                                blockValues.push_back(BlockPos(aabbPresice.startX, aabbPresice.startY, aabbPresice.startZ));
                            }
                        }
                    }
                }
            }else {
                if(t != -1) {
                    return true;
                }
            }
            return false;
        };
        tree->getLeafOfTree(eval);
    }

    if(tValues.size() > 0) {
        internalBlockLookingAt = blockValues.at(blockValues.size() - 1);
        blockLookingAt = &internalBlockLookingAt;
        sideOfBlockLookingAt = sideValues.at(sideValues.size() - 1);
    }else {
        blockLookingAt = nullptr;
        sideOfBlockLookingAt = NORTH;        
    }
}

BlockPos* Player::getBlockLookingAt() {
    return blockLookingAt;
}

Pos Player::getCameraNormal() {
    glm::vec3 d = glm::vec3();
    d[0] = 0;
    d[1] = 0;
    d[2] = 1;

    glm::vec3 n1 = Renderer::calculateYRotationMatrix(-getYRotation()) * d;

    glm::vec3 n2 = Renderer::calculateXRotationMatrix(-getXRotation()) * n1;

    return Pos(n2[0], n2[1], n2[2]);
}

float Player::raycast(AABB aabb, SideEnum* side) {
    Pos cameraPos = getCameraPosition();
    Pos cameraNormal = getCameraNormal();
    if(cameraNormal.x == 0) {
        if(cameraPos.x < aabb.startX || cameraPos.x > aabb.startX + aabb.xSize) {
            return -1;
        }
    }

    if(cameraNormal.y == 0) {
        if(cameraPos.y < aabb.startX || cameraPos.y > aabb.startX + aabb.ySize) {
            return -1;
        }
    }

    if(cameraNormal.z == 0) {
        if(cameraPos.z < aabb.startX || cameraPos.z > aabb.startX + aabb.zSize) {
            return -1;
        }
    }

    float t1 = (aabb.startX - cameraPos.x) / cameraNormal.x;
    float t2 = (aabb.startX + aabb.xSize - cameraPos.x) / cameraNormal.x;
    float t3 = (aabb.startY - cameraPos.y) / cameraNormal.y;
    float t4 = (aabb.startY + aabb.ySize - cameraPos.y) / cameraNormal.y;
    float t5 = (aabb.startZ - cameraPos.z) / cameraNormal.z;
    float t6 = (aabb.startZ + aabb.zSize - cameraPos.z) / cameraNormal.z;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
    if (tmax < 0) {
        return -1;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax) {
        return -1;
    }

    if (tmin < 0) {
        return tmax;
    }

    if(t1 == tmin) {
        *side = WEST;
    }else if(t2 == tmin) {
        *side = EAST;
    }else if(t3 == tmin) {
        *side = DOWN;
    }else if(t4 == tmin) {
        *side = UP;
    }else if(t5 == tmin) {
        *side = SOUTH;
    }else if(t6 == tmin) {
        *side = NORTH;
    }
    return tmin;
}

SideEnum Player::getSideOfBlockLookingAt() {
    return sideOfBlockLookingAt;
}

void Player::setItemInHandIndex(int index) {
    itemInHandIndex = index;
}

void Player::setBufferedChunkLocation(BlockPos pos) {
    this->bufferedChunkLocation = pos;
}


void Player::move(glm::vec3* moveVec) {    
    double d3 = (*moveVec)[0];
    double d4 = (*moveVec)[1];
    double d5 = (*moveVec)[2];

    Pos previousPos = pos;

    pos.x += d3;

    if(!this->validatePosition(pos, world->getBlockData())) {
        pos = previousPos;
    }

    previousPos = pos;

    pos.z += d5;

    if(!this->validatePosition(pos, world->getBlockData())) {
        pos = previousPos;
    }

    previousPos = pos;

    float yPosToSnapTo[1] = {pos.y};

    pos.y += d4;

    if(!this->validatePosition(pos, world->getBlockData(), yPosToSnapTo)) {
        pos = previousPos;
        if(d4 < 0) {
            pos.y = yPosToSnapTo[0];
            isJumping = false;
        }
        (*moveVec)[1] = 0;
    }

    bool flag = isBlockUnderPlayer();

    if(!flag && isGrounded && !isJumping && isSneaking) {
        pos = sneakPos;

        (*moveVec)[1] = 0;
    }else if(!isJumping && isSneaking && flag) {
        sneakPos = pos;
    }

    flag = isBlockUnderPlayer();

    if(!flag) {
        isGrounded = false;
    }else {
        isGrounded = true;
    }
}

void Player::updateHorizontalMotion() {
    float unitX = 0;
    float unitZ = 0;

    if(zInputDirection > 0) {
        if(xInputDirection > 0) {
            unitX = sqrt(2)/2;
            unitZ = sqrt(2)/2;
        }

        if(xInputDirection == 0) {
            unitZ = 1;
        }

        if(xInputDirection < 0) {
            unitX = -sqrt(2)/2;
            unitZ = sqrt(2)/2;
        }
    }

    if(zInputDirection == 0) {
        if(xInputDirection > 0) {
            unitX = 1;
        }

        if(xInputDirection == 0) {
            //dont move at all
        }

        if(xInputDirection < 0) {
            unitX = -1;
        }
    }

    if(zInputDirection < 0) {
        if(xInputDirection > 0) {
            unitX = sqrt(2)/2;
            unitZ = -sqrt(2)/2;
        }

        if(xInputDirection == 0) {
            unitZ = -1;
        }

        if(xInputDirection < 0) {
            unitX = -sqrt(2)/2;
            unitZ = -sqrt(2)/2;            
        }
    }

    double xMovRel = (speed * unitX) / 60;
    double zMovRel = (speed * unitZ) / 60;

    double degRADS = yaw / 180.0 * M_PI;
    double xMovAbs = xMovRel * cos(degRADS) - zMovRel * sin(degRADS);
    double zMovAbs = zMovRel * cos(degRADS) + xMovRel * sin(degRADS);

    bool flag = isSneaking;

    if (flag) {
        xMovAbs = xMovAbs / 3;
        zMovAbs = zMovAbs / 3;
    }

    flag = !isSneaking && isSprinting;
    
    if(flag && isJumping && !waterPhysics) {
        xMovAbs = xMovAbs * 1.5;
        zMovAbs = zMovAbs * 1.5;
    }else if(flag) {
        xMovAbs = xMovAbs * 1.3;
        zMovAbs = zMovAbs * 1.3;
    }

    if(waterPhysics) {
        xMovAbs = xMovAbs * 0.4;
        zMovAbs = zMovAbs * 0.4;
    }

    if(isGrounded) {
        motion[0] = xMovAbs;
        motion[2] = zMovAbs;
    }else {
        if(abs(xMovAbs) > 0) {
            motion[0] = xMovAbs;
        }else {
            motion[0] *= 0.8;
        }
        
        if(abs(zMovAbs) > 0) {
            motion[2] = zMovAbs;
        }else {
            motion[2] *= 0.8;
        }
    }

    xInputDirection = 0;
    zInputDirection = 0;
}


//ahagahlkjdsflaksjf
bool Player::isBlockUnderPlayer() {
    pos.y -= 0.002;
    if(!this->validatePosition(pos, world->getBlockData())) {
        pos.y += 0.002;
        return true;
    }else {
        pos.y += 0.002;
        return false;
    }
}

bool Player::canJumpInWater(BlockArrayData* data) {
    AABB aabb = getAABB();
    aabb.startY += 0.4;
    aabb.ySize -= 0.4;
    return data->isAABBInWater(aabb);
}

bool Player::isInWater(BlockArrayData* data) {
    AABB aabb = getAABB();
    aabb.ySize += 0.05;
    return data->isAABBInWater(aabb);
}

Inventory* Player::getInventory() {
    return &inventory;
}

int Player::getItemInHandIndex() {
    return itemInHandIndex;
}

void Player::displayGui(Renderer* renderer) {
    if(gui != nullptr) {
        gui->displayGui(renderer, mouseX, mouseY);
    }
}

void Player::processInput(std::string event, std::string key, std::shared_ptr<Event> e) {
    if(event == "KEYPRESSED" || key == "") {
        if(gui == nullptr) {
            if(e->getEventID() == settings->getSetting(SLOT1) || key == settings->getSetting(SLOT1)) {
                setItemInHandIndex(0);
            }

            if(e->getEventID() == settings->getSetting(SLOT2) || key == settings->getSetting(SLOT2)) {
                setItemInHandIndex(1);
            }

            if(e->getEventID() == settings->getSetting(SLOT3) || key == settings->getSetting(SLOT3)) {
                setItemInHandIndex(2);
            }

            if(e->getEventID() == settings->getSetting(SLOT4) || key == settings->getSetting(SLOT4)) {
                setItemInHandIndex(3);
            }

            if(e->getEventID() == settings->getSetting(SLOT5) || key == settings->getSetting(SLOT5)) {
                setItemInHandIndex(4);
            }

            if(e->getEventID() == settings->getSetting(SLOT6) || key == settings->getSetting(SLOT6)) {
                setItemInHandIndex(5);
            }

            if(e->getEventID() == settings->getSetting(SLOT7) || key == settings->getSetting(SLOT7)) {
                setItemInHandIndex(6);
            }

            if(e->getEventID() == settings->getSetting(SLOT8) || key == settings->getSetting(SLOT8)) {
                setItemInHandIndex(7);
            }

            if(e->getEventID() == settings->getSetting(SLOT9) || key == settings->getSetting(SLOT9)) {
                setItemInHandIndex(8);
            }

            if(e->getEventID() == settings->getSetting(USE_SECOND) || key == settings->getSetting(USE_SECOND)) {
                inventory[itemInHandIndex].onUse(world);
                world->getTimerMap()->resetTimer("itemUseTimer");
            }
        }

        if(e->getEventID() == settings->getSetting(INVENTORY) || key == settings->getSetting(INVENTORY)) {
            if(gui == nullptr) {
                glfwSetInputMode(world->getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
                gui = std::make_unique<InventoryGui>(world->getRenderer(), &inventory);
            }else if(gui->getID() == 0) {  
                glfwSetInputMode(world->getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
                gui = nullptr;
            }
        }

        if(e->getEventID() == settings->getSetting(ATTACK) || key == settings->getSetting(ATTACK)) {
            if(gui == nullptr) {                
                inventory[itemInHandIndex].onLeftClick(world, blockLookingAt);
                if(blockLookingAt != nullptr) {
                    BlockPos selected = *blockLookingAt;
                    world->getBlockData()->removeBlockAtPosition(selected);
                }
            }else {
                gui->mouseClick(mouseX, mouseY);
            }
        }

        if(e->getEventID() == settings->getSetting(USE) || key == settings->getSetting(USE)) {
            if(gui == nullptr) {
                inventory[itemInHandIndex].onUse(world);
                world->getTimerMap()->resetTimer("itemUseTimer");
            }
        }
    }
    
    if(event == "KEYHELD" || key == "") {        
        if(gui == nullptr) {
            if(key == settings->getSetting(MOVE_FORWARD)) {
                ++zInputDirection;
            }

            if(key == settings->getSetting(MOVE_LEFT)) {
                --xInputDirection;
            }

            if(key == settings->getSetting(MOVE_BACK)) {
                --zInputDirection;
            }

            if(key == settings->getSetting(MOVE_RIGHT)) {
                ++xInputDirection;
            }

            if(key == settings->getSetting(USE_SECOND) || event == settings->getSetting(USE_SECOND)) {
                if(std::chrono::duration_cast<std::chrono::milliseconds>(world->getTimerMap()->getTimerDuration("itemUseTimer")).count() > 200) {
                    inventory[itemInHandIndex].onUse(world);
                    world->getTimerMap()->resetTimer("itemUseTimer");

                }
            }

            if(key == settings->getSetting(JUMP) || event == settings->getSetting(JUMP)) {
                if(isGrounded || canJumpInWater(world->getBlockData())) {
                    isJumping = true;
                    motion[1] = 0.21 * ((waterPhysics) ? 0.4 : 1);
                }
            }

            if(key == settings->getSetting(SNEAK)) {
                isSneaking = true;
            }

            if(key == settings->getSetting(SPRINT)) {
                isSprinting = true;
            }
        }
    }

    if(event == "KEYRELEASED") {
        if(gui == nullptr) {
            if(key == settings->getSetting(SNEAK)) {
                isSneaking = false;
            }

            if(key == settings->getSetting(SPRINT)) {
                isSprinting = false;
            }
        }
    }

    if(event == "MOUSEMOVEDOFFSET") {
        if(gui == nullptr) {
            MouseMovedOffsetEvent mouseEvent = *dynamic_cast<MouseMovedOffsetEvent*>(e.get());
            yaw -= mouseEvent.xOffset;
            pitch -= mouseEvent.yOffset;

            if(pitch > 89) {
                pitch = 89;
            }
            if(pitch < -89) {
                pitch = -89;
            }
        }
    }

    if(event == "MOUSEPOS") {
        MousePosEvent mouseEvent = *dynamic_cast<MousePosEvent*>(e.get());
        mouseX = mouseEvent.x;
        mouseY = mouseEvent.y;
    }
}
