#include "Player.h"
#include "World.h"
#include <iostream>
#include "Events.h"
#include <math.h>
#include "Blocks.h"
#include <thread> 

Player::Player(World* _world) : Entity(), world(_world), settings(_world->getSettings()), gui(nullptr) {
    world->getTimerMap()->addTimerToMap("itemUseTimer");
    pos.y = 100;
}

Player::~Player() {
    if(gui != nullptr) {
        gui->close();
    }
}

void Player::updateEntity(World* world) {  

    updateHorizontalMotion();

    move(&motion, world);

    updatePlayerLookingAt(world);

    if(isInWater(world->getBlockData())) {
        waterPhysics = true;
    }else {
        waterPhysics = false;
    }

    float nearPlaneLength = 2 * tan(90 / 2) * 0.01; //90 degrees is the FOV of the camera, 0.01 is the near plane distance
    float nearPlaneWidth =  nearPlaneLength / world->getRenderer()->getAspectRatio();

    Pos cameraPosition = getCameraPosition();

    AABB cameraABBB = AABB(cameraPosition.x - nearPlaneLength / 2.0, cameraPosition.y - nearPlaneWidth / 2.0, cameraPosition.z,
                            nearPlaneLength, nearPlaneWidth, 0.001);

    if(world->getBlockData()->isAABBInWater(cameraABBB)) {
        world->getRenderer()->setWaterTint(true);
    }else {
        world->getRenderer()->setWaterTint(false);
    }

    if(!isBlockUnderEntity(world)) {
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

double Player::getXRotation() {
    return yaw;
}
double Player::getYRotation() {
    return pitch;
}

bool Player::isThirdPerson() {
    return thirdPerson;
}

Pos Player::getCameraPosition() {
    glm::vec3 nonRotatedPos = glm::vec3(getPos().x + getAABB().xSize / 2, getPos().y + getAABB().ySize * 3.0 / 4.0, getPos().z + getAABB().zSize / 2);
    glm::vec3 rotatedPos = (VulkanRenderer::calculateXRotationMatrix(-getXRotation()) * glm::vec3(0, 0, 0));
    
    return Pos(rotatedPos.x + nonRotatedPos.x, rotatedPos.y + nonRotatedPos.y + ((isThirdPerson()) ? 2 : 0), rotatedPos.z + nonRotatedPos.z + ((isThirdPerson()) ? -2 : 0));
}

void Player::updatePlayerLookingAt(World* world) {
    Pos cameraPosition = getCameraPosition();
    Pos cameraNormal = getCameraNormal();

    float previousT = -1;

    std::vector<Chunk*> chunksCrossed = std::vector<Chunk*>();

    for(const std::pair<const BlockPos, LoadedChunkInfo>& pair : world->getBlockData()->getLoadedChunkLocations()) {
        Chunk* chunk = world->getBlockData()->getChunkWithBlock(pair.first);
        if(!chunk->isFakeChunk()) {
            AABB aabb = chunk->getChunkAABB();

            SideEnum sideIntersect = NORTH;

            float t = raycast(aabb, &sideIntersect, cameraPosition, cameraNormal);

            if(t != -1 && (t < previousT || previousT == -1)) {
                chunksCrossed.push_back(chunk);
            }
        }
    }
    
    std::vector<float> tValues = std::vector<float>();
    std::vector<SideEnum> sideValues = std::vector<SideEnum>();
    std::vector<BlockPos> blockValues = std::vector<BlockPos>();

    std::function<bool(AABB, bool, std::optional<SBDA>*)> eval = [this, &tValues, &sideValues, &blockValues, &cameraPosition, &cameraNormal](AABB aabb, bool isLeaf, std::optional<SBDA>* block) -> bool {
        SideEnum sideIntersect = NORTH;

        float t = raycast(aabb, &sideIntersect, cameraPosition, cameraNormal);

        if(isLeaf) {
            std::array<BlockData, 256>& blockArray = block->value().array;

            if(t != -1) {
                for(int i = 0; i < 256; ++i) {
                    BlockData& block = blockArray.at(i);
                    
                    if((block.isSolid())) {
                        AABB aabbPresice = block.getAABB();
                        float tPresice = raycast(aabbPresice, &sideIntersect, cameraPosition, cameraNormal);
                        float max = (tValues.size() > 1) ? tValues.at(tValues.size() - 1) : tPresice + 1;
                        if(tPresice != -1 && tPresice < max && tPresice <= 5) {
                            tValues.push_back(tPresice);
                            sideValues.push_back(sideIntersect);
                            blockValues.push_back(block.getPos());
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

    for(int j = 0; j < chunksCrossed.size(); ++j) {
        auto tree = chunksCrossed.at(j)->getBlockTree();
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

    glm::vec3 n1 = VulkanRenderer::calculateYRotationMatrix(-getYRotation()) * d;

    glm::vec3 n2 = VulkanRenderer::calculateXRotationMatrix(-getXRotation()) * n1;

    return Pos(n2[0], n2[1], n2[2]);
}

float min(float a, float b) {
    if(a < b) {
        return a;
    }else {
        return b;
    }
}

float max(float a, float b) {
    if(a > b) {
        return a;
    }else {
        return b;
    }
}

float Player::raycast(AABB aabb, SideEnum* side, Pos cameraPos, Pos cameraNormal) {
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

    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    
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

void Player::displayGui(VulkanRenderer* renderer) {
    if(gui != nullptr) {
        gui->displayGui(renderer, mouseX, mouseY);
    }
}

void Player::processInput(std::string event, std::string key, std::shared_ptr<Event> e) {
    if(event == "KEYPRESSED" || key == "") {
        if(gui == nullptr) {
            if(e->getEventID() == settings->getSetting(SLOT1)->getSettingState() || key == settings->getSetting(SLOT1)->getSettingState()) {
                setItemInHandIndex(0);
            }

            if(e->getEventID() == settings->getSetting(SLOT2)->getSettingState() || key == settings->getSetting(SLOT2)->getSettingState()) {
                setItemInHandIndex(1);
            }

            if(e->getEventID() == settings->getSetting(SLOT3)->getSettingState() || key == settings->getSetting(SLOT3)->getSettingState()) {
                setItemInHandIndex(2);
            }

            if(e->getEventID() == settings->getSetting(SLOT4)->getSettingState() || key == settings->getSetting(SLOT4)->getSettingState()) {
                setItemInHandIndex(3);
            }

            if(e->getEventID() == settings->getSetting(SLOT5)->getSettingState() || key == settings->getSetting(SLOT5)->getSettingState()) {
                setItemInHandIndex(4);
            }

            if(e->getEventID() == settings->getSetting(SLOT6)->getSettingState() || key == settings->getSetting(SLOT6)->getSettingState()) {
                setItemInHandIndex(5);
            }

            if(e->getEventID() == settings->getSetting(SLOT7)->getSettingState() || key == settings->getSetting(SLOT7)->getSettingState()) {
                setItemInHandIndex(6);
            }

            if(e->getEventID() == settings->getSetting(SLOT8)->getSettingState() || key == settings->getSetting(SLOT8)->getSettingState()) {
                setItemInHandIndex(7);
            }

            if(e->getEventID() == settings->getSetting(SLOT9)->getSettingState() || key == settings->getSetting(SLOT9)->getSettingState()) {
                setItemInHandIndex(8);
            }

            if(e->getEventID() == settings->getSetting(USE_SECOND)->getSettingState() || key == settings->getSetting(USE_SECOND)->getSettingState()) {
                inventory[itemInHandIndex].onUse(world);
                world->getTimerMap()->resetTimer("itemUseTimer");
            }
        }

        if(e->getEventID() == settings->getSetting(INVENTORY)->getSettingState() || key == settings->getSetting(INVENTORY)->getSettingState()) {
            if(gui == nullptr) {
                glfwSetInputMode(world->getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
                gui = std::make_unique<InventoryGui>(world->getRenderer(), &inventory);
            }else if(gui->getID() == 0) {  
                glfwSetInputMode(world->getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
                gui->close();
                gui = nullptr;
            }
        }

        if(e->getEventID() == settings->getSetting(ATTACK)->getSettingState() || key == settings->getSetting(ATTACK)->getSettingState()) {
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

        if(e->getEventID() == settings->getSetting(USE)->getSettingState() || key == settings->getSetting(USE)->getSettingState()) {
            if(gui == nullptr) {
                inventory[itemInHandIndex].onUse(world);
                world->getTimerMap()->resetTimer("itemUseTimer");
            }
        }
    }
    
    if(event == "KEYHELD" || key == "") {        
        if(gui == nullptr) {
            if(key == settings->getSetting(MOVE_FORWARD)->getSettingState()) {
                ++zInputDirection;
            }

            if(key == settings->getSetting(MOVE_LEFT)->getSettingState()) {
                --xInputDirection;
            }

            if(key == settings->getSetting(MOVE_BACK)->getSettingState()) {
                --zInputDirection;
            }

            if(key == settings->getSetting(MOVE_RIGHT)->getSettingState()) {
                ++xInputDirection;
            }

            if(key == settings->getSetting(USE_SECOND)->getSettingState() || event == settings->getSetting(USE_SECOND)->getSettingState()) {
                if(std::chrono::duration_cast<std::chrono::milliseconds>(world->getTimerMap()->getTimerDuration("itemUseTimer")).count() > 200) {
                    inventory[itemInHandIndex].onUse(world);
                    world->getTimerMap()->resetTimer("itemUseTimer");
                }
            }

            if(key == settings->getSetting(JUMP)->getSettingState() || event == settings->getSetting(JUMP)->getSettingState()) {
                if(isGrounded || canJumpInWater(world->getBlockData())) {
                    isJumping = true;
                    motion[1] = 0.21 * ((waterPhysics) ? 0.47 : 1);
                }
            }

            if(key == settings->getSetting(SNEAK)->getSettingState()) {
                isSneaking = true;
            }

            if(key == settings->getSetting(SPRINT)->getSettingState()) {
                isSprinting = true;
            }
        }
    }

    if(event == "KEYRELEASED") {
        if(gui == nullptr) {
            if(key == settings->getSetting(SNEAK)->getSettingState()) {
                isSneaking = false;
            }

            if(key == settings->getSetting(SPRINT)->getSettingState()) {
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

SideEnum Player::horizontalSidePlacedOn() {
    switch((int)floor((double)(yaw * 4.0 / 360.0) + 0.5) & 3) {
        case 0:
            return SOUTH;
        case 1:
            return WEST;
        case 2:
            return NORTH;
        case 3:
            return EAST;
        default:
            return NEUTRAL;
    }
}

SideEnum Player::sideLookingAt() {
    return sideOfBlockLookingAt;
}

void Player::closeGui(VulkanRenderer* renderer) {
    if(gui != nullptr) {
        gui->close();
    }
}