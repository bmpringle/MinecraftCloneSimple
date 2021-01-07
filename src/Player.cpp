#include "Player.h"
#include "World.h"
#include <iostream>
#include "Events.h"
#include <math.h>
#include "RenderHelper.h"
#include <simd/matrix.h>
#include "Blocks.h"

Player::Player(World* _world) : pos(Pos(0, 5, 0)), world(_world) {
    world->getTimerMap()->addTimerToMap("playerUpdateTimer");
    blockInHand = std::shared_ptr<Block>(new BlockDirt());
}

void Player::updatePlayerInWorld(World* world) {  
    Pos previousPos = pos;

    //handle space bar and gravity from events
    long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(world->getTimerMap()->getTimerDurationAndReset("playerUpdateTimer")).count();

    currentYSpeed -= (world->getWorldGravity() / 1000.0 * milliseconds);

    currentYSpeed = (currentYSpeed < -78.4) ? -78.4 : currentYSpeed;

    pos.y += (currentYSpeed / 1000.0 * milliseconds);

    float yPosToSnapTo[1] = {pos.y};

    if(!this->validatePosition(pos, *world->getBlockData(), yPosToSnapTo)) {
        pos = previousPos;
        if(currentYSpeed < 0) {
            pos.y = yPosToSnapTo[0];
        }
        currentYSpeed = 0;
    }


    previousPos = pos;

    //handle WASD movement input grabbed during events
    double duration = moveVector.durationInMilliseconds;

    float unitX = 0;
    float unitZ = 0;

    if(moveVector.relativeZ > 0) {
        if(moveVector.relativeX > 0) {
            unitX = sqrt(2)/2;
            unitZ = sqrt(2)/2;
        }

        if(moveVector.relativeX == 0) {
            unitZ = 1;
        }

        if(moveVector.relativeX < 0) {
            unitX = -sqrt(2)/2;
            unitZ = sqrt(2)/2;
        }
    }

    if(moveVector.relativeZ == 0) {
        if(moveVector.relativeX > 0) {
            unitX = 1;
        }

        if(moveVector.relativeX == 0) {
            //dont move at all
        }

        if(moveVector.relativeX < 0) {
            unitX = -1;
        }
    }

    if(moveVector.relativeZ < 0) {
        if(moveVector.relativeX > 0) {
            unitX = sqrt(2)/2;
            unitZ = -sqrt(2)/2;
        }

        if(moveVector.relativeX == 0) {
            unitZ = -1;
        }

        if(moveVector.relativeX < 0) {
            unitX = -sqrt(2)/2;
            unitZ = -sqrt(2)/2;            
        }
    }

    double xMovRel = duration / 1000.0 * speed * unitX;
    double zMovRel = duration / 1000.0 * speed * unitZ;

    double degRADS = yaw/180.0*M_PI;
    double xMovAbs = xMovRel * cos(degRADS) - zMovRel * sin(degRADS);
    double zMovAbs = zMovRel * cos(degRADS) + xMovRel * sin(degRADS);

    pos.x += xMovAbs;

    if(!this->validatePosition(pos, *world->getBlockData())) {
        pos = previousPos;
    }

    previousPos = pos;

    pos.z += zMovAbs;

    if(!this->validatePosition(pos, *world->getBlockData())) {
        pos = previousPos;
    }

    moveVector.durationInMilliseconds = 0;
    moveVector.relativeX = 0;
    moveVector.relativeZ = 0;

    updatePlayerLookingAt(world);
}

