#include "Player.h"
#include "World.h"
#include <iostream>
#include "Events.h"
#include <math.h>
#include "Blocks.h"

Player::Player(World* _world) : pos(Pos(0, 5, 0)), world(_world), bufferedChunkLocation(BlockPos(0, 0, 0)) {
    world->getTimerMap()->addTimerToMap("playerUpdateTimer");
    world->getTimerMap()->addTimerToMap("itemUseTimer");
    itemInHand = std::unique_ptr<Item>(new ItemBlock(std::shared_ptr<Block>(new BlockDirt())));
}

void Player::updateClient(World* world) {  
    Pos previousPos = pos;

    //handle space bar and gravity from events
    long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(world->getTimerMap()->getTimerDurationAndReset("playerUpdateTimer")).count();

    if(isGrounded) {
        updateHorizontalMotion(milliseconds);
    }

    move(&motion);

    updatePlayerLookingAt(world);
}

void Player::updateServer(World* _world) {
    long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(world->getTimerMap()->getTimerDuration("playerUpdateTimer")).count();

    if(!isBlockUnderPlayer()) {
        motion[1] -= 0.038;
        motion[1] *= 0.98;
    }

    if(abs(motion[0]) < 0.005) {
        motion[0] = 0;
    }else {
        motion[0] *= 0.98;
    }

    if(abs(motion[2]) < 0.005) {
        motion[2] = 0;
    }else {
        motion[2] *= 0.98;
    }

    if(bufferedChunkLocation != world->getBlockData()->getChunkWithBlock(getPos().toBlockPos()).getChunkCoordinates() && !world->getBlockData()->getChunkWithBlock(getPos().toBlockPos()).isFakeChunk()) {
        setBufferedChunkLocation(world->getBlockData()->getChunkWithBlock(getPos().toBlockPos()).getChunkCoordinates());
        world->getBlockData()->updateLoadedChunks(bufferedChunkLocation, world);
    }else if(!world->getBlockData()->getChunkWithBlock(getPos().toBlockPos()).isFakeChunk()){
        setBufferedChunkLocation(world->getBlockData()->getChunkWithBlock(getPos().toBlockPos()).getChunkCoordinates());
    }
}

//todo -> speed up validation checks and lookat checks
void Player::listenTo(std::shared_ptr<Event> e) {
    if(e->getEventID() == "KEYPRESSED") {
        KeyPressedEvent keyEvent = *dynamic_cast<KeyPressedEvent*>(e.get());

        if(keyEvent.key == "1") {
            setItemInHand(std::unique_ptr<Item>(new ItemBlock(std::shared_ptr<Block>(new BlockDirt()))));
        }

        if(keyEvent.key == "2") {
            setItemInHand(std::unique_ptr<Item>(new ItemBlock(std::shared_ptr<Block>(new BlockCobblestone()))));
        }

        if(keyEvent.key == "u") {
            itemInHand->onRightClick(world);
            world->getTimerMap()->resetTimer("itemUseTimer");
        }
    }
    
    if(e->getEventID() == "KEYHELD") {
        KeyHeldEvent keyEvent = *dynamic_cast<KeyHeldEvent*>(e.get());

        long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(keyEvent.duration).count();

        if(keyEvent.key == "w") {
            ++zInputDirection;
        }

        if(keyEvent.key == "a") {
            --xInputDirection;
        }

        if(keyEvent.key == "s") {
            --zInputDirection;
        }

        if(keyEvent.key == "d") {
            ++xInputDirection;
        }

        if(keyEvent.key == "u") {
            if(std::chrono::duration_cast<std::chrono::milliseconds>(world->getTimerMap()->getTimerDuration("itemUseTimer")).count() > 150) {
                itemInHand->onRightClick(world);
                world->getTimerMap()->resetTimer("itemUseTimer");

            }
        }

        if(keyEvent.key == " ") {
            if(isGrounded) {
                isJumping = true;
                motion[1] = 0.38;
            }
        }

        if(keyEvent.key == "RIGHT_SHIFT") {
            isSneaking = true;
        }

        if(keyEvent.key == "LEFT_SHIFT") {
            isSprinting = true;
        }
    }

    if(e->getEventID() == "KEYRELEASED") {
        KeyReleasedEvent keyEvent = *dynamic_cast<KeyReleasedEvent*>(e.get());

        if(keyEvent.key == "RIGHT_SHIFT") {
            isSneaking = false;
        }

        if(keyEvent.key == "LEFT_SHIFT") {
            isSprinting = false;
        }
    }

    if(e->getEventID() == "MOUSEMOVED") {
        MouseMovedEvent mouseEvent = *dynamic_cast<MouseMovedEvent*>(e.get());
        yaw -= mouseEvent.xOffset;
        pitch -= mouseEvent.yOffset;

        if(pitch > 89) {
            pitch = 89;
        }
        if(pitch < -89) {
            pitch = -89;
        }
    }

    if(e->getEventID() == "LEFTMOUSEBUTTONPRESSED") {
        LeftMouseButtonPressedEvent mouseEvent = *dynamic_cast<LeftMouseButtonPressedEvent*>(e.get());
        itemInHand->onLeftClick(world, blockLookingAt);
    }

    if(e->getEventID() == "RIGHTMOUSEBUTTONPRESSED") {
        itemInHand->onRightClick(world);
        world->getTimerMap()->resetTimer("itemUseTimer");
    }
}