void Player::listenTo(std::shared_ptr<Event> e) {
    if(e->getEventID() == "KEYHELD") {
        KeyHeldEvent keyEvent = *dynamic_cast<KeyHeldEvent*>(e.get());

        long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(keyEvent.duration).count();

        if(!keyEvent.key) {
            return;
        }

        if(keyEvent.key == 'w') {
            if(moveVector.durationInMilliseconds == 0) {
                moveVector.durationInMilliseconds = milliseconds;
            }
            ++moveVector.relativeZ;
        }

        if(keyEvent.key == 'a') {
            if(moveVector.durationInMilliseconds == 0) {
                moveVector.durationInMilliseconds = milliseconds;
            }
            --moveVector.relativeX;
        }

        if(keyEvent.key == 's') {
            if(moveVector.durationInMilliseconds == 0) {
                moveVector.durationInMilliseconds = milliseconds;
            }
            --moveVector.relativeZ;
        }

        if(keyEvent.key == 'd') {
            if(moveVector.durationInMilliseconds == 0) {
                moveVector.durationInMilliseconds = milliseconds;
            }
            ++moveVector.relativeX;
        }

        if(keyEvent.key == ' ') {
            if(currentYSpeed == 0) {
                currentYSpeed = 10;
            }
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
        if(this->blockLookingAt != nullptr) {
            BlockPos selected = *blockLookingAt;
            world->getBlockData()->removeBlockAtPosition(selected);
        }
    }

    if(e->getEventID() == "RIGHTMOUSEBUTTONPRESSED") {
        RightMouseButtonPressedEvent mouseEvent = *dynamic_cast<RightMouseButtonPressedEvent*>(e.get());
        if(this->blockLookingAt != nullptr) {
            BlockPos location = BlockPos(0, 0, 0);

            switch(sideOFBlockLookingAt) {
                default:
                    break;
                case 1:
                    location = BlockPos(blockLookingAt->x - 1, blockLookingAt->y, blockLookingAt->z);
                    if(world->getBlockData()->getBlockAtPosition(location) == nullptr && blockInHand != nullptr) {
                        world->getBlockData()->setBlockAtPosition(location, blockInHand);
                        blockInHand = std::shared_ptr<Block>(new BlockDirt());
                        bool valid = validatePosition(pos, *world->getBlockData());
                        if(!valid) {
                            world->getBlockData()->removeBlockAtPosition(location);
                        }else {
                            blockInHand = std::shared_ptr<Block>(new BlockDirt());
                        }                        
                    }
                    break;
                case 2:
                    location = BlockPos(blockLookingAt->x + 1, blockLookingAt->y, blockLookingAt->z);
                    if(world->getBlockData()->getBlockAtPosition(location) == nullptr && blockInHand != nullptr) {
                        world->getBlockData()->setBlockAtPosition(location, blockInHand);
                        blockInHand = std::shared_ptr<Block>(new BlockDirt());
                        bool valid = validatePosition(pos, *world->getBlockData());
                        if(!valid) {
                            world->getBlockData()->removeBlockAtPosition(location);
                        }else {
                            blockInHand = std::shared_ptr<Block>(new BlockDirt());
                        }                        
                    }
                    break;
                case 3:
                    location = BlockPos(blockLookingAt->x, blockLookingAt->y - 1, blockLookingAt->z);
                    if(world->getBlockData()->getBlockAtPosition(location) == nullptr && blockInHand != nullptr) {
                        world->getBlockData()->setBlockAtPosition(location, blockInHand);
                        blockInHand = std::shared_ptr<Block>(new BlockDirt());
                        bool valid = validatePosition(pos, *world->getBlockData());
                        if(!valid) {
                            world->getBlockData()->removeBlockAtPosition(location);
                        }else {
                            blockInHand = std::shared_ptr<Block>(new BlockDirt());
                        }                        
                    }
                    break;
                case 4:
                    location = BlockPos(blockLookingAt->x, blockLookingAt->y + 1, blockLookingAt->z);
                    if(world->getBlockData()->getBlockAtPosition(location) == nullptr && blockInHand != nullptr) {
                        world->getBlockData()->setBlockAtPosition(location, blockInHand);
                        bool valid = validatePosition(pos, *world->getBlockData());
                        if(!valid) {
                            world->getBlockData()->removeBlockAtPosition(location);
                        }else {
                            blockInHand = std::shared_ptr<Block>(new BlockDirt());
                        }
                    }
                    break;
                case 5:
                    location = BlockPos(blockLookingAt->x, blockLookingAt->y, blockLookingAt->z - 1);
                    if(world->getBlockData()->getBlockAtPosition(location) == nullptr && blockInHand != nullptr) {
                        world->getBlockData()->setBlockAtPosition(location, blockInHand);
                        blockInHand = std::shared_ptr<Block>(new BlockDirt());
                        bool valid = validatePosition(pos, *world->getBlockData());
                        if(!valid) {
                            world->getBlockData()->removeBlockAtPosition(location);
                        }else {
                            blockInHand = std::shared_ptr<Block>(new BlockDirt());
                        }                        
                    }
                    break;
                case 6:
                    location = BlockPos(blockLookingAt->x, blockLookingAt->y, blockLookingAt->z + 1);
                    if(world->getBlockData()->getBlockAtPosition(location) == nullptr && blockInHand != nullptr) {
                        world->getBlockData()->setBlockAtPosition(location, blockInHand);
                        blockInHand = std::shared_ptr<Block>(new BlockDirt());
                        bool valid = validatePosition(pos, *world->getBlockData());
                        if(!valid) {
                            world->getBlockData()->removeBlockAtPosition(location);
                        }else {
                            blockInHand = std::shared_ptr<Block>(new BlockDirt());
                        }                        
                    }
                    break;
            }
        }
    }
}

AABB Player::getAABB() {
    return AABB(0, 0, 0, 0.5, 2, 0.5);
}

RenderedModel Player::getRenderedModel() {
    //RenderedPoint p1 = RenderedPoint(0, 0, 0, /**uv coords*/ 0, 0, 0);
    //RenderedPoint p2 = RenderedPoint(0.5, 0, 0, /**uv coords*/ 0, 0, 0);
    //RenderedPoint p3 = RenderedPoint(0.5, 0, 0.5, /**uv coords*/ 0, 0, 0);
    //RenderedPoint p4 = RenderedPoint(0, 0, 0.5, /**uv coords*/ 0, 0, 0);
    //RenderedPoint p5 = RenderedPoint(0, 2, 0, /**uv coords*/ 0, 0, 0);
    //RenderedPoint p6 = RenderedPoint(0.5, 2, 0, /**uv coords*/ 0, 0, 0);
    //RenderedPoint p7 = RenderedPoint(0.5, 2, 0.5, /**uv coords*/ 0, 0, 0);
    //RenderedPoint p8 = RenderedPoint(0, 2, 0.5, /**uv coords*/ 0, 0, 0);

    /*RenderedTriangle t1 = RenderedTriangle(p1, p2, p3);
    RenderedTriangle t2 = RenderedTriangle(p1, p4, p3);
    RenderedTriangle t3 = RenderedTriangle(p5, p6, p7);
    RenderedTriangle t4 = RenderedTriangle(p5, p8, p7);
    RenderedTriangle t5 = RenderedTriangle(p1, p4, p8);
    RenderedTriangle t6 = RenderedTriangle(p1, p5, p8);
    RenderedTriangle t7 = RenderedTriangle(p2, p3, p7);
    RenderedTriangle t8 = RenderedTriangle(p2, p6, p7);
    RenderedTriangle t9 = RenderedTriangle(p1, p2, p5);
    RenderedTriangle t10 = RenderedTriangle(p2, p5, p6);
    RenderedTriangle t11 = RenderedTriangle(p3, p4, p8);
    RenderedTriangle t12 = RenderedTriangle(p3, p7, p8);

    RenderedTriangle triangleArray[12] = {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12};*/
    std::vector<RenderedTriangle> triangles = std::vector<RenderedTriangle>();

    /*for(RenderedTriangle triangle : triangleArray) {
        triangles.push_back(triangle);
    }*/

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

    for(int i = 0; i < data.getRawBlockArray().size(); ++i) {
        Block b = *data.getRawBlockArray().at(i);
        AABB blockAABB = b.getAABB();
        blockAABB.add(b.getPos());

        if(AABBIntersectedByAABB(playerAABB, blockAABB)) {
            return false;
        }
    }
    return true;
}

bool Player::validatePosition(Pos newPosition, BlockArrayData data, float* yToSnapTo) {
    AABB playerAABB = getAABB();
    playerAABB.add(newPosition);

    for(int i = 0; i < data.getRawBlockArray().size(); ++i) {
        Block b = *data.getRawBlockArray().at(i);
        AABB blockAABB = b.getAABB();
        blockAABB.add(b.getPos());

        if((blockAABB.startY < playerAABB.startY + playerAABB.ySize) && AABBIntersectedByAABB(playerAABB, blockAABB)) {
            yToSnapTo[0] = blockAABB.startY + blockAABB.ySize;
            return false;
        }
    }
    return true;
}

Pos Player::getCameraPosition() {
    return Pos(getPos().x + getAABB().xSize / 2, getPos().y + getAABB().ySize * 3.0 / 4.0, getPos().z + getAABB().zSize / 2);
}

void Player::updatePlayerLookingAt(World* world) {
    bool isLooking = false;
    float previousT = -1;

    int side = 0;

    for(int i = 0; i < world->getBlockData()->getRawBlockArray().size(); ++i) {
        std::shared_ptr<Block> block = world->getBlockData()->getRawBlockArray().at(i);
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

    if(!isLooking) {
        blockLookingAt = nullptr;
        sideOFBlockLookingAt = 0;
    }else {
        blockLookingAt = &internalBlockLookingAt;
        sideOFBlockLookingAt = side;
    }
}

BlockPos* Player::getBlockLookingAt() {
    return blockLookingAt;
}

Pos Player::getCameraNormal() {
    simd_float3 d = simd_float3();
    d[0] = 0;
    d[1] = 0;
    d[2] = 1;

    simd_float3 n1 = simd_mul(calculateYRotationMatrix(-getYRotation()), d);

    simd_float3 n2 = simd_mul(calculateXRotationMatrix(-getXRotation()), n1);

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