AABB Player::getAABB() {
    return AABB(0, 0, 0, 0.6, (isSneaking) ? sneakingHeight : standingHeight, 0.6);
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

bool Player::validatePosition(Pos newPosition, BlockArrayData data) {
    AABB playerAABB = getAABB();
    playerAABB.add(newPosition);

    for(int i = 0; i < data.getLoadedChunkLocations().size(); ++i) {
        Chunk c = data.getChunkWithBlock(data.getLoadedChunkLocations().at(i).chunkLocation);
        AABB chunkAABB = c.getChunkAABB();

        BlockPos chunkCoords = c.getChunkCoordinates();
        chunkAABB.add(chunkCoords);

        if(AABBIntersectedByAABB(playerAABB, chunkAABB) && !c.isFakeChunk()) {
            for(int k = 0; k < c.getBlocksInChunk().size(); ++k) {
                std::shared_ptr<Block> block = c.getBlocksInChunk().at(k);
                AABB blockAABB = block->getAABB();
                blockAABB.add(block->getPos());

                if(AABBIntersectedByAABB(playerAABB, blockAABB)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Player::validatePosition(Pos newPosition, BlockArrayData data, float* yToSnapTo) {
    AABB playerAABB = getAABB();
    playerAABB.add(newPosition);

    for(int i = 0; i < data.getLoadedChunkLocations().size(); ++i) {
        Chunk c = data.getChunkWithBlock(data.getLoadedChunkLocations().at(i).chunkLocation);
        AABB chunkAABB = c.getChunkAABB();
        
        BlockPos chunkCoords = c.getChunkCoordinates();
        chunkAABB.add(chunkCoords);
        
        if(AABBIntersectedByAABB(playerAABB, chunkAABB) && !c.isFakeChunk()) {
            for(int k = 0; k < c.getBlocksInChunk().size(); ++k) {
                std::shared_ptr<Block> block = c.getBlocksInChunk().at(k);
                AABB blockAABB = block->getAABB();
                blockAABB.add(block->getPos());

                if(AABBIntersectedByAABB(playerAABB, blockAABB)) {
                    yToSnapTo[0] = blockAABB.startY + blockAABB.ySize;
                    return false;
                }
            }
        }
    }
    return true;
}

Pos Player::getCameraPosition() {
    glm::vec3 nonRotatedPos = glm::vec3(getPos().x + getAABB().xSize / 2, getPos().y + getAABB().ySize * 3.0 / 4.0, getPos().z + getAABB().zSize / 2);
    glm::vec3 rotatedPos = (WorldRenderer::calculateXRotationMatrix(-getXRotation()) * glm::vec3(0, 0, 0));
    
    return Pos(rotatedPos.x + nonRotatedPos.x, rotatedPos.y + nonRotatedPos.y, rotatedPos.z + nonRotatedPos.z);
}

void Player::updatePlayerLookingAt(World* world) {
    bool isLooking = false;
    float previousT = -1;

    int side = 0;

    std::vector<Chunk> chunksCrossed = std::vector<Chunk>();

    for(int i = 0; i < world->getBlockData()->getLoadedChunkLocations().size(); ++i) {
        Chunk chunk = world->getBlockData()->getChunkWithBlock(world->getBlockData()->getLoadedChunkLocations().at(i).chunkLocation);
        if(!chunk.isFakeChunk()) {
            AABB aabb = chunk.getChunkAABB();
            aabb.add(chunk.getChunkCoordinates());

            int sideIntersect = 0;

            float t = raycast(aabb, &sideIntersect);

            if(t != -1 && (t < previousT || previousT == -1)) {
                chunksCrossed.push_back(chunk);
            }
        }
    }

    for(int j = 0; j < chunksCrossed.size(); ++j) {
        for(int k = 0; k < chunksCrossed.at(j).getBlocksInChunk().size(); ++k) {
            std::shared_ptr<Block> block = chunksCrossed.at(j).getBlocksInChunk().at(k);

            AABB aabb = block->getAABB();
            aabb.add(block->getPos());

            int sideIntersect = 0;

            float t = raycast(aabb, &sideIntersect);

            if(t != -1 && (t < previousT || previousT == -1) && t <= 5) {
                Pos normal = getCameraNormal();
                Pos lookVector = Pos(normal.x * t, normal.y * t, normal.z * t);
                internalBlockLookingAt = block->getPos();
                isLooking = true;
                previousT = t;
                if(sideIntersect != 0) {
                    side = sideIntersect;
                }
            }
        }
    }

    if(!isLooking) {
        blockLookingAt = nullptr;
        sideOfBlockLookingAt = 0;
    }else {
        blockLookingAt = &internalBlockLookingAt;
        sideOfBlockLookingAt = side;
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

    glm::vec3 n1 = WorldRenderer::calculateYRotationMatrix(-getYRotation()) * d;

    glm::vec3 n2 = WorldRenderer::calculateXRotationMatrix(-getXRotation()) * n1;

    return Pos(n2[0], n2[1], n2[2]);
}

float Player::raycast(AABB aabb, int* side) {
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
        *side = 1;
    }else if(t2 == tmin) {
        *side = 2;
    }else if(t3 == tmin) {
        *side = 3;
    }else if(t4 == tmin) {
        *side = 4;
    }else if(t5 == tmin) {
        *side = 5;
    }else if(t6 == tmin) {
        *side = 6;
    }
    return tmin;
}

int Player::getSideOfBlockLookingAt() {
    return sideOfBlockLookingAt;
}

void Player::setItemInHand(std::unique_ptr<Item> item) {
    itemInHand = std::move(item);
}

void Player::setBufferedChunkLocation(BlockPos pos) {
    this->bufferedChunkLocation = pos;
}


void Player::move(glm::vec3* moveVec) {    
    double d0 = pos.x;
    double d1 = pos.y;
    double d2 = pos.z;

    double d3 = (*moveVec)[0];
    double d4 = (*moveVec)[1];
    double d5 = (*moveVec)[2];
    bool flag = isGrounded && isSneaking;

    if (flag) {
        d3 = d3 / 3;
        d5 = d5 / 3;
    }

    flag = isGrounded && !isSneaking && isSprinting;

    if(flag) {
        d3 = d3 * 1.3;
        d5 = d5 * 1.3;
    }

    if (!isGrounded && isSneaking) {
        d3 = d3 / 2;
        d5 = d5 / 2;
    }

    Pos previousPosX = pos;

    pos.x += d3;

    if(!this->validatePosition(pos, *world->getBlockData())) {
        pos = previousPosX;
    }

    Pos previousPosZ = pos;

    pos.z += d5;

    if(!this->validatePosition(pos, *world->getBlockData())) {
        pos = previousPosZ;
        pos = previousPosX;
        pos.z += d5;
        sneakPos = pos;
        if(!this->validatePosition(pos, *world->getBlockData())) {
            pos = previousPosZ;
        }
    }

    Pos previousPos = pos;

    float yPosToSnapTo[1] = {pos.y};

    pos.y += d4;

    if(!this->validatePosition(pos, *world->getBlockData(), yPosToSnapTo)) {
        pos = previousPos;
        if(d4 < 0) {
            pos.y = yPosToSnapTo[0];
            isJumping = false;
        }
        (*moveVec)[1] = 0;
    }

    if(!isBlockUnderPlayer() && isGrounded && !isJumping && isSneaking) {
        pos = sneakPos;

        (*moveVec)[1] = 0;
    }else if(!isJumping && isSneaking && isBlockUnderPlayer()) {
        sneakPos = pos;
    }

    if(!isBlockUnderPlayer()) {
        isGrounded = false;
    }else {
        isGrounded = true;
    }
}

void Player::updateHorizontalMotion(long milliseconds) {
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

    double xMovRel = milliseconds / 1000.0 * speed * unitX;
    double zMovRel = milliseconds / 1000.0 * speed * unitZ;

    double degRADS = yaw / 180.0 * M_PI;
    double xMovAbs = xMovRel * cos(degRADS) - zMovRel * sin(degRADS);
    double zMovAbs = zMovRel * cos(degRADS) + xMovRel * sin(degRADS);

    motion[0] = xMovAbs;
    motion[2] = zMovAbs;

    xInputDirection = 0;
    zInputDirection = 0;
}

bool Player::isBlockUnderPlayer() {
    BlockArrayData* data = world->getBlockData();
    Pos playerPos = getPos();
    
    pos.y -= 0.02;
    if(!this->validatePosition(pos, *world->getBlockData())) {
        pos.y += 0.02;
        return true;
    }
    pos.y += 0.02;
    return false;
}
//fix sneak float